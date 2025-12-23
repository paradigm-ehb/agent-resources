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

/**
 * These structs are intentionally incomplete here.
 * Their definitions live in the .c file.
 *
 */

typedef struct Cpu Cpu;
typedef struct Ram Ram;
typedef struct Disk Disk;
typedef struct Device Device;

typedef struct Proces Proces;

Cpu *cpu_create(void);
Ram *ram_create(void);
Disk *disk_create(void);
Device *device_create(void);

void cpu_destroy(struct Cpu *cpu);
void ram_destroy(struct Ram *ram);
void disk_destroy(struct Disk *disk);
void device_destroy(struct Device *device);

int cpu_read(struct Cpu *cpu);
int ram_read(struct Ram *ram);
int disk_read(struct Disk *disk);
int device_read(struct Device *device);

int process_kill(int pid, int signal);

char *cpu_get_vendor(Cpu *a);
char *cpu_get_model(Cpu *a);
char *cpu_get_frequency(Cpu *a);
char *cpu_get_cores(Cpu *a);

char *ram_get_total(Ram *a);
char *ram_get_free(Ram *a);

size_t disk_get_count(Disk *a);
char *disk_get_partitions(Disk *a);

char *device_get_os_version(Device *d);
char *device_get_uptime(Device *d);
char **device_get_procs(Device *d);

size_t device_get_procs_count(Device *d);

#endif
