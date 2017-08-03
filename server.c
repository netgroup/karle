#include "global.h"

int main(int argc, char *argv[]) {
  // Init steps
  int sockfd;
  int n;
  struct sockaddr_in serv_addr;
  struct sockaddr cli_addr;
  socklen_t cli_addr_len = sizeof(cli_addr);
  struct timeval tval;
  bool evaluate_loss = false;
  unsigned char ts_buff[3];
  uint64_t ts;
  uint32_t ts1, ts2, ts3, ts4, rtt;
  uint64_t Ss = 0;
  uint64_t Rs = 0;
  uint64_t rcv_by_client = 0;
  uint64_t sent_server_seen_by_client = 0;
  double loss = 0.0;
  // Set timeout options for socket
  memset((void *) &tval, 0, sizeof(tval));
  tval.tv_sec = SERVER_TIMEOUT_S;
  tval.tv_usec = (SERVER_TIMEOUT_MS % 1000) * 1000;
  // Set up socket
  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  // Creation was not possible
  if (sockfd < 0)
    error("ERROR opening socket");
  // Set timeout option for rcv
  if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tval, sizeof(tval)) < 0)
    error("Error setting rcv timeout");
  // Set binding address
  memset((char *) &serv_addr, '\0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(PORT_SERVER);
  // Bind to the given address
  if(bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    error("ERROR on binding");
  // Set the first ts for the loss evaluation
  node_state.last_evaluation = get_time64();
  // Print debug
  printf("Waiting for packets\n");
  // Main loop dispatacher
  while(1){
    // Read datagram
    n = recvfrom(sockfd, &probe, sizeof(struct probe), 0, &cli_addr, &cli_addr_len);
    // Get time at receiving ts_r_s
    ts = get_time64();
    // There was an error
    if(n < sizeof(struct probe))
      error("ERROR in recvfrom");
    // Loss evaluation
    if((ts - node_state.last_evaluation) >= EVALUATION_TIMEOUT_US){
      // Loss will be evaluated
      // with the recv probe
      evaluate_loss = true;
    }
    // Timeout
    if(n < 0) {
      perror("Timeout");
      // Prepare the probe packet
      memset((void*) &probe, 0, sizeof(struct probe));
      // Waiting for the next probe
      continue;
    }
    // Let's increment Rs
    Rs = get_and_increment_rcv();
    // Let's update Sc sent by the client
    node_state.sent_temp = probe.sent1;
    // If we are in the loss interval
    if(evaluate_loss){
      // Reset boolean
      evaluate_loss = false;
      // Update last evaluation
      node_state.last_evaluation = ts;
      // Rc(k) - Rc_last
      rcv_by_client = probe.rcv - node_state.last_rcv;
      // Ss(k) - Ss_last
      sent_server_seen_by_client = probe.sent2 - node_state.last_sent;
      // Evaluate loss
      loss = 1 - (double)rcv_by_client/(double)sent_server_seen_by_client;
      // Update EWMA

      // Debug print
      if (DEBUG) {
        // Print EWMAs and last info
        printf("Ss: %lu\n", node_state.sent);
        printf("Rs: %lu\n", node_state.rcv);
        printf("Sc: %lu\n", node_state.sent_temp);
        printf("Rc: %lu\n", probe.rcv);
        printf("Ss by client: %lu\n", probe.sent2);
        printf("Ss_last: %lu\n", node_state.last_sent);
        printf("Rc_last: %lu\n", node_state.last_rcv);
        printf("LOSS: %f\n", loss);
        printf("EWMA LOSS: %f\n", 0.0);
      }
      // Update last_sent (Ss_last) with last sent seen from the client
      node_state.last_sent = probe.sent2;
      // Update last_rcv (Rc_last) with last rcv from the client
      node_state.last_rcv = probe.rcv;
    }
    // Convert ts_r_s for the probe
    get_time(ts_buff, ts);
    // Copy in the packet
    memcpy(probe.ts_r_s, ts_buff, sizeof(ts_buff));
    // Evaluate RTT
    ts4 = get_time32(ts_buff);
    ts3 = get_time32(probe.ts_s_c);
    ts2 = get_time32(probe.ts_r_c);
    ts1 = get_time32(probe.ts_s_s);
    // Update EWMA

    // Server cannot calculate the first time the RTT
    if(ts4 > 0 && ts3 > 0 && ts2 >0 && ts1 > 0) {
      rtt = (ts4-ts1)-(ts3-ts2);
      // Debug print
      if (DEBUG) {
        printf("ts4: %u us\n", ts4);
        printf("ts3: %u us\n", ts3);
        printf("ts2: %u us\n", ts2);
        printf("ts1: %u us\n", ts1);
        printf("RTT: %u us\n", rtt);
        printf("EWMA RTT: %u us\n", 0);
      }
    }
    // Get time before sending
    ts = get_time64();
    // Convert ts_s_s for the probe
    get_time(ts_buff, ts);
    // Copy in the packet
    memcpy(probe.ts_s_s, ts_buff, sizeof(ts_buff));
    // Let's increment Ss
    Ss = get_and_increment_sent();
    // Push Ss(k)
    probe.sent1 = Ss;
    // Push Sc(k)
    probe.sent2 = node_state.sent_temp;
    // Push Rs(k)
    probe.rcv = Rs;
    // Send back the probe to the client
    n = sendto(sockfd, &probe, sizeof(struct probe), 0, (struct sockaddr *)&cli_addr, sizeof(serv_addr));
    // There was an error
    if(n < 0)
      error("ERROR in sendto");
  }
}
