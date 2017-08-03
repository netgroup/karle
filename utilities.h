#ifndef _UTILITIES_H
#define _UTILITIES_H

#include "global.h"

// Print error message and exit
void error(char *msg);
// Utility function returning ts as uint64_t
uint64_t get_time64();
// Utility function returning ts as 3-bytes array
void get_time(unsigned char * buff, uint64_t usec);
// Utility function converting ts from 3-bytes array to unsigned int
uint32_t get_time32(unsigned char* buff);
// Utility function incrementing sent counter
uint64_t get_and_increment_sent();
// Utility function incrementing sent rcv
uint64_t get_and_increment_rcv();

#endif
