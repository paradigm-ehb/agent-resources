#ifndef GRPC_H
#define GRPC_H

#include <grpcpp/server_context.h>
#include <grpcpp/support/status.h>

#include "grpc/generated/resources/v3/deviceresources.pb.h"
#include "base/base.h"


local_internal void
grpc_server_setup(void);


local_internal grpc::Status
get_system_resources_impl(
  grpc::ServerContext                            *ctx,
  const resources::v3::GetSystemResourcesRequest *req,
  resources::v3::GetSystemResourcesReply         *resp
);

local_internal grpc::Status
get_cpu_impl(
  grpc::ServerContext                  *ctx,
  const resources::v3::GetCpuRequest   *req,
  resources::v3::GetCpuReply           *reply
);

local_internal grpc::Status
get_ram_impl(
  grpc::ServerContext                      *ctx,
  const resources::v3::GetMemoryRequest   *req,
  resources::v3::GetMemoryReply           *reply
);

#endif /* GRPC_H */
