#include <grpcpp/server_context.h>
#include <grpcpp/support/status.h>

#include "libres/resources.h"
#include "grpc/generated/resources/v3/deviceresources.pb.h"
#include "grpc/grpc_include.h"

#include "base/base.h"

using resources::v3::GetSystemResourcesReply;
using resources::v3::GetSystemResourcesRequest;
using resources::v3::SystemResources;

local_internal grpc::Status
               get_system_resources_impl(grpc::ServerContext *ctx,
               const GetSystemResourcesRequest               *req,
               GetSystemResourcesReply                       *resp)
{
    /* NOTE(nasr): for the general get_resources we don't take in any parameters*/
    (void)req;

    if (ctx->IsCancelled())
    {
        return grpc::Status(
        grpc::CANCELLED,
        "request cancelled");
    }
    std::chrono::system_clock::time_point deadline = ctx->deadline();

    if (deadline < std::chrono::system_clock::now())
    {
        return grpc::Status(
        grpc::StatusCode::DEADLINE_EXCEEDED,
        "deadline exceeded");
    }

    mem_arena *temp_arena = arena_create(MiB(8));

    Cpu    *internal_cpu    = cpu_create(temp_arena);
    Ram    *internal_ram    = ram_create(temp_arena);
    Disk   *internal_disk   = disk_create(temp_arena);
    Device *internal_device = device_create(temp_arena);

    /*
   * TODO(nasr): find a way to cleanup this deadline mess
   * */
    cpu_read(internal_cpu);

    if (deadline < std::chrono::system_clock::now())
    {
        return grpc::Status(
        grpc::StatusCode::DEADLINE_EXCEEDED,
        "deadline exceeded");
    }

    ram_read(internal_ram);

    if (deadline < std::chrono::system_clock::now())
    {
        return grpc::Status(
        grpc::StatusCode::DEADLINE_EXCEEDED,
        "deadline exceeded");
    }

    disk_read(internal_disk, temp_arena);

    if (deadline < std::chrono::system_clock::now())
    {
        return grpc::Status(
        grpc::StatusCode::DEADLINE_EXCEEDED,
        "deadline exceeded");
    }

    device_read(internal_device);

    if (deadline < std::chrono::system_clock::now())
    {
        return grpc::Status(
        grpc::StatusCode::DEADLINE_EXCEEDED,
        "deadline exceeded");
    }

    SystemResources *out = resp->mutable_resources();

    /*
   * NOTE(nasr): don't set the directive fo the individual resources
   * they could conflict with the internal ones, just to avoid mistakes
   * */
    resources::v3::Cpu *cpu_msg = out->mutable_cpu();

    cpu_msg->set_vendor(internal_cpu->vendor);
    cpu_msg->set_model(internal_cpu->model);
    cpu_msg->set_frequency(internal_cpu->frequency);
    cpu_msg->set_total_time(internal_cpu->total_time);
    cpu_msg->set_idle_time(internal_cpu->idle_time);
    cpu_msg->set_cores(internal_cpu->cores);

    resources::v3::Memory *ram_msg = out->mutable_memory();

    ram_msg->set_total(internal_ram->total);
    ram_msg->set_free(internal_ram->free);

    /*
   * TODO(nasr): doing the disk later because it's tricky to handle with all of those partitions
   * maybe we should split disk and partitions up
   * and make the disk only handle the usage etc.
   * and partitions do what they already do
   * without being linked to disk
   * */

    /*
   * TODO(nasr):  Same story for devices and process, theyre currently linked to each other and that doesn't really surve any purpose besides the fact that it seemed "logical"
   * at the time of writing that code
   * */

    arena_destroy(temp_arena);

    return grpc::Status::OK;
}

local_internal grpc::Status
               get_cpu_impl(
               grpc::ServerContext                *ctx,
               const resources::v3::GetCpuRequest *req,
               resources::v3::GetCpuReply         *resp)
{
    (void)req;

    if (ctx->IsCancelled())
    {
        return grpc::Status(
        grpc::CANCELLED,
        "request cancelled");
    }
    std::chrono::system_clock::time_point deadline = ctx->deadline();

    if (deadline < std::chrono::system_clock::now())
    {
        return grpc::Status(
        grpc::StatusCode::DEADLINE_EXCEEDED,
        "deadline exceeded");
    }

    mem_arena *temp_arena = arena_create(MiB(8));

    Cpu *internal_cpu = cpu_create(temp_arena);

    /*
   * TODO(nasr): find a way to cleanup this deadline mess
   * */
    cpu_read(internal_cpu);

    resources::v3::Cpu *cpu_msg = resp->mutable_cpu();

    /*
   * NOTE(nasr): don't set the directive fo the individual resources
   * they could conflict with the internal ones, just to avoid mistakes
   * */

    cpu_msg->set_vendor(internal_cpu->vendor);
    cpu_msg->set_model(internal_cpu->model);
    cpu_msg->set_frequency(internal_cpu->frequency);
    cpu_msg->set_total_time(internal_cpu->total_time);
    cpu_msg->set_idle_time(internal_cpu->idle_time);
    cpu_msg->set_cores(internal_cpu->cores);

    arena_destroy(temp_arena);

    return grpc::Status::OK;
}

local_internal grpc::Status
               get_ram_impl(grpc::ServerContext      *ctx,
               const resources::v3::GetMemoryRequest *req,
               resources::v3::GetMemoryReply         *resp)
{
    (void)req;

    if (ctx->IsCancelled())
    {
        return grpc::Status(
        grpc::CANCELLED,
        "request cancelled");
    }
    std::chrono::system_clock::time_point deadline = ctx->deadline();

    if (deadline < std::chrono::system_clock::now())
    {
        return grpc::Status(
        grpc::StatusCode::DEADLINE_EXCEEDED,
        "deadline exceeded");
    }

    mem_arena *temp_arena = arena_create(MiB(8));

    Ram *internal_ram = ram_create(temp_arena);

    /*
   * TODO(nasr): find a way to cleanup this deadline mess
   * */
    ram_read(internal_ram);

    resources::v3::Memory *ram_msg = resp->mutable_memory();

    /*
   * NOTE(nasr): don't set the directive fo the individual resources
   * they could conflict with the internal ones, just to avoid mistakes
   * */

    ram_msg->set_total(internal_ram->total);
    ram_msg->set_free(internal_ram->free);

    arena_destroy(temp_arena);

    return grpc::Status::OK;
}

local_internal grpc::Status
               get_device_impl(grpc::ServerContext      *ctx,
               const resources::v3::GetDeviceRequest *req,
               resources::v3::GetDeviceReply         *resp)
{


}
