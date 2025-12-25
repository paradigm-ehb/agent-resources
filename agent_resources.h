#ifndef RESOURCES_H
#define RESOURCES_H

#include <stddef.h>
#include <stdint.h>

#define RESOURCES_API_VERSION 1

#define OK 0
#define ERR_IO 1
#define ERR_PARSE 2
#define ERR_PERM 3
#define ERR_INVALID 4

enum {
  BUFFER_SIZE_SMALL = 128,
  BUFFER_SIZE_DEFAULT = 256,
  BUFFER_SIZE_LARGE = 512,
  PATH_MAX_LEN = 4096
};

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef i16 b16;
typedef i32 b32;

static inline u64 KiB(u64 n) { return n << 10; }
static inline u64 MiB(u64 n) { return n << 20; }
static inline u64 GiB(u64 n) { return n << 30; }

typedef struct mem_arena mem_arena;

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
  char cores[BUFFER_SIZE_SMALL];
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

int process_list_collect(Process_List *list, mem_arena *arena);

int process_read(i32 pid, Process *out);

int process_kill(i32 pid, i32 signal);

#endif /* RESOURCES_H */
