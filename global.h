#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <netinet/in.h>
#include <string.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
// Our includes
#include "utilities.h"
#include "packet.h"
#include "state.h"

// IP used by server
#define IP_SERVER "10.0.0.3"
// Port used by client and server for the communication
#define PORT_SERVER 9999
// DEBUG level
#define DEBUG 1

// Probe interval in us
extern uint32_t PROBE_INTERVAL_US;
// Probe interval in ms
extern uint32_t PROBE_INTERVAL_MS;
// Probe interval in s
extern uint32_t PROBE_INTERVAL_S;
// Socket timeout for the client in us
extern uint32_t CLIENT_TIMEOUT_US;
// Socket timeout for the client in ms
extern uint32_t CLIENT_TIMEOUT_MS;
// Socket timeout for the client in s
extern uint32_t CLIENT_TIMEOUT_S;
// Socket timeout for the server in us
extern uint32_t SERVER_TIMEOUT_US;
// Socket timeout for the server in ms
extern uint32_t SERVER_TIMEOUT_MS;
// Socket timeout for the server in s
extern uint32_t SERVER_TIMEOUT_S;
// Evaluation timeout in us
extern uint32_t EVALUATION_TIMEOUT_US;
// Socket timeout for the server in ms
extern uint32_t EVALUATION_TIMEOUT_MS;
// Socket timeout for the server in s
extern uint32_t EVALUATION_TIMEOUT_S;

#endif
