#include "argparse.h"

#include <stdio.h>
#include <string.h>

void display_usage_text();

CmdArgs parse_cmd_args(int argc, char **argv) {
  CmdArgs args = {0};

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-c") == 0) {
      args.mode = MODE_CLIENT;
    } else if (strcmp(argv[i], "-s") == 0) {
      args.mode = MODE_SERVER;
    } else if (strcmp(argv[i], "-h") == 0) {
      args.mode = MODE_HELP;
    }
  }

  return args;
}

void display_usage_text() {
  printf("[OVERVIEW]: Application for running a TCP client or server\n\n");
  printf("[USAGE]: ./build/Sandbox [OPTIONS]\n\n");
  printf("[OPTIONS]:\n");
  printf("  -c Client Mode - For Connecting to an already running server\n");
  printf("  -s Server Mode - For Starting a new local server instance\n");
  printf("  -h Help - View this help text\n");
  printf("\n");
}
