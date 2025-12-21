#ifndef AGENT_RESOURCES_H
#define AGENT_RESOURCES_H

#include <stddef.h>
#include <stdint.h>

#define AGENT_RESOURCES_API_VERSION 1

#define AGENT_OK 0
#define AGENT_ERR_IO 1
#define AGENT_ERR_PARSE 2
#define AGENT_ERR_PERM 3
#define AGENT_ERR_INVALID 4

/**
 * These structs are intentionally incomplete here.
 * Their definitions live in the .c file.
 *
 */

typedef struct AgentCpu AgentCpu;
typedef struct AgentRam AgentRam;
typedef struct AgentDisk AgentDisk;
typedef struct AgentDevice AgentDevice;

typedef struct Proces Proces;

AgentCpu *agent_cpu_create(void);
AgentRam *agent_ram_create(void);
AgentDisk *agent_disk_create(void);
AgentDevice *agent_device_create(void);

void agent_cpu_destroy(struct AgentCpu *cpu);
void agent_ram_destroy(struct AgentRam *ram);
void agent_disk_destroy(struct AgentDisk *disk);
void agent_device_destroy(struct AgentDevice *device);

int agent_cpu_read(struct AgentCpu *cpu);
int agent_ram_read(struct AgentRam *ram);
int agent_disk_read(struct AgentDisk *disk);
int agent_device_read(struct AgentDevice *device);

int agent_process_kill(int pid, int signal);

char *agent_cpu_get_vendor(AgentCpu *a);
char *agent_cpu_get_model(AgentCpu *a);
char *agent_cpu_get_frequency(AgentCpu *a);
char *agent_cpu_get_cores(AgentCpu *a);

char *agent_ram_get_total(AgentRam *a);
char *agent_ram_get_free(AgentRam *a);

size_t agent_disk_get_count(AgentDisk *a);
char *agent_disk_get_partitions(AgentDisk *a);

char *agent_device_get_os_version(AgentDevice *d);
char *agent_device_get_uptime(AgentDevice *d);
char **agent_device_get_procs(AgentDevice *d);

size_t agent_device_get_procs_count(AgentDevice *d);

#endif
