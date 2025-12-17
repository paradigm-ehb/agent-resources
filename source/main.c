/*
 * Author: nasr
 * Year: 2025-2026
 * */

#include <unistd.h>
#include <stdint.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <proc/readproc.h>
#include <signal.h>

// TODO(nasr): create and start virtual machines using KVM - QEMU - LIBVIRT
// #include <libvirt/libvirt.h>

#define MAXC_CHAR 256


typedef struct {

	uint8_t *base; 
	size_t idx;
	size_t cap;

} Arena;

Arena 
*ArenaAlloc(void) 
{

	return NULL;
}

void
*ArenaRelease(Arena *arena) 
{

	return NULL;
}

Arena
*ArenaPush(Arena *arena, size_t bytes)
{

	return NULL;
}

void
ArenaPop(Arena *arena)
{

}

uint64_t
ArenaGetPos(Arena *arena)
{

	return 0;
}

void
ArenaClear(Arena *arena)
{
	// TODO(nasr): check if there is more needed to do here
	free(arena);
}

typedef struct 
{

	char vendor[MAXC_CHAR]; 
	char model[MAXC_CHAR]; 
	char frequency[MAXC_CHAR]; 
	char cores[MAXC_CHAR]; 

} Cpu;

typedef struct 
{
	char total[MAXC_CHAR];
	char free[MAXC_CHAR];
} Ram;

typedef struct 
{
	long size;
	char *name;
} Disk;

typedef struct 
{ 

	char hostname[MAXC_CHAR]; 
	char os_version[MAXC_CHAR];
	char uptime[MAXC_CHAR];
	char **procs;
	int	 procs_count;

} Device;


// math heper functions
// @param takes a base and an exponent
int
ipow(int base , int exp ) 
{

	uint64_t result = 0;
	for (int i = 0; i < exp; ++i) {

		result *= base;
	}

	return result;

	return 0;
}

int
fpow(double base, double exp) 
{

	float result = 0;

	for (int i = 0; i < exp; ++i) {

		result *= base;
	}

	return result;
}


void
cpu_data(Cpu *cpu) 
{
	char basic_cpu_info[]	= "/proc/cpuinfo";

	char vendor_id[]		= { "vendor_id"};
	char model[]			= { "model name"};
	char frequency[]		= { "cpu MHz"};
	char cores[]			= { "cpu cores"};

	FILE *file = fopen(basic_cpu_info, "r");
	if (!file)
	{
		//TODO(nasr): write this to the created agent log system
		return;
	}

	char buffer[MAXC_CHAR] = "";
	while(fgets(buffer, sizeof(buffer), file) != NULL)
	{
		char *colon = strchr(buffer, ':');
		if (!colon)
			continue; 

		char *start = colon + 1;
		 
		while (*start == ' ')
			start++;

		char *end = strchr(start, '\n');
   
		if (!end)
			end = start + strlen(start);

		size_t length = (size_t)(end - start);

		if ((strncmp(buffer, vendor_id, sizeof(vendor_id) - 1)) == 0) 
		{
			memcpy(cpu->vendor, start, length);
			cpu->vendor[length] = '\0';
		}

		if ((strncmp(buffer, model, sizeof(model) - 1)) == 0) 
		{
			memcpy(cpu->model, start, length);
			cpu->model[length] = '\0';
		}

		if ((strncmp(buffer, frequency , sizeof(frequency) - 1)) == 0) 
		{
			memcpy(cpu->frequency, start, length);
			cpu->frequency[length] = '\0';
		}

		if ((strncmp(buffer, cores, sizeof(cores) - 1)) == 0) 
		{
			memcpy(cpu->cores, start, length);
			cpu->cores[length] = '\0';
		}
	}

	fclose(file);
}

void
memory_data(Ram *ram) 
{

	char mem_info[]	= "/proc/meminfo";

	char total[]		= { "MemTotal"};
	char free[]			= { "MemFree"};

	FILE *file = fopen(mem_info, "r");
	if (!file)
	{
		//TODO(nasr): write this to the created agent log system
		fclose(file);
		return;
	}

	char buffer[MAXC_CHAR] = "";
	while(fgets(buffer, sizeof(buffer), file) != NULL)
	{
		char *colon = strchr(buffer, ':');
		if (!colon)
			continue; 
 
		char *start = colon + 1;
		 
		while (*start == ' ')
			start++;

		char *end = strchr(start, 'k');

		if (!end)
			end = start + strlen(start);

		size_t length = (size_t)(end - start);


		if ((strncmp(buffer, total, sizeof(total) - 1)) == 0) 
		{
			memcpy(ram->total, start, length);
			ram->total[length] = '\0';
		}

		if ((strncmp(buffer, free, sizeof(free) - 1)) == 0) 
		{
			memcpy(ram->free, start, length);
			ram->free[length] = '\0';
		}

	}
	fclose(file);
}

void
disk_data(Disk *disk) 
{
	// TODO(nasr): get the partitions names, get the paritions sizes
	// TODO(nasr): rewrite the disk struct
	char disk_info[]	= "/proc/partitions";
	FILE *file = fopen(disk_info, "r");
	if (!file)
	{
		fclose(file);
		return;
	}

	fclose(file);
}

// own impementation
void
get_processes(Device *device)
{
    const char *path = "/proc";
    struct dirent *entry;

    size_t pid_count = 0;
    size_t pid_cap   = 8;

    DIR *dP = opendir(path);
    if (!dP) {
        perror("failed to open /proc");
        return;
    }

    char **pid_list = (char**)malloc(sizeof(char*) * pid_cap);
    if (!pid_list) {
        perror("malloc failed");
        closedir(dP);
        return;
    }

    while ((entry = readdir(dP)) != NULL) {
        if (atoi(entry->d_name) == 0)
            continue;

        if (pid_count == pid_cap) {
            pid_cap *= 2;
            char **tmp = (char**)realloc(pid_list, sizeof(char*) * pid_cap);
            if (!tmp) {
				free(pid_list);
                perror("realloc failed");
                break;
            }
            pid_list = tmp;
        }

        size_t len = strlen(entry->d_name) + 1;
        pid_list[pid_count] = (char*)malloc(len);
        if (!pid_list[pid_count]) {
            perror("malloc failed");
            break;
        }

        memcpy(pid_list[pid_count], entry->d_name, len);
        pid_count++;
    }

    closedir(dP);

    size_t valid_count = 0;
    device->procs = (char**)malloc(sizeof(char*) * pid_count);
    if (!device->procs) {
        perror("malloc failed");
    }

    for (size_t i = 0; i < pid_count; i++) {
        char sub_path[64];

        snprintf(sub_path, sizeof(sub_path), "/proc/%s", pid_list[i]);

        DIR *sdP = opendir(sub_path);
        if (!sdP)
            continue;

        closedir(sdP);

        device->procs[valid_count++] = pid_list[i];
    }
	device->procs_count = valid_count;
}


void
device_data(Device *device) 
{

	const char *uptime_path = "/proc/uptime";
	const char *os_version_path = "/proc/version";

	FILE *uptime_file = fopen(uptime_path, "r");
	if (!uptime_file)
		return;

	FILE *os_version_file = fopen(os_version_path, "r");
	if (!os_version_file)
		return;



	char os_version_content[MAXC_CHAR]	= "";
	char uptime_content[MAXC_CHAR]		= "";

	fgets(os_version_content, MAXC_CHAR, os_version_file);
	fgets(uptime_content,MAXC_CHAR, uptime_file); 

	memcpy(device->uptime, uptime_content, sizeof(uptime_content));
	memcpy(device->os_version, os_version_content ,sizeof(os_version_content));

	fclose(uptime_file);
	fclose(os_version_file);


	get_processes(device);
}

// Library
int
list_processes()
{
    PROCTAB *proc = openproc(PROC_FILLSTAT | PROC_FILLSTATUS);
    proc_t proc_info;

    if (!proc) {
        fprintf(stderr, "failed to open proc table\n");
        return 1;
    }
	proc_t *p;
	while ((p = readproc(proc, NULL)) != NULL) {
		printf("PID: %d CMD: %s STATE: %c\n",
				p->tid,
				p->cmd,
				p->state);
	}
	// iterate all processes
    closeproc(proc);
	return 0;
}

int
kill_process(char *pid)
{

	return 0;

}

int 
main() 
{
	ezway();
	// Arena *arena; 
	//
	Cpu *cpu;
	Ram *ram;
	Device *device;
	Disk *disk;

	// ArenaPush(arena, sizeof(Cpu));
	// ArenaPush(arena, sizeof(Ram));
	// ArenaPush(arena, sizeof(Disk));
	// ArenaPush(arena, sizeof(Device));
	//
	// ArenaAlloc();

	cpu = (Cpu*)malloc(sizeof(Cpu));
	ram = (Ram*)malloc(sizeof(Ram));
	device = (Device*)malloc(sizeof(Device));
	disk = (Disk*)malloc(sizeof(Disk));

	cpu_data(cpu);
	memory_data(ram);
	device_data(device);
	disk_data(disk);

	printf("================================================================");
	printf("\nmodel: %s\n", cpu->model);
	printf("\nvendor: %s\n", cpu->vendor);
	printf("\nfrequency: %s\n", cpu->frequency);
	printf("\ncores: %s\n", cpu->cores);


	printf("================================================================");


	printf("\ntotal: %s\n", ram->total);
	printf("\nfree: %s\n", ram->free);

	printf("================================================================\n");
	
	printf("os_version: %s\n", device->os_version);
	printf("uptime: %s\n", device->uptime);


	printf("================================================================\n");

	for (int i = 0; i < device->procs_count; i++)
	{

		printf("procs: %s\n", device->procs[i]);
	}

	for (int i = 0; i < device->procs_count; i++) {
		free(device->procs[i]);
	}
	free(device->procs);

	free(cpu);
	free(ram);
	free(device);
	free(disk);

	// ArenaRelease(arena);
	return 0;
}

// TODO(nasr): find a way to use libvirt to create and start a virtual machine
// TODO(nasr): find a way to pass parameters to libvirt

void
create_vm()
{

	// TODO(nasr): create and start virtual machines using libirt

}

