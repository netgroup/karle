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

/*
uint64_t get_and_increment_sent(){
  // Init steps
  int local_seq_num;
  // Move forward measure sequence number
  context->loc_seq_number = context->loc_seq_number + 1;
  // Take local seq number
  local_seq_num = context->loc_seq_number;
  // If we reached the max, let's restart with the sequence number
  if(context->loc_seq_number == INT_MAX)
    context->loc_seq_number = 0;
  // Done, return
  return local_seq_num;
}
*/

