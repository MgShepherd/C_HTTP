#include <client.h>
#include <server.h>

#include "argparse.h"

int main(int argc, char **argv) {
  H_Server server = {0};
  CmdArgs args = parse_cmd_args(argc, argv);

  if (args.mode == MODE_SERVER) {
    h_server_start(&server);
    h_server_stop(&server);
  } else if (args.mode == MODE_CLIENT) {
    h_client_connect();
  } else {
    display_usage_text();
  }

  return 0;
}
