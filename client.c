#include "global.h"

int main(int argc, char **argv) {
  // Init steps
  int sockfd;
  int n;
  in_addr_t s_addr;
  struct sockaddr_in serv_addr;
  struct timeval tval;
  // Create the socket
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  // Creation was not possible
  if (sockfd < 0)
      error("Error opening socket");
  // Set the socket address
  s_addr = inet_addr(IP_SERVER);
  memset((char *) &serv_addr, '\0', sizeof(serv_addr));
  memcpy(&serv_addr.sin_addr.s_addr, &s_addr, sizeof(s_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT_SERVER);
  // Main loop, send periodically udp probe
  while(1) {
    // Take current time
    gettimeofday(&tval, NULL);
    // Send current time to the server
    n = sendto(sockfd, &tval, sizeof(tval), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    // There was an error
    if(n < 0)
      error("ERROR in sendto");
    // Sleep for an amount of seconds to guarantee
    // the defined probe interval
    sleep(SLEEP_TIME);
  }
  // Done, exit
  return 0;
}