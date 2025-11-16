#include <client.h>
#include <server.h>

int main() {
  Server server = {0};
  h_connect_to_server();
  h_server_start(&server);

  h_server_stop(&server);

  return 0;
}
