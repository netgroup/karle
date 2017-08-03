#include "global.h"

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
  // Initialize node_state
  memset((void *) & node_state, 0, sizeof(struct state));
  // Set timeout options for socket
  memset((void *) &tval, 0, sizeof(tval));
  tval.tv_sec = CLIENT_TIMEOUT_S;
  tval.tv_usec = (CLIENT_TIMEOUT_MS % 1000) * 1000;
  // Create the socket
  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  // Creation was not possible
  if(sockfd < 0)
    error("Error opening socket");
  // Set timeout opetion for rcv
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
    // Prepare the probe packet
    memset((void*) &probe, 0, sizeof(struct probe));
    // Reset ts_buff
    memset((void*) ts_buff, 0, sizeof(ts_buff));
    // Take current time
    ts = get_time64();
    get_time(ts_buff, ts);
    // Insert ts in the packet
    memcpy(probe.ts_s_c, ts_buff, sizeof(ts_buff));


    // Reset ts_buff
    memset((void*) ts_buff, 0, sizeof(ts_buff));
    // Convert last_ts
    get_time(ts_buff, node_state.last_ts);
    // Insert ts in the packet
    memcpy(probe.ts_r_c, ts_buff, sizeof(ts_buff));


    // Send probe to the server
    n = sendto(sockfd, &probe, sizeof(struct probe), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    // There was an error
    if(n < 0)
      error("ERROR in sendto");
    // Take time before read
    start = get_time64();
    // Read datagram
    n = recvfrom(sockfd, &probe, sizeof(struct probe), 0, NULL, NULL);
    // Get time at receiving
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
      // Reset last ts
      node_state.last_ts = 0;
      // Waiting for the next probe
      continue;
    }
    // If we are in the loss interval
    if(evaluate_loss){
      // Reset boolean
      evaluate_loss = false;
      // Update last evaluation
      node_state.last_evaluation = ts;
      // Evaluate loss and update EWMA
      printf("LOSS: %f\n", 0.0);
      // Print EWMAs and last info
      printf("EWMA LOSS: %f\n", 0.0);
    }
    // Evaluate RTT and update EWMA
    get_time(ts_buff, ts);
    ts4 = get_time32(ts_buff);
    ts3 = get_time32(probe.ts_s_s);
    ts2 = get_time32(probe.ts_r_s);
    ts1 = get_time32(probe.ts_s_c);
    rtt = (ts4-ts1)-(ts3-ts2);
    // Debug print
    printf("ts4: %u us\n", ts4);
    printf("ts3: %u us\n", ts3);
    printf("ts2: %u us\n", ts2);
    printf("ts1: %u us\n", ts1);
    printf("RTT: %u us\n", rtt);
    printf("EWMA RTT: %u us\n", 0);


    // Update node_state
    node_state.last_ts = ts;


    // Calculate the time spent during receiving
    ts = get_time64();
    // Calculate the time to sleep removing the time
    // spent during processing
    to_sleep = PROBE_INTERVAL_US - (ts - start);
    // Sleep for an amount of seconds to guarantee
    // the defined probe interval
    if (to_sleep > 0)
      usleep(to_sleep);
  }
  // Done, exit
  return 0;
}