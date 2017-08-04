#include "global.h"

uint32_t PROBE_INTERVAL_US = 0;
uint32_t PROBE_INTERVAL_MS = 0;
uint32_t PROBE_INTERVAL_S = 0;
uint32_t CLIENT_TIMEOUT_US = 0;
uint32_t CLIENT_TIMEOUT_MS = 0;
uint32_t CLIENT_TIMEOUT_S = 0;
uint32_t SERVER_TIMEOUT_US = 0;
uint32_t SERVER_TIMEOUT_MS = 0;
uint32_t SERVER_TIMEOUT_S = 0;
uint32_t EVALUATION_TIMEOUT_US = 0;
uint32_t EVALUATION_TIMEOUT_MS = 0;
uint32_t EVALUATION_TIMEOUT_S = 0;

int main(int argc, char **argv) {
  // Init steps
  int sockfd;
  int n;
  in_addr_t s_addr;
  struct sockaddr_in serv_addr;
  struct timeval tval;
  unsigned char ts_buff[3];
  uint64_t ts, start;
  bool evaluate_loss = false;
  uint32_t ts1, ts2, ts3, ts4, rtt;
  uint64_t to_sleep = 0;
  uint64_t last_ts = 0;
  uint64_t Sc = 0;
  uint64_t Rc = 0;
  uint64_t rcv_by_server = 0;
  uint64_t sent_client_seen_by_server = 0;
  double loss = 0.0;
  // Parse cmd line looking for ka period and ka timeout period
  if(parse_cmd_line(argc, argv) != 0){
    exit(1);
  }
  // Initialize node_state
  memset((void *) &node_state, 0, sizeof(struct state));
  // Set timeout options for socket
  memset((void *) &tval, 0, sizeof(tval));
  tval.tv_sec = CLIENT_TIMEOUT_S;
  tval.tv_usec = (CLIENT_TIMEOUT_MS % 1000) * 1000;
  // Create the socket
  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  // Creation was not possible
  if(sockfd < 0)
    error("Error opening socket");
  // Set timeout option for rcv
  if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tval, sizeof(tval)) < 0)
    error("Error setting rcv timeout");
  // Set the socket address
  s_addr = inet_addr(IP_SERVER);
  memset((void *) &serv_addr, '\0', sizeof(serv_addr));
  memcpy(&serv_addr.sin_addr.s_addr, &s_addr, sizeof(s_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT_SERVER);
  // Set the first ts for the loss evaluation
  node_state.last_evaluation = get_time64();
  // Main loop, send periodically udp probe
  while(1) {
    // Take current time
    ts = get_time64();
    // Convert to 24 bit
    get_time(ts_buff, ts);
    // Insert ts_s_c in the packet
    memcpy(probe.ts_s_c, ts_buff, sizeof(ts_buff));
    // Convert last_ts aka ts_r_c
    get_time(ts_buff, last_ts);
    // Insert ts_r_c in the packet
    memcpy(probe.ts_r_c, ts_buff, sizeof(ts_buff));
    // Let's increment Sc
    Sc = get_and_increment_sent();
    // Push Sc(k)
    probe.sent1 = Sc;
    // Push Ss(kprev)
    probe.sent2 = node_state.sent_temp;
    // Push Rc(kprev)
    probe.rcv = Rc;
    // Send probe to the server
    n = sendto(sockfd, &probe, sizeof(struct probe), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    // There was an error
    if(n < 0)
      error("ERROR in sendto");
    // Take the time before read
    start = get_time64();
    // Read datagram
    n = recvfrom(sockfd, &probe, sizeof(struct probe), 0, NULL, NULL);
    // Get time at receiving
    ts = get_time64();
    // Double copy
    last_ts = ts;
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
      // Reset last ts
      last_ts = 0;
      // Waiting for the next probe
      continue;
    }
    // Let's increment Rc
    Rc = get_and_increment_rcv();
    // Let's update Ss
    node_state.sent_temp = probe.sent1;
    // If we are in the loss interval
    if(evaluate_loss){
      // Reset boolean
      evaluate_loss = false;
      // Update last evaluation
      node_state.last_evaluation = ts;
      // Rs(k) - Rs_last
      rcv_by_server = probe.rcv - node_state.last_rcv;
      // Sc(k) - Sc_last
      sent_client_seen_by_server = probe.sent2 - node_state.last_sent;
      // Evaluate loss
      loss = 1 - (double)rcv_by_server/(double)sent_client_seen_by_server;
      // Update EWMA loss

      // Debug print
      if (DEBUG) {
        // Print EWMAs and last info
        printf("Sc: %lu\n", node_state.sent);
        printf("Rc: %lu\n", node_state.rcv);
        printf("Ss: %lu\n", node_state.sent_temp);
        printf("Rs: %lu\n", probe.rcv);
        printf("Sc by server: %lu\n", probe.sent2);
        printf("Sc_last: %lu\n", node_state.last_sent);
        printf("Rs_last: %lu\n", node_state.last_rcv);
        printf("LOSS: %f\n", loss);
        printf("EWMA LOSS: %f\n", 0.0);
      }
      // Update last_sent (Sc_last) with last sent seen from server
      node_state.last_sent = probe.sent2;
      // Update last_rcv (Rs_last) with last rcv from server
      node_state.last_rcv = probe.rcv;
    }
    // Evaluate RTT
    get_time(ts_buff, ts);
    ts4 = get_time32(ts_buff);
    ts3 = get_time32(probe.ts_s_s);
    ts2 = get_time32(probe.ts_r_s);
    ts1 = get_time32(probe.ts_s_c);
    rtt = (ts4-ts1)-(ts3-ts2);
    // Update EWMA RTT

    // Debug print
    if (DEBUG) {
      printf("ts4: %u us\n", ts4);
      printf("ts3: %u us\n", ts3);
      printf("ts2: %u us\n", ts2);
      printf("ts1: %u us\n", ts1);
      printf("RTT: %u us\n", rtt);
      printf("EWMA RTT: %u us\n", 0);
    }
    // Calculate the time spent during receiving
    ts = get_time64();
    // Calculate the time to sleep removing the time
    // spent during processing
    to_sleep = PROBE_INTERVAL_US - (ts - start);
    // Sleep for an amount of seconds to guarantee
    // the defined probe interval
    if (to_sleep > 0 &&  to_sleep <= PROBE_INTERVAL_US)
      usleep(to_sleep);
  }
  // Done, exit
  return 0;
}