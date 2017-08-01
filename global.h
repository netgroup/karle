#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <netinet/in.h>
#include <string.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
// Our include
#include "utilities.h"

// IP used by server
#define IP_SERVER "10.0.0.3"
// Port used by client and server for the communication
#define PORT_SERVER 9999
// Probe interval in ms
#define PROBE_INTERVAL 2000
// Sleep time for the client
#define SLEEP_TIME PROBE_INTERVAL/1000

#endif
