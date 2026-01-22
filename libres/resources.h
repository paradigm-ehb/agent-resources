#ifndef RESOURCES_H
#define RESOURCES_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "base/base.h"
#include "base/base_arena.h"

#define RESOURCES_API_VERSION 1

typedef struct Cpu Cpu;
typedef struct Ram Ram;
typedef struct Disk Disk;
typedef struct Device Device;
typedef struct FileSystem FileSystem;

typedef struct Partition Partition;
typedef struct Process Process;
typedef struct Process_List Process_List;

/**
 * TODO(nasr):
 *
 * Heh? what are you doing with the types here,
 * you mismatched the namings
 * check for what you are doing in the cgo wrapper
 * this could be a big issue
 *
 * */
typedef int32_t ProcessState;

typedef enum Process_State
{
  PROCESS_UNDEFINED = 0,
  PROCESS_RUNNING = 1,
  PROCESS_SLEEPING = 2,
  PROCESS_DISK_SLEEP = 3,
  PROCESS_STOPPED = 4,
  PROCESS_TRACING_STOPPED = 5,
  PROCESS_ZOMBIE = 6,
  PROCESS_DEAD = 7,
  PROCESS_IDLE = 8,

} Process_State;

struct Process
{
  i32 pid;
  Process_State state;
  u64 utime;
  u64 stime;
  u32 num_threads;
  char name[BUFFER_SIZE_SMALL];
};

struct Process_List
{
  Process *items;
  size_t count;
  size_t capacity;
};

struct Partition
{
  u64 major;
  u64 minor;
  u64 blocks;
  char name[BUFFER_SIZE_SMALL];
};

struct Cpu
{
  char vendor[BUFFER_SIZE_DEFAULT];
  char model[BUFFER_SIZE_DEFAULT];
  char frequency[BUFFER_SIZE_SMALL];
  u64 total_time;
  u64 idle_time;
  u32 cores;
};

struct Ram
{
  u64 total;
  u64 free;
};

struct FileSystem
{
  u64 total;
  u64 free;
  u64 available;
  u64 used;
};

struct Disk
{
  Partition *partitions;

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

  int port;
  enum TransportLayerType type;
  char alias[BUFFER_SIZE_DEFAULT];
  char comment[BUFFER_SIZE_DEFAULT];
};

struct Device
{
  char os_version[BUFFER_SIZE_DEFAULT];
  char uptime[BUFFER_SIZE_DEFAULT];
  Process_List processes;
  /**
   *
   * */
  struct Port *port;
};

local_internal Cpu *
cpu_create(mem_arena *arena);

local_internal int
cpu_read(Cpu *out);

local_internal int
cpu_read_arm64(Cpu *out);

local_internal int
cpu_read_amd64(Cpu *out);

local_internal int
cpu_read_usage(Cpu *out);

local_internal int
cpu_get_cores_enabled_arm(Cpu *out);

local_internal int
cpu_read_cpu_model_name_arm64(Cpu *out);

local_internal int
cpu_read_enabled_core_cpu_frequency(Cpu *out, int enabled_cpu_count);

local_internal Ram *
ram_create(mem_arena *arena);

local_internal int
ram_read(Ram *out);

local_internal Disk *
disk_create(mem_arena *arena);

local_internal int
disk_read(Disk *out, mem_arena *arena);

local_internal FileSystem *
fs_create(mem_arena *arena);

local_internal int
fs_read(char *path, FileSystem *fs);

local_internal Device *
device_create(mem_arena *arena);

local_internal int
device_read(Device *out);

local_internal int
device_up_time(Device *out);

local_internal int
process_list_collect(Process_List *list, mem_arena *arena);

local_internal  int
process_read(int32_t pid, Process *out);

local_internal int
process_kill(pid_t pid, int signal);

#endif /* RESOURCES_H */
