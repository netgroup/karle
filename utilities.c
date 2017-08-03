#include "utilities.h"

void error(char *msg){
  // Print on stderror
  perror(msg);
  // Done exit
  exit(1);
}

uint64_t get_time64(){
  // Init step
  uint64_t usec;
  uint64_t million;
  struct timeval tv;
  // Get the timestamp
  gettimeofday(&tv, NULL);
  // Micro seconds
  million = 1000000;
  // Save for now the seconds
  usec = tv.tv_sec;
  // Finally convert everything in microseconds
  usec = usec * million + tv.tv_usec;
  // Done, return
  return usec;
}

void get_time(unsigned char * buffer, uint64_t usec){
  // Squeeze an u64 to 24 bit using a buffer
  usec = usec >> 4;
  buffer[0] = usec % 256;
  usec = usec >> 8;
  buffer[1] = usec % 256;
  usec = usec >> 8;
  buffer[2] = usec % 256;
}

uint32_t get_time32(unsigned char *buffer){
  // Init step
  uint32_t val;
  // Expand to 32 bit the previous value in 24 bit
  val = buffer[2];
  val = val << 8;
  val += buffer[1];
  val = val << 8;
  val += buffer[0];
  val = val << 4;
  return val;
}

uint64_t get_and_increment_sent(){
  // Init steps
  int sent;
  // Move forward sent
  node_state.sent = node_state.sent + 1;
  // Update local sent
  sent = node_state.sent;
  // If we reached the max, let's restart
  if(node_state.sent == UINT64_MAX)
    node_state.sent = 0;
  // Done, return
  return sent;
}

uint64_t get_and_increment_rcv(){
  // Init steps
  int rcv;
  // Move forward sent
  node_state.rcv = node_state.rcv + 1;
  // Update local sent
  rcv = node_state.rcv;
  // If we reached the max, let's restart
  if(node_state.rcv == UINT64_MAX)
    node_state.rcv = 0;
  // Done, return
  return rcv;
}

