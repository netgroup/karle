#ifndef _PACKET_H
#define _PACKET_H

// Probe structure
struct probe {
  // Ts when client sends
  unsigned char ts_s_c[3];
  // Ts when server receives
  unsigned char ts_r_s[3];
  // Ts when server sends
  unsigned char ts_s_s[3];
  // Ts when client receives
  unsigned char ts_r_c[3];
};

// Probe packet sent/received
struct probe probe;

#endif