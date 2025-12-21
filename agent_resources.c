/*
 * name: Abdellah El Morabit
 * organization: Paradigm-Ehb
 * year: 2025-2026
 * description: resources gathering library
 *
 */

#define _POSIX_C_SOURCE 200809L

#include "agent_resources.h"

#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXC_CHAR 256

/*
 * Represents a disk partition with major/minor device numbers and block count.
 */
typedef struct {
  uint64_t major;
  uint64_t minor;
  uint64_t blocks;
  char *name;
} Partition;

/*
 * CPU information structure containing vendor, model, frequency, and core
 * count.
 */
typedef struct {
  char vendor[MAXC_CHAR];
  char model[MAXC_CHAR];
  char frequency[MAXC_CHAR];
  char cores[MAXC_CHAR];
} Cpu;

/*
 * RAM information structure containing total and free memory.
 */
typedef struct {
  char total[MAXC_CHAR];
  char free[MAXC_CHAR];
} Ram;

/*
 * Dynamic array of disk partitions with capacity tracking.
 */
typedef struct {
  Partition *parts;
  size_t count;
  size_t cap;
} Disk;

/*
 * Device information including OS version, uptime, and running processes.
 */
typedef struct {
  char os_version[MAXC_CHAR];
  char uptime[MAXC_CHAR];
  char **procs;
  size_t procs_count;
} Device;

struct AgentCpu {
  Cpu data;
};

struct AgentRam {
  Ram data;
};

struct AgentDisk {
  Disk data;
};

struct AgentDevice {
  Device data;
};

/*
 * disk_push_partition - Add a partition to the disk structure
 * @d: Pointer to the Disk structure
 * @p: Partition to add
 *
 * Dynamically grows the partition array if needed. Doubles capacity when full.
 * If realloc fails, the partition is not added and function returns silently.
 */
void disk_push_partition(Disk *d, Partition p) {
  if (d->count == d->cap) {
    size_t ncap = d->cap ? d->cap * 2 : 8;
    Partition *np = realloc(d->parts, ncap * sizeof(*np));
    if (!np)
      return;
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
int is_numeric(const char *s) {
  for (; *s; ++s)
    if (*s < '0' || *s > '9')
      return 0;
  return 1;
}

/*
 * agent_cpu_create - Allocate and initialize a new AgentCpu structure
 *
 * Return: Pointer to newly allocated AgentCpu, or NULL on allocation failure
 */
AgentCpu *agent_cpu_create(void) { return calloc(1, sizeof(AgentCpu)); }

/*
 * agent_cpu_read - Read CPU information from /proc/cpuinfo
 * @out: Pointer to AgentCpu structure to populate
 *
 * Reads vendor_id, model name, cpu MHz, and cpu cores from /proc/cpuinfo.
 * The function reads the first occurrence of each field.
 *
 * Return: AGENT_OK on success, AGENT_ERR_INVALID if out is NULL,
 *         AGENT_ERR_IO if /proc/cpuinfo cannot be opened
 */
int agent_cpu_read(AgentCpu *out) {
  if (!out)
    return AGENT_ERR_INVALID;

  FILE *f = fopen("/proc/cpuinfo", "r");
  if (!f)
    return AGENT_ERR_IO;

  char buf[MAXC_CHAR];
  while (fgets(buf, sizeof(buf), f)) {
    char *colon = strchr(buf, ':');
    if (!colon)
      continue;

    char *val = colon + 1;
    while (*val == ' ')
      val++;

    size_t len = strcspn(val, "\n");

    if (!strncmp(buf, "vendor_id", 9))
      memcpy(out->data.vendor, val, len);
    else if (!strncmp(buf, "model name", 10))
      memcpy(out->data.model, val, len);
    else if (!strncmp(buf, "cpu MHz", 7))
      memcpy(out->data.frequency, val, len);
    else if (!strncmp(buf, "cpu cores", 9))
      memcpy(out->data.cores, val, len);
  }

  fclose(f);
  return AGENT_OK;
}

/*
 * agent_cpu_destroy - Free an AgentCpu structure
 * @c: Pointer to AgentCpu to free
 */
void agent_cpu_destroy(AgentCpu *c) { free(c); }

/*
 * agent_ram_create - Allocate and initialize a new AgentRam structure
 *
 * Return: Pointer to newly allocated AgentRam, or NULL on allocation failure
 */
AgentRam *agent_ram_create(void) { return calloc(1, sizeof(AgentRam)); }

/*
 * agent_ram_read - Read RAM information from /proc/meminfo
 * @out: Pointer to AgentRam structure to populate
 *
 * Reads MemTotal and MemFree from /proc/meminfo in kilobytes.
 *
 * Return: AGENT_OK on success, AGENT_ERR_INVALID if out is NULL,
 *         AGENT_ERR_IO if /proc/meminfo cannot be opened
 */
int agent_ram_read(AgentRam *out) {
  if (!out)
    return AGENT_ERR_INVALID;

  FILE *f = fopen("/proc/meminfo", "r");
  if (!f)
    return AGENT_ERR_IO;

  char buf[MAXC_CHAR];
  while (fgets(buf, sizeof(buf), f)) {
    char *colon = strchr(buf, ':');
    if (!colon)
      continue;

    char *val = colon + 1;
    while (*val == ' ')
      val++;

    size_t len = strcspn(val, " k\n");

    if (!strncmp(buf, "MemTotal", 8))
      memcpy(out->data.total, val, len);
    else if (!strncmp(buf, "MemFree", 7))
      memcpy(out->data.free, val, len);
  }

  fclose(f);
  return AGENT_OK;
}

/*
 * agent_ram_destroy - Free an AgentRam structure
 * @r: Pointer to AgentRam to free
 */
void agent_ram_destroy(AgentRam *r) { free(r); }

/*
 * agent_disk_create - Allocate and initialize a new AgentDisk structure
 *
 * Return: Pointer to newly allocated AgentDisk, or NULL on allocation failure
 */
AgentDisk *agent_disk_create(void) { return calloc(1, sizeof(AgentDisk)); }

/*
 * agent_disk_read - Read disk partition information from /proc/partitions
 * @out: Pointer to AgentDisk structure to populate
 *
 * Reads all partitions from /proc/partitions, storing major/minor device
 * numbers, block count, and device name for each partition. Skips the header
 * line and any malformed entries.
 *
 * Return: AGENT_OK on success, AGENT_ERR_INVALID if out is NULL,
 *         AGENT_ERR_IO if /proc/partitions cannot be opened
 */
int agent_disk_read(AgentDisk *out) {
  if (!out)
    return AGENT_ERR_INVALID;

  FILE *f = fopen("/proc/partitions", "r");
  if (!f)
    return AGENT_ERR_IO;

  char buf[MAXC_CHAR];
  fgets(buf, sizeof(buf), f);

  while (fgets(buf, sizeof(buf), f)) {
    Partition p = {0};
    char name[MAXC_CHAR];

    if (sscanf(buf, "%lu %lu %lu %255s", &p.major, &p.minor, &p.blocks, name) !=
        4)
      continue;

    p.name = strdup(name);
    if (!p.name)
      continue;

    disk_push_partition(&out->data, p);
  }

  fclose(f);
  return AGENT_OK;
}

/*
 * agent_disk_destroy - Free an AgentDisk structure and all partition data
 * @d: Pointer to AgentDisk to free
 *
 * Frees all dynamically allocated partition names and the partition array.
 */
void agent_disk_destroy(AgentDisk *d) {
  if (!d)
    return;
  for (size_t i = 0; i < d->data.count; ++i)
    free(d->data.parts[i].name);
  free(d->data.parts);
  free(d);
}

/*
 * agent_device_create - Allocate and initialize a new AgentDevice structure
 *
 * Return: Pointer to newly allocated AgentDevice, or NULL on allocation failure
 */
AgentDevice *agent_device_create(void) {
  return calloc(1, sizeof(AgentDevice));
}

/*
 * collect_processes - Collect all running process IDs from /proc
 * @dev: Pointer to Device structure to populate with process IDs
 *
 * Scans /proc directory for numeric entries (process IDs) and stores them
 * as strings in the Device structure. Dynamically grows the process array
 * as needed.
 */
void collect_processes(Device *dev) {
  DIR *d = opendir("/proc");
  if (!d)
    return;

  struct dirent *e;
  size_t cap = 8;
  dev->procs = malloc(sizeof(char *) * cap);

  while ((e = readdir(d))) {
    if (!is_numeric(e->d_name))
      continue;

    if (dev->procs_count == cap) {
      cap *= 2;
      char **np = realloc(dev->procs, sizeof(char *) * cap);
      if (!np)
        break;
      dev->procs = np;
    }

    dev->procs[dev->procs_count++] = strdup(e->d_name);
  }

  closedir(d);
}

/*
 * agent_device_read - Read device information including OS version, uptime, and
 * processes
 * @out: Pointer to AgentDevice structure to populate
 *
 * Reads OS version from /proc/version, uptime from /proc/uptime, and collects
 * all running process IDs from /proc directory.
 *
 * Return: AGENT_OK on success, AGENT_ERR_INVALID if out is NULL,
 *         AGENT_ERR_IO if required files cannot be opened
 */
int agent_device_read(AgentDevice *out) {
  if (!out)
    return AGENT_ERR_INVALID;

  FILE *u = fopen("/proc/uptime", "r");
  FILE *v = fopen("/proc/version", "r");
  if (!u || !v) {
    if (u)
      fclose(u);
    if (v)
      fclose(v);
    return AGENT_ERR_IO;
  }

  fgets(out->data.uptime, sizeof(out->data.uptime), u);
  fgets(out->data.os_version, sizeof(out->data.os_version), v);

  fclose(u);
  fclose(v);

  collect_processes(&out->data);
  return AGENT_OK;
}

/*
 * agent_device_destroy - Free an AgentDevice structure and all process data
 * @d: Pointer to AgentDevice to free
 *
 * Frees all dynamically allocated process ID strings and the process array.
 */
void agent_device_destroy(AgentDevice *d) {
  if (!d)
    return;
  for (size_t i = 0; i < d->data.procs_count; ++i)
    free(d->data.procs[i]);
  free(d->data.procs);
  free(d);
}

/*
 * agent_process_kill - Send a signal to a process
 * @pid: Process ID to signal
 * @signal: Signal number to send (e.g., SIGTERM, SIGKILL)
 *
 * Wrapper around kill(2) system call with error handling.
 *
 * Return: AGENT_OK on success,
 *         AGENT_ERR_INVALID if pid is invalid or process not found,
 *         AGENT_ERR_PERM if permission denied,
 *         AGENT_ERR_IO for other errors
 */
int agent_process_kill(pid_t pid, int signal) {
  if (pid <= 0)
    return AGENT_ERR_INVALID;

  if (kill(pid, signal) == -1) {
    if (errno == EPERM)
      return AGENT_ERR_PERM;
    if (errno == ESRCH)
      return AGENT_ERR_INVALID;
    return AGENT_ERR_IO;
  }
  return AGENT_OK;
}

char *agent_cpu_get_vendor(AgentCpu *a) { return a->data.vendor; }
char *agent_cpu_get_model(AgentCpu *a) { return a->data.model; }
char *agent_cpu_get_frequency(AgentCpu *a) { return a->data.frequency; }
char *agent_cpu_get_cores(AgentCpu *a) { return a->data.cores; }

char *agent_ram_get_total(AgentRam *a) { return a->data.total; }
char *agent_ram_get_free(AgentRam *a) { return a->data.free; }

Partition *agent_disk_get_partitions(AgentDisk *a) { return a->data.parts; }
size_t agent_disk_get_count(AgentDisk *a) { return a->data.count; }

uint64_t agent_partition_get_major(Partition *p) { return p->major; }
uint64_t agent_partition_get_minor(Partition *p) { return p->minor; }
uint64_t agent_partition_get_blocks(Partition *p) { return p->blocks; }
char *agent_partition_get_name(Partition *p) { return p->name; }

char *agent_device_get_os_version(AgentDevice *d) { return d->data.os_version; }
char *agent_device_get_uptime(AgentDevice *d) { return d->data.uptime; }
char **agent_device_get_procs(AgentDevice *d) { return d->data.procs; }
size_t agent_device_get_procs_count(AgentDevice *d) {

  return d->data.procs_count;
}
