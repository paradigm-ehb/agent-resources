#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <memory>

#include "grpc_server/server.h"
#include "base/base.h"

#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

local_internal void
grpc_server_intercept_logger(void)
{
  /*
    TODO(nasr): every grpc call contains meta data we can inspect
    based on this we can give better responses
   */

}

/*

  NOTE(nasr): think about
 */


/*
 * grpc server setup
 */
local_internal void
grpc_server_setup(void)
{

  /*
    TODO(nasr): provide a way for defining this?
   */

  const char *server_address = "0.0.0.0:5000";
  grpc::ServerBuilder sb;

  /*
    enabled for first initial check to see if a server is online or not
   */
  grpc::EnableDefaultHealthCheckService(true);

  grpc::reflection::InitProtoReflectionServerBuilderPlugin();

  sb.AddListeningPort(
      server_address,
    grpc::InsecureServerCredentials());

  std::unique_ptr<grpc::Server> server(sb.BuildAndStart());

  server->Wait();

}
