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
  // Set timeout options for socket
  memset((void *) &tval, 0, sizeof(tval));
  tval.tv_sec = SERVER_TIMEOUT_S;
  tval.tv_usec = (SERVER_TIMEOUT_MS % 1000) * 1000;
  // Set up socket
  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  // Creation was not possible
  if (sockfd < 0)
    error("ERROR opening socket");
  // Set timeout opetion for rcv
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
  // Print debug
  printf("Waiting for packets\n");



  // Main loop dispatacher
  while(1){
    // Read datagram
    n = recvfrom(sockfd, &probe, sizeof(struct probe), 0, &cli_addr, &cli_addr_len);
    // Get time at receiving
    ts = get_time64();


    // Loss Evaluation
    /*if(loss_count >= LOSS){
      // Reset count
      loss_count = 0;
      // Loss will be evaluated
      // with the next probe
      evaluate_loss = true;
    }*/


    // Timeout
    if(n < 0) {
      perror("Timeout");
      // Prepare the probe packet
      memset((void*) &probe, 0, sizeof(struct probe));
      // Waiting for the next probe
      continue;
    }
    // If we are in the loss interval
    if(evaluate_loss){
      // Reset boolean
      evaluate_loss = false;
      // Evaluate loss and update EWMA
      printf("LOSS: %f\n", 0.0);
      // Print EWMAs and last info
      printf("EWMA LOSS: %f\n", 0.0);
    }


    // Convert ts_r_s for the probe
    get_time(ts_buff, ts);
    // Copy in the packet
    memcpy(probe.ts_r_s, ts_buff, sizeof(ts_buff));
    // Evaluate RTT and update EWMA
    ts4 = get_time32(ts_buff);
    ts3 = get_time32(probe.ts_s_c);
    ts2 = get_time32(probe.ts_r_c);
    ts1 = get_time32(probe.ts_s_s);

    // Server cannot calculate the first time the RTT
    if(ts4 > 0 && ts3 > 0 && ts2 >0 && ts1 > 0) {
      rtt = (ts4-ts1)-(ts3-ts2);
      // Debug print
      printf("ts4: %u us\n", ts4);
      printf("ts3: %u us\n", ts3);
      printf("ts2: %u us\n", ts2);
      printf("ts1: %u us\n", ts1);
      printf("RTT: %u us\n", rtt);
      printf("EWMA RTT: %u us\n", 0);
    }


    // Get time before to send
    ts = get_time64();
    // Convert ts_s_s for the probe
    get_time(ts_buff, ts);
    // Copy in the packet
    memcpy(probe.ts_s_s, ts_buff, sizeof(ts_buff));
    // Send back the probe to the client
    n = sendto(sockfd, &probe, sizeof(struct probe), 0, (struct sockaddr *)&cli_addr, sizeof(serv_addr));
    // There was an error
    if(n < 0)
      error("ERROR in sendto");
  }
}
