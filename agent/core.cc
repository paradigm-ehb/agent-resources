#include <grpcpp/server_builder.h>
#include <unistd.h>

#include "grpc_server/server.h"


int
main(void)
{
  grpc_server_setup();
  return 0;
}
