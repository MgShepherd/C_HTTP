#include <client.h>
#include <server.h>

#include "argparse.h"
#include "common.h"

int main(int argc, char **argv) {
  CmdArgs args = parse_cmd_args(argc, argv);

  if (args.mode == MODE_SERVER) {
    H_Socket server = {0};
    h_server_start(&server);
    h_socket_close(&server);
  } else if (args.mode == MODE_CLIENT) {
    H_Socket client = {0};
    h_client_connect(&client);
    h_socket_close(&client);
  } else {
    display_usage_text();
  }

  return 0;
}
