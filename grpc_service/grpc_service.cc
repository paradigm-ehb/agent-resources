#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <memory>

#include "grpc_service/grpc_service.h"
#include "base/base.h"

#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>


/*
 * grpc server setup
 */
local_internal void
grpc_server_setup()
{

  const char *server_address = "0.0.0.0:5000";

  grpc::ServerBuilder sb;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();

  sb.AddListeningPort(
      server_address,
    grpc::InsecureServerCredentials());

  std::unique_ptr<grpc::Server> server(sb.BuildAndStart());

  server->Wait();

}

