#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

#define SENDER_PORT 3490
#define SENDER_IP "10.0.0.1"

int main(int argc, char *argv[]) {
  // Init steps
  int sockfd;
  int rcv_num, loop_count, i;
  char buf;
  struct sockaddr_in sender_addr;
  //open socket and connect
  if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
    perror("socket error"); // do some error checking!
    exit(1);
  }
  // Set sender address
  sender_addr.sin_family = AF_INET;
  sender_addr.sin_port = htons(SENDER_PORT);
  sender_addr.sin_addr.s_addr = inet_addr(SENDER_IP);
  memset(sender_addr.sin_zero, '\0', sizeof(sender_addr.sin_zero));
  // Connect to the sender
  if((connect(sockfd,(struct sockaddr *)&sender_addr, sizeof(sender_addr))) == -1){
    perror("connect error");
    exit(1);
  }
  //connection established
  printf("Connection to sender established\n");
  //reads 100 packets of 1 byte and sends them back as ack packets
  printf("Receive 100 packets of 1 byte and send then back\n");
  for(i=0;i<100;i++){
    rcv_num = recv(sockfd, &buf, sizeof(char), 0);
    if(rcv_num!=0) {
      //send  ack
      send(sockfd, &buf, sizeof(char), 0);
    }
    else{
      perror("Receive error");
      exit(1);
    }
  }
  printf("\tDone\nClose socket and exit\n");
  close(sockfd);
  exit(0);
}