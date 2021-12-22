#include "third_party/ANSI-color-codes.h"

#define EXIT_CODE_SOCK_CREATE_ERROR     -1
#define EXIT_CODE_SOCK_SETOPT_ERROR     -2
#define EXIT_CODE_SOCK_BIND_FAILED      -3
#define EXIT_CODE_SOCK_LISTEN_FAILED    -4

// OPTIONS:
#define MAX_QUEUED_CONN_REQUESTS        3
#define MAX_CHUNK_SIZE                  1024

#define SERR_PREFIX     RED     "ERROR:\t"  reset
#define SINFO_PREFIX    GRN     "INFO:\t"   reset