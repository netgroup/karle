#include "utilities.h"

void error(char *msg){
  perror(msg);
  exit(1);
}
