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
// Our includes
#include "utilities.h"
#include "packet.h"
#include "state.h"

// IP used by server
#define IP_SERVER "10.0.0.3"
// Port used by client and server for the communication
#define PORT_SERVER 9999
// Probe interval in us
#define PROBE_INTERVAL_US PROBE_INTERVAL_MS * 1000
// Probe interval in ms
#define PROBE_INTERVAL_MS 100
// Probe interval in s
#define PROBE_INTERVAL_S PROBE_INTERVAL_MS/1000
// Socket timeout for the client in us
#define CLIENT_TIMEOUT_US PROBE_INTERVAL_US
// Socket timeout for the client in ms
#define CLIENT_TIMEOUT_MS PROBE_INTERVAL_MS
// Socket timeout for the client in s
#define CLIENT_TIMEOUT_S PROBE_INTERVAL_S
// Socket timeout for the server in us
#define SERVER_TIMEOUT_US 2 * CLIENT_TIMEOUT_US
// Socket timeout for the server in ms
#define SERVER_TIMEOUT_MS 2 * CLIENT_TIMEOUT_MS
// Socket timeout for the server in s
#define SERVER_TIMEOUT_S 2 * CLIENT_TIMEOUT_S
// Evaluation timeout in us
#define EVALUATION_TIMEOUT_US 100 * PROBE_INTERVAL_US
// Socket timeout for the server in ms
#define EVALUATION_TIMEOUT_MS 100 * PROBE_INTERVAL_MS
// Socket timeout for the server in s
#define EVALUATION_TIMEOUT_S 100 * PROBE_INTERVAL_S
// DEBUG level
#define DEBUG 1

#endif
