#include "global.h"

int main(int argc, char *argv[]) {
  // Init steps
  int sockfd;
  int n;
  struct sockaddr_in serv_addr;
  struct timeval tval_recv;
  struct timeval tval_now;
  // Set up socket
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  // Creation was not possible
  if (sockfd < 0)
    error("ERROR opening socket");
  // Set binding address
  memset((char *) &serv_addr, '\0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(PORT_SERVER);
  // Bind to the given address
  if(bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    error("ERROR on binding");
  printf("Waiting for packets\n");
  // Main loop dispatacher
  while(1){
    // Read datagram
    n = recvfrom(sockfd, &tval_recv, sizeof(tval_recv), 0, NULL, NULL);
    // There was an error
    if(n < 0)
      error("ERROR in recvfrom");
    // Get current time
    gettimeofday(&tval_now, NULL);
    // Print one way delay
    printf("%ld.%06ld\n", tval_now.tv_sec - tval_recv.tv_sec,
       (long int)(tval_now.tv_usec - tval_recv.tv_usec));
  }
}
