/*
 * Author: nasr
 * Year: 2025-2026
 * */

#include <unistd.h>
#include <signal.h>
#include <stdint.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

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
	uint64_t major;
	uint64_t minor;
	uint64_t blocks;
	char	 *name;

} Partitions;

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
	Partitions *part;
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
// return the int in the buffer
// cool code but won't use it propably :(
uint64_t
*get_part_num(char *input)
{
	uint64_t *major = malloc(sizeof(uint64_t));
	while (*input && *input == ' ')
	{
		input++;
	}

	while (*input >= '0' && *input <= '9')
	{
		// add the current index ascii code - the ascii code of 0 to get the integer
		// then multiply the major value by 10 to insert the next value
		*major = (*major * 10) + (*input - '0');
		input++;
	}

	return major;
}


void disk_data(Disk *disk) {

	if (!disk->part)
		perror("nothing allocated for part");

	char disk_info[] = "/proc/partitions";

	char buffer[MAXC_CHAR] = "";
	FILE *file = fopen(disk_info, "r");
	if (!file)
		return;

	fgets(buffer, sizeof(buffer), file);

	while (fgets(buffer, sizeof(buffer), file) != NULL)
	{
		char num_buf[MAXC_CHAR] = "";
		char name_buf[MAXC_CHAR] = "";

		size_t num_idx = 0;
		size_t name_idx = 0;
		int field = 0;

		for (size_t i = 0; buffer[i] && buffer[i] != '\n'; i++)
		{
			if (buffer[i] >= '0' && buffer[i] <= '9')
			{
				num_buf[num_idx++] = buffer[i];
			}
			else if (buffer[i] == ' ')
			{
				if (num_idx > 0)
				{
					num_buf[num_idx] = '\0';

					if (field == 0)
						disk->part->major = atoi(num_buf);
					else if (field == 1)
						disk->part->minor = atoi(num_buf);
					else if (field == 2)
						disk->part->blocks = atoi(num_buf);

					field++;
					num_idx = 0;
				}
			}
			else
			{
				name_buf[name_idx++] = buffer[i];
			}
		}

		name_buf[name_idx] = '\0';

		disk->part->name = malloc(sizeof(char) *name_idx);
		strcpy(disk->part->name, name_buf);

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

int
terminate_process(char *pid)
{
	pid_t p ;

	if (kill(p, SIGTERM) == -1)
	{
		perror("failed to kill pid: %s", pid);
		return 1;
	}
	return 0;

}

