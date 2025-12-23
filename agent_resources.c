/*
 * name: Abdellah El Morabit
 * organization: Paradigm-Ehb
 * year: 2025-2026
 * description: resources gathering library
 *
 */

#define _POSIX_C_SOURCE 200809L

#include "agent_resources.h"

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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

/**
 * Mem sizes for Arena Allocation
 * */
#define KiB(n) ((u64)(n) << 10)
#define MiB(n) ((u64)(n) << 20)
#define GiB(n) ((u64)(n) << 30)

/**
 * Error macro's
 *
 * */
#define OK 0
#define ERR_IO 1
#define ERR_PARSE 2
#define ERR_PERM 3
#define ERR_INVALID 4

/**
 * Helper macro's
 *
 * */

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define ALIGN_UP_POW2(n, p) (((u64)(n) + ((u64)(p) - 1)) & (~((u64)(p) - 1)))

enum
{
    BUFFER_SIZE_SMALL = 128,
    BUFFER_SIZE_DEFAULT = 256,
    BUFFER_SIZE_LARGE = 512,
    PATH_MAX_LEN = 4096
};

/*
 * Represents a disk partition with major/minor device numbers and block count.
 */
typedef struct
{
    u64 major;
    u64 minor;
    u64 blocks;
    char* name;
} Partition;

/*
 * CPU information structure containing vendor, model, frequency, and core
 * count.
 */
struct Cpu
{
    char vendor[BUFFER_SIZE_DEFAULT];
    char model[BUFFER_SIZE_DEFAULT];
    char frequency[BUFFER_SIZE_SMALL];
    char cores[BUFFER_SIZE_SMALL];
};

/*
 * RAM information structure containing total and free memory.
 */
struct Ram
{
    char total[BUFFER_SIZE_DEFAULT];
    char free[BUFFER_SIZE_DEFAULT];
};

/*
 * Dynamic array of disk partitions with capacity tracking.
 */
struct Disk
{
    Partition* parts;
    size_t count;
    size_t cap;
};

/*
 * Device information including OS version, uptime, and running processes.
 */
struct Device
{
    char os_version[BUFFER_SIZE_DEFAULT];
    char uptime[BUFFER_SIZE_DEFAULT];
    char** procs;
    size_t procs_count;
};

struct Proces
{
    char* pid;
    char* command;
    char* state;
    char* ppid; // parent process id
    char* pgrp; // process group id
    char* session;
    char* tty_nr;
    char* tpgid;
    char* utime; // user cpu time
    char* num_threads; // thread count
};

/**
 * replacing malloc/free with arena allocaters
 *
 * */

#define ARENA_BASE_POS (sizeof(mem_arena))
// void * for the size of a pointer on the machine, 64/32bit comp
#define ARENA_ALIGN (sizeof(void*))

typedef struct
{
    u64 capacity;
    u64 pos;

} mem_arena;

// arena prototypes
mem_arena*
arena_create(u64 capacity);
// make it a void pointer to allow implicit conversion
void
arena_destory(mem_arena* arena);
void*
arena_push(mem_arena* arena, u64 size, b32 non_zero);
void
arena_pop(mem_arena* arena, u64 size);
void
arena_pop_to(mem_arena* arena, u64 pos);
void
arena_clear(mem_arena* arena);

mem_arena*
arena_create(u64 capacity)
{
    mem_arena* arena = (mem_arena*)malloc(capacity);
    arena->capacity = capacity;
    arena->pos = ARENA_BASE_POS;

    return arena;
}

// make it a void pointer to allow implicit conversion
void
arena_destory(mem_arena* arena)
{
    free(arena);
}
void*
arena_push(mem_arena* arena, u64 size, b32 non_zero)
{
    u64 pos_aligned = ALIGN_UP_POW2(arena->pos, ARENA_ALIGN);
    u64 new_pos = pos_aligned + size;

    if (new_pos > arena->capacity)
    {
        assert(0);
        return NULL;
    }

    arena->pos = new_pos;
    // cast to u8 to be able to do pointer arithemtic
    u8* out = (u8*)arena + pos_aligned;

    if (!non_zero)
    {
        memset(out, 0, size);
    }
    return out;
}
void
arena_pop(mem_arena* arena, u64 size)
{
    size = MIN(size, arena->pos - ARENA_BASE_POS);
    arena->pos -= size;
}

void
arena_pop_to(mem_arena* arena, u64 pos)
{
    u64 size = pos < arena->pos ? arena->pos - pos : 0;
    arena_pop(arena, size);
}

void
arena_clear(mem_arena* arena)
{
    arena_pop_to(arena, ARENA_BASE_POS);
}

#define PUSH_STRUCT(arena, T) (T*)arena_push((arena), sizeof(T), false)
#define PUSH_STRUCT_NZ(arena, T) (T*)arena_push((arena), sizeof(T), true)
#define PUSH_ARRAY(arena, T, n) (T*)arena_push((arena), sizeof(T) * (n), false)
#define PUSH_ARRAY_NZ(arena, T, n) \
    (T*)arena_push((arena), sizeof(T) * (n), true)

/**
 * Getters for data models to retreive in CGO
 *
 * */
char*
cpu_get_vendor(Cpu* a)
{
    return a->vendor;
}
char*
cpu_get_model(Cpu* a)
{
    return a->model;
}
char*
cpu_get_frequency(Cpu* a)
{
    return a->frequency;
}
char*
cpu_get_cores(Cpu* a)
{
    return a->cores;
}

char*
ram_get_total(Ram* a)
{
    return a->total;
}
char*
ram_get_free(Ram* a)
{
    return a->free;
}

char*
disk_get_partitions(Disk* a)
{
    return a->parts->name;
}
size_t
disk_get_count(Disk* a)
{
    return a->count;
}

u64
partition_get_major(Partition* p)
{
    return p->major;
}
u64
partition_get_minor(Partition* p)
{
    return p->minor;
}
u64
partition_get_blocks(Partition* p)
{
    return p->blocks;
}
char*
partition_get_name(Partition* p)
{
    return p->name;
}

char*
device_get_os_version(Device* d)
{
    return d->os_version;
}
char*
device_get_uptime(Device* d)
{
    return d->uptime;
}
// TODO(nasr): fix the return type
// @return returns a list of processes
char**
device_get_procs(Device* d)
{
    return d->procs;
}
size_t
device_get_procs_count(Device* d)
{
    return d->procs_count;
}

/*
 * disk_push_partition - Add a partition to the disk structure
 * @d: Pointer to the Disk structure
 * @p: Partition to add
 *
 * Dynamically grows the partition array if needed. Doubles capacity when
 * full. If realloc fails, the partition is not added and function returns
 * silently.
 */
void
disk_push_partition(Disk* d, Partition p)
{
    if (d->count == d->cap)
    {
        size_t ncap = d->cap ? d->cap * 2 : 8;
        Partition* np = realloc(d->parts, ncap * sizeof(*np));
        if (!np)
        {
            return;
        }
        d->parts = np;
        d->cap = ncap;
    }
    d->parts[d->count++] = p;
}

/*
 * is_numeric - Check if a string contains only digits
 * @s: String to check
 *
 * Return: 1 if string contains only numeric characters, 0 otherwise
 */
int
is_numeric(const char* s)
{
    for (; *s; ++s)
    {
        if (*s < '0' || *s > '9')
        {
            return 0;
        }
    }
    return 1;
}

/*
 * cpu_create - Allocate and initialize a new Cpu structure
 *
 * Return: Pointer to newly allocated Cpu, or NULL on allocation failure
 */
Cpu*
cpu_create(void)
{
    return calloc(1, sizeof(Cpu));
}

/*
 * cpu_read - Read CPU information from /proc/cpuinfo
 * @out: Pointer to Cpu structure to populate
 *
 * Reads vendor_id, model name, cpu MHz, and cpu cores from /proc/cpuinfo.
 * The function reads the first occurrence of each field.
 *
 * Return: OK on success, AGENT_ERR_INVALID if out is NULL,
 *         ERR_IO if /proc/cpuinfo cannot be opened
 */

/**
 * get a read of the enabled cpu cores to get a view of the folder structure
 * before starting to read every single of of them
 *
 * call
 *
 * */
int
cpu_read_enabled_core_cpu_frequency(Cpu* out, int enabled_cpu_count)
{
    if (!out)
    {
        return ERR_INVALID;
    }

    char curr_freq[BUFFER_SIZE_SMALL];
    char path[PATH_MAX_LEN];

    snprintf(path, sizeof(path), "/sys/devices/system/cpu/cpu%d/cpuinfo_cur_freq",
        enabled_cpu_count);

    FILE* fp = fopen(path, "r");
    if (!fgets(curr_freq, sizeof(curr_freq), fp))
    {
        return ERR_PARSE;
    }

    size_t len = sizeof(curr_freq) / sizeof(char);
    memcpy(out->frequency, curr_freq, len);
    out->frequency[len] = '\0';
    // AGAIN!!! DONT FORGET TO NULL TERMINATE STRINGS

    fclose(fp);

    return OK;
}
// TODO(nasr): i was doing something with directories i forgot what

// TODO(nasr): read the binary /proc/device-tree/model
// it contains the device model + cpu model
int
cpu_read_cpu_model_name_arm64(Cpu* out)
{
    int of = open("/proc/device-tree/model", O_RDONLY);

    u8 buffer[BUFFER_SIZE_DEFAULT];
    size_t s = sizeof(buffer);
    ssize_t rf = read(of, buffer, s);

    printf("%s", out->frequency);

    return OK;
}

int
cpu_get_enabled_cores_arm64(void)
{
    FILE* fp = fopen("/sys/devices/system/cpu/enabled", "r");
    if (!fp)
    {
        return -1;
    }

    char buffer[BUFFER_SIZE_DEFAULT];
    if (!fgets(buffer, sizeof(buffer), fp))
    {
        fclose(fp);
        return -1;
    }
    fclose(fp);

    int max_core = 0;
    int dash = 0;

    for (size_t i = 0; buffer[i] != '\0'; ++i)
    {
        char c = buffer[i];

        if (c == '-')
        {
            dash = 1;
            continue;
        }

        if (!dash)
        {
            continue;
        }

        if (c < '0' || c > '9')
        {
            break;
        }

        max_core = max_core * 10 + (c - '0');
    }

    return max_core + 1;
}

int
cpu_read_arm64(Cpu* out)
{
    if (!out)
    {
        return ERR_INVALID;
    }

    // TODO(nasr): gather all information arm
    return OK;
}

int
cpu_read_amd64(Cpu* out)
{
    if (!out)
    {
        return ERR_INVALID;
    }

    FILE* f = fopen("/proc/cpuinfo", "r");
    if (!f)
        return ERR_IO;

    char buf[BUFFER_SIZE_LARGE];
    while (fgets(buf, sizeof(buf), f))
    {
        char* colon = strchr(buf, ':');
        if (!colon)
            continue;

        char* val = colon + 1;
        while (*val == ' ')
            val++;

        size_t len = strcspn(val, "\n");

        if (!strncmp(buf, "vendor_id", 9))
        {
            memcpy(out->vendor, val, len);
        }
        else if (!strncmp(buf, "model name", 10))
        {
            memcpy(out->model, val, len);
        }
        else if (!strncmp(buf, "cpu MHz", 7))
        {
            memcpy(out->frequency, val, len);
        }
        else if (!strncmp(buf, "cpu cores", 9))
        {
            memcpy(out->cores, val, len);
        }
    }

    fclose(f);
    return OK;
}

int
cpu_read(Cpu* out)
{
    // TODO(nasr): C macro check for architecture and call appropriate function
    printf("%s", out->model);

    return OK;
}

/*
 * cpu_destroy - Free an Cpu structure
 * @c: Pointer to Cpu to free
 */
void
cpu_destroy(Cpu* c)
{
    free(c);
}

/*
 * ram_create - Allocate and initialize a new Ram structure
 *
 * Return: Pointer to newly allocated Ram, or NULL on allocation failure
 */
Ram*
ram_create(void)
{
    return calloc(1, sizeof(Ram));
}

/*
 * ram_read - Read RAM information from /proc/meminfo
 * @out: Pointer to Ram structure to populate
 *
 * Reads MemTotal and MemFree from /proc/meminfo in kilobytes.
 *
 * Return: OK on success, AGENT_ERR_INVALID if out is NULL,
 *         ERR_IO if /proc/meminfo cannot be opened
 */
int
ram_read(Ram* out)
{
    if (!out)
        return ERR_INVALID;

    FILE* f = fopen("/proc/meminfo", "r");
    if (!f)
        return ERR_IO;

    char buf[BUFFER_SIZE_LARGE];
    while (fgets(buf, sizeof(buf), f))
    {
        char* colon = strchr(buf, ':');
        if (!colon)
        {
            continue;
        }

        char* val = colon + 1;
        while (*val == ' ')
        {
            val++;
        }

        size_t len = strcspn(val, " k\n");

        if (!strncmp(buf, "MemTotal", 8))
        {
            memcpy(out->total, val, len);
        }
        else if (!strncmp(buf, "MemFree", 7))
        {
            memcpy(out->free, val, len);
        }
    }

    fclose(f);
    return OK;
}

/*
 * ram_destroy - Free an Ram structure
 * @r: Pointer to Ram to free
 */
void
ram_destroy(Ram* r)
{
    free(r);
}

/*
 * disk_create - Allocate and initialize a new Disk structure
 *
 * Return: Pointer to newly allocated Disk, or NULL on allocation failure
 */
Disk*
disk_create(void)
{
    return calloc(1, sizeof(Disk));
}

/*
 * disk_read - Read disk partition information from /proc/partitions
 * @out: Pointer to Disk structure to populate
 *
 * Reads all partitions from /proc/partitions, storing major/minor device
 * numbers, block count, and device name for each partition. Skips the header
 * line and any malformed entries.
 *
 * Return: OK on success, AGENT_ERR_INVALID if out is NULL,
 *         ERR_IO if /proc/partitions cannot be opened
 */
int
disk_read(Disk* out)
{
    if (!out)
    {
        return ERR_INVALID;
    }

    FILE* f = fopen("/proc/partitions", "r");
    if (!f)
    {
        return ERR_IO;
    }

    char buf[BUFFER_SIZE_DEFAULT];
    fgets(buf, sizeof(buf), f);

    while (fgets(buf, sizeof(buf), f))
    {
        Partition p = { 0 };
        char name[BUFFER_SIZE_DEFAULT];

        if (sscanf(buf, "%lu %lu %lu %255s", &p.major, &p.minor, &p.blocks, name) !=
            4)
            continue;

        p.name = strdup(name);
        if (!p.name)
            continue;

        disk_push_partition(out, p);
    }

    fclose(f);
    return OK;
}

/*
 * disk_destroy - Free an Disk structure and all partition data
 * @d: Pointer to Disk to free
 *
 * Frees all dynamically allocated partition names and the partition array.
 */
void
disk_destroy(Disk* d)
{
    if (!d)
        return;
    for (size_t i = 0; i < d->count; ++i)
        free(d->parts[i].name);
    free(d->parts);
    free(d);
}

/*
 * device_create - Allocate and initialize a new Device structure
 *
 * Return: Pointer to newly allocated Device, or NULL on allocation failure
 */
Device*
device_create(void)
{
    return calloc(1, sizeof(Device));
}

/*
 * collect_processes - Collect all running process IDs from /proc
 * @dev: Pointer to Device structure to populate with process IDs
 *
 * Scans /proc directory for numeric entries (process IDs) and stores them
 * as strings in the Device structure. Dynamically grows the process array
 * as needed.
 */
void
collect_processes(Device* dev)
{
    DIR* d = opendir("/proc");
    if (!d)
        return;

    struct dirent* e;
    size_t cap = 8;
    dev->procs = malloc(sizeof(char*) * cap);

    while ((e = readdir(d)))
    {
        if (!is_numeric(e->d_name))
            continue;

        if (dev->procs_count == cap)
        {
            cap *= 2;
            char** np = realloc(dev->procs, sizeof(char*) * cap);
            if (!np)
                break;
            dev->procs = np;
        }

        dev->procs[dev->procs_count++] = strdup(e->d_name);
    }

    closedir(d);
}

int
collect_processes_stats(char* pid, Proces* out)
{
    FILE* fp = fopen(pid, "r");

    char buf[BUFFER_SIZE_SMALL];
    fgets(buf, sizeof(buf), fp);

    if (!fgets(buf, sizeof(buf), fp))
        return ERR_PARSE;

    // TODO(nasr): parse buffer to correct stuff
    // TODO(nasr): fix the length
    memcpy(out->pid, buf, BUFFER_SIZE_SMALL);

    fclose(fp);

    return OK;
}

/*
 * device_read - Read device information including OS version, uptime, and
 * processes
 * @out: Pointer to Device structure to populate
 *
 * Reads OS version from /proc/version, uptime from /proc/uptime, and collects
 * all running process IDs from /proc directory.
 *
 * Return: OK on success, AGENT_ERR_INVALID if out is NULL,
 *         ERR_IO if required files cannot be opened
 */
int
device_read(Device* out)
{
    if (!out)
    {
        return ERR_INVALID;
    }

    FILE* u = fopen("/proc/uptime", "r");
    FILE* v = fopen("/proc/version", "r");
    if (!u || !v)
    {
        if (u)
        {
            fclose(u);
        }
        if (v)
        {
            fclose(v);
        }
        return ERR_IO;
    }

    fgets(out->uptime, sizeof(out->uptime), u);
    fgets(out->os_version, sizeof(out->os_version), v);

    fclose(u);
    fclose(v);

    collect_processes(out);
    return OK;
}

/*
 * device_destroy - Free an Device structure and all process data
 * @d: Pointer to Device to free
 *
 * Frees all dynamically allocated process ID strings and the process array.
 */
void
device_destroy(Device* d)
{
    if (!d)
        return;
    for (size_t i = 0; i < d->procs_count; ++i)
        free(d->procs[i]);
    free(d->procs);
    free(d);
}

/*
 * process_kill - Send a signal to a process
 * @pid: Process ID to signal
 * @signal: Signal number to send (e.g., SIGTERM, SIGKILL)
 *
 * Wrapper around kill(2) system call with error handling.
 *
 * Return: OK on success,
 *         ERR_INVALID if pid is invalid or process not found,
 *         ERR_PERM if permission denied,
 *         ERR_IO for other errors
 */
int
process_kill(pid_t pid, int signal)
{
    if (pid <= 0)
    {
        return ERR_INVALID;
    }

    if (kill(pid, signal) == -1)
    {
        if (errno == EPERM)
        {
            return ERR_PERM;
        }
        if (errno == ESRCH)
        {
            return ERR_INVALID;
        }

        return ERR_IO;
    }
    return OK;
}
