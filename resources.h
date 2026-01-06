#ifndef RESOURCES_H
#define RESOURCES_H

#include <stddef.h>
#include <stdint.h>
#include "base.h"
#include "arena.h"

#define RESOURCES_API_VERSION 1

typedef struct Cpu Cpu;
typedef struct Ram Ram;
typedef struct Disk Disk;
typedef struct Device Device;

typedef struct Partition Partition;
typedef struct Process Process;
typedef struct Process_List Process_List;

typedef enum Process_State {
  PROCESS_UNDEFINED = 'N',
  PROCESS_RUNNING = 'R',
  PROCESS_SLEEPING = 'S',
  PROCESS_DISK_SLEEP = 'D',
  PROCESS_STOPPED = 'T',
  PROCESS_TRACING_STOPPED = 't',
  PROCESS_ZOMBIE = 'Z',
  PROCESS_DEAD = 'X',
} Process_State;

struct Process {
  i32 pid;
  Process_State state;

  u64 utime;
  u64 stime;

  u32 num_threads;

  char name[BUFFER_SIZE_SMALL];
};

struct Process_List {
  Process *items;
  size_t count;
  size_t capacity;
};

struct Partition {
  u64 major;
  u64 minor;
  u64 blocks;
  char name[BUFFER_SIZE_SMALL];
};

struct Cpu {
  char vendor[BUFFER_SIZE_DEFAULT];
  char model[BUFFER_SIZE_DEFAULT];
  char frequency[BUFFER_SIZE_SMALL];
  u32 cores;
};

struct Ram {
  char total[BUFFER_SIZE_DEFAULT];
  char free[BUFFER_SIZE_DEFAULT];
};

struct Disk {
  Partition *partitions;
  size_t count;
  size_t capacity;
};

struct Device {
  char os_version[BUFFER_SIZE_DEFAULT];
  char uptime[BUFFER_SIZE_DEFAULT];
  Process_List processes;
};

mem_arena *arena_create(u64 capacity);
/**
 * TODO(nasr): add error handling for both the destroy and the clear
 * */
void arena_destroy(mem_arena *arena);
void arena_clear(mem_arena *arena);

Cpu *cpu_create(mem_arena *arena);
Ram *ram_create(mem_arena *arena);
Disk *disk_create(mem_arena *arena);
Device *device_create(mem_arena *arena);

int cpu_read(Cpu *cpu);
int ram_read(Ram *ram);
int disk_read(Disk *disk, mem_arena *arena);
int device_read(Device *device);

// TODO(nasr): add a function that updates certain values incrementally
// instead of neading to update the entire cpu struct

int process_list_collect(Process_List *list, mem_arena *arena);

int process_read(i32 pid, Process *out);

int process_kill(i32 pid, i32 signal);

#endif /* RESOURCES_H */
