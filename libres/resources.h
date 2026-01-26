#ifndef RESOURCES_H
#define RESOURCES_H
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "base/base.h"
#include "base/base_arena.h"

#define RESOURCES_API_VERSION 1

typedef struct sys_cpu          cpu;
typedef struct sys_memory       memory;
typedef struct sys_disk         disk;
typedef struct sys_device       device;
typedef struct sys_filesystem   fs;
typedef struct sys_partition    partition;
typedef struct sys_process      proc;
typedef struct sys_process_list proc_list;

typedef enum
{
    proc_undefined       = 0,
    proc_running         = 1,
    proc_sleeping        = 2,
    proc_disk_sleeping   = 3,
    proc_stopped         = 4,
    proc_tracing_stopped = 5,
    proc_zombie          = 6,
    proc_dead            = 7,
    proc_idle            = 8,

} sys_proc_state;

struct sys_process
{
    char          name[BUFFER_SIZE_SMALL];
    u64           utime;
    u64           stime;
    i32           pid;
    i8           num_threads;
    sys_proc_state state;
};

struct sys_process_list
{
    proc *items;
    umm      count;
    umm      capacity;
};

struct sys_partition
{
    u64  major;
    u64  minor;
    u64  blocks;
    char name[BUFFER_SIZE_SMALL];
};

struct sys_cpu
{
    u64  total_time;
    u64  idle_time;
    u32  cores;
    char vendor[BUFFER_SIZE_DEFAULT];
    char model[BUFFER_SIZE_DEFAULT];
    char frequency[BUFFER_SIZE_SMALL];
};

struct sys_memory
{
    u64 total;
    u64 free;
};

struct sys_filesystem
{
    u64 total;
    u64 free;
    u64 available;
    u64 used;
};

struct sys_disk
{
    partition *partitions;

    size_t part_count;
    size_t part_capacity;
};

enum TransportLayerType
{
    TRANSPORT_TCP,
    TRANSPORT_UDP,
    TRANSPORT_SCTP,
    /**
   * NOTE(nasr): don't forget to account for SCTP
   * */
};

struct Port
{
    /**
   * Get the name by calling
   * what service is listening to port xx
   *
   * */
    char name[BUFFER_SIZE_SMALL];

    int                     port;
    enum TransportLayerType type;
    char                    alias[BUFFER_SIZE_DEFAULT];
    char                    comment[BUFFER_SIZE_DEFAULT];
};

struct sys_device
{
    char         os_version[BUFFER_SIZE_DEFAULT];
    char         uptime[BUFFER_SIZE_DEFAULT];
    proc_list processes;
    struct Port *port;
};

local_internal cpu *
cpu_create(global_arena *arena);

local_internal int
cpu_read(cpu *out);

local_internal int
cpu_read_arm64(cpu *out);

local_internal int
cpu_read_amd64(cpu *out);

local_internal int
cpu_read_usage(cpu *out);

local_internal int
cpu_get_cores_enabled_arm(cpu *out);

local_internal int
cpu_read_cpu_model_name_arm64(cpu *out);

local_internal int
cpu_read_enabled_core_cpu_frequency(cpu *out, int enabled_cpu_count);

local_internal memory *
ram_create(global_arena *arena);

local_internal int
ram_read(memory *out);

local_internal disk *
disk_create(global_arena *arena);

local_internal int
disk_read(disk *out, global_arena *arena);

local_internal fs *
fs_create(global_arena *arena);

local_internal int
fs_read(char *path, fs *fs);

local_internal device *
device_create(global_arena *arena);

local_internal int
device_read(device *out);

local_internal int
device_up_time(device *out);

local_internal int
process_list_collect(proc_list *list, global_arena *arena);

local_internal int
process_read(int32_t pid, proc *out);

local_internal int
process_kill(pid_t pid, int signal);

#endif /* RESOURCES_H */
