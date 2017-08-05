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

int parse_cmd_line(int argc, char **argv){
  // Init steps
  int index;
  int c;
  opterr = 0;
  // Iterating on arguments -k and -t
  while((c = getopt (argc, argv, "k:t:")) != -1){
    // Switch on options
    switch(c){
      // keep-alive period case
      case 'k':
        sscanf(optarg, "%u", &PROBE_INTERVAL_MS);
      break;
      // keep-alive timeout case
      case 't':
        sscanf(optarg, "%u", &EVALUATION_TIMEOUT_MS);
      break;
      // Help case
      case '?':
        if(optopt == 'k')
          fprintf(stderr, "Option -%c requires keep-alive period (ms) as argument.\n", optopt);
        if(optopt == 't')
          fprintf(stderr, "Option -%c requires keep-alive timeout period (ms) as argument.\n", optopt);
        else if(isprint (optopt))
          fprintf(stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
      // Default case
      default:
        fprintf(stderr, "ERROR cmd line - Please specify -k keep-alive period and -t keep-alive timeout period\n");
      return 1;
    }
  }
  // Sanity checks
  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);
  // Keep-alive period has to be a valid interval
  if (PROBE_INTERVAL_MS == 0){
    fprintf (stderr, "ERROR cmd line - Please specify -k keep-alive period (ms)\n");
    return 1;
  }
  // Keep-alive timeout period has to be a valid interval
  if (EVALUATION_TIMEOUT_MS == 0){
    fprintf (stderr, "ERROR cmd line - Please specify -t keep-alive timeout period (ms)\n");
    return 1;
  }
  // Define probe interval in us
  PROBE_INTERVAL_US = PROBE_INTERVAL_MS * 1000;
  // Define probe interval in s
  PROBE_INTERVAL_S = PROBE_INTERVAL_MS / 1000;
  // Define socket timeout for the client in us
  CLIENT_TIMEOUT_US = PROBE_INTERVAL_US;
  // Define socket timeout for the client in ms
  CLIENT_TIMEOUT_MS = PROBE_INTERVAL_MS;
  // Define socket timeout for the client in s
  CLIENT_TIMEOUT_S = PROBE_INTERVAL_S;
  // Define socket timeout for the server in us
  SERVER_TIMEOUT_US = 2 * CLIENT_TIMEOUT_US;
  // Define socket timeout for the server in ms
  SERVER_TIMEOUT_MS = 2 * CLIENT_TIMEOUT_MS;
  // Define socket timeout for the server in s
  SERVER_TIMEOUT_S = 2 * CLIENT_TIMEOUT_S;
  // Define evaluation timeout in us
  EVALUATION_TIMEOUT_US = 100 * PROBE_INTERVAL_US;
  // Define socket timeout for the server in ms
  EVALUATION_TIMEOUT_MS = 100 * PROBE_INTERVAL_MS;
  // Define socket timeout for the server in s
  EVALUATION_TIMEOUT_S = 100 * PROBE_INTERVAL_S;
  // Done, exit
  return 0;
}

