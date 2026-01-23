
#include <memory>

#include "grpc/grpc_include.h"

#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>


#include "grpc/generated/resources/v3/deviceresources.grpc.pb.h"
#include "grpc/generated/resources/v3/deviceresources.pb.h"

#include "base/base.h"

class ResourcesServiceImpl final
  : public resources::v3::ResourcesService::Service
{
public:
  grpc::Status
  GetSystemResources(
    grpc::ServerContext                            *ctx,
    const resources::v3::GetSystemResourcesRequest *req,
    resources::v3::GetSystemResourcesReply         *reply
  ) override;

  grpc::Status
  GetCpu(
    grpc::ServerContext         *ctx,
    const resources::v3::GetCpuRequest *req,
    resources::v3::GetCpuReply         *reply
  ) override;

  grpc::Status
  GetMemory(
    grpc::ServerContext              *ctx,
    const resources::v3::GetMemoryRequest *req,
    resources::v3::GetMemoryReply         *reply
  ) override;
};

grpc::Status
ResourcesServiceImpl::GetSystemResources(
  grpc::ServerContext                            *ctx,
  const resources::v3::GetSystemResourcesRequest *req,
  resources::v3::GetSystemResourcesReply         *reply
)
{
  return get_system_resources_impl(ctx, req, reply);
}

grpc::Status
ResourcesServiceImpl::GetCpu(
  grpc::ServerContext                  *ctx,
  const resources::v3::GetCpuRequest   *req,
  resources::v3::GetCpuReply           *reply
)
{
  return get_cpu_impl(ctx, req, reply);
}

grpc::Status
ResourcesServiceImpl::GetMemory(
  grpc::ServerContext                      *ctx,
  const resources::v3::GetMemoryRequest   *req,
  resources::v3::GetMemoryReply           *reply
)
{
  return get_ram_impl(ctx, req, reply);
}


local_internal void
grpc_server_setup(void)
{
  ResourcesServiceImpl service;

  const char *server_address = "0.0.0.0:5000";
  grpc::ServerBuilder sb;

  /* Health + reflection (for grpcurl / debugging) */
  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();

  sb.AddListeningPort(
    server_address,
    grpc::InsecureServerCredentials()
  );

  sb.RegisterService(&service);

  printf(
    "gRPC service registered: %s\n",
    resources::v3::ResourcesService::service_full_name()
  );

  std::unique_ptr<grpc::Server> server(sb.BuildAndStart());

  /* Blocks forever — expected for a server */
  server->Wait();
}
