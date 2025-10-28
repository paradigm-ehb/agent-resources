/*
 * 
 * Author: nasr
 * Year: 2025-2026
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "types.h"
#include "lib.h"

#define MAXC 1024
#define MAXC_CHAR 256
#define CONVERT_BYTES_TO_GIGABYTES 107374182   
#define D 1073741824

/*  
 * prototype function declarations
 * cpu, ram, disk, device
 *
 */

void cpu_name();
void cpu_threads();
void cpu_temperature();
void cpu_frequency();
void get_total();
void get_usage();
void mem_size();
void av_mem_size();
void device_up_time();
void device_os_version();
void device_hostname();
void device_model();

#ifdef __gnu_linux__

#include <sys/sysinfo.h>


void cpu_name() {
  int buffer_size = 256;
  char cpu_name[buffer_size];

  FILE *fp = fopen("/proc/cpuinfo", "r");
  if (!fp)
    perror("can't open /proc/cpuinfo");

  char line[buffer_size];
  while (fgets(line, sizeof(line), fp))
  {
    if (strncmp(line, "model name", 10) == 0)
    {
      char *colon = strchr(line, ':');
      if (colon)
      {
        snprintf(cpu_name, buffer_size, "%s", colon + 2);
        cpu_name[strcspn(cpu_name, "\n")] = 0;
        int err = fclose(fp);
        if (err != 0)
          perror("error closing /proc/cpuinfo");
      }
    }
  }

  u_cpu->name = cpu_name;

}

void cpu_frequency() {
  char* buffer = malloc(MAXC_CHAR);

  FILE *fp = fopen("/proc/cpuinfo", "r");
  if (!fp) {
    printf("can't open /proc/cpuinfo");
    return;
  }

  while (fgets(buffer, MAXC_CHAR, fp)) {
    if (strstr(buffer, "cpu MHz") != NULL)
    {
      char *colon = strchr(buffer, ':');
      if (colon)
      {
        buffer[strcspn(buffer, "\n")] = 0;
        snprintf(buffer, MAXC_CHAR, "%s", colon);
      }
    }

    fclose(fp);
    u_cpu->frequency = buffer;
  }
  return;
}

void mem_size() {

  struct sysinfo info;

  if (sysinfo(&info) != 0) {
    perror("sysinfo");
    return;
  }

  long total_ram = info.totalram * info.mem_unit; 
  u_ram->total = total_ram;

}


void mem_av_size() {
  struct sysinfo info;

  if (sysinfo(&info) != 0) {
    perror("sysinfo");
  }

  long total_ram = info.totalram * info.mem_unit;
  long free_ram = info.freeram * info.mem_unit;

  u_ram->available = free_ram;
}


void cpu_temperature() {

  int delay = 0;
  while (1)
  {
    sleep(delay);
    FILE *pf = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (!pf)
      printf("error reading /proc/cpuinfo");

    char buffer[MAXC];
    while (fgets(buffer, sizeof(buffer), pf))
    {
      int a = atoi(buffer);
      a /= 1000;
      printf("%dC\n", a);
      fflush(stdout);
    }
    fclose(pf);
  }
}


void device_up_time() {
  struct sysinfo info;
  if (sysinfo(&info) == -1)
    perror("sysinfo");

  u_device->uptime = info.uptime;
}


void device_model() {


  return;

}

void cpu_threads() {

  return;
}

void get_total() {


}

void get_usage() {


}

void device_os_version() {


}

#endif

#ifdef __APPLE__

#include <sys/sysctl.h>

void cpu_name() {

  char *name = NULL;
  size_t len = 0;

  if (sysctlbyname("machdep.cpu.brand_string", NULL, &len, NULL, 0) < 0){

    perror("error in assigning the size for the cpu name variable\n");
    return;
  }

  name = malloc(len);

  if (name == NULL) {
    perror("malloc failed"); 
    return;
  }

  if(sysctlbyname("machdep.cpu.brand_string", name, &len, NULL, 0) < 0){
    perror("error in assigning the value to the cpu name variable\n");
    free(name);
    return;
  }

  u_cpu->name = name;
}

void cpu_threads() {

  int count;
  size_t len = sizeof(count);
  if (sysctlbyname("machdep.cpu.thread_count", &count, &len, NULL, 0) < 0){
    perror("error in retrieving the cpu threads count\n");
    return;
  }

  u_cpu->threads = count;
}

void cpu_frequency() {
  // assigning a value of 0, value not available on macos
  u_cpu->frequency = 0;

}

void cpu_temperature() {
  // assigning a value of 0, value not available on macos
  u_cpu->temperature = 0;
}

void mem_size() {
  int64_t size;
  size_t len = sizeof(size);
  if (sysctlbyname("hw.memsize", &size, &len, NULL, 0) < 0)
    perror("error in retrieving the memory size");

  u_ram->total = size / D;
}

void mem_av_size() {
  int64_t size;
  size_t len = sizeof(size);
  if (sysctlbyname("hw.memsize_usable", &size, &len, NULL, 0) < 0)
    perror("error in retrieving the available memory size");

  u_ram->available = size / D;
}

void get_total() {
  
}

void get_usage() {


}

void device_hostname(){

  char *name;
  size_t size = 0;

  if (sysctlbyname("kern.hostname", NULL, &size, NULL, 0) < 0)
    perror("failed retrieving the hostname: \n"); 

  name = malloc(size);

  if(sysctlbyname("kern.hostname", name, &size, NULL, 0) < 0){
    perror("failed retrieving the hostname: \n");
    free(name);
    return ;
  }

  u_device->name = name;
  return ;
}

void device_up_time(){

  // struct timeval boottime;
  unsigned long boottime ; 
  unsigned long len = sizeof(boottime);

  if (sysctlbyname("kern.boottime", &boottime, NULL, NULL, len) == -1){
    perror("sysctl error");
    return;
  }
}

void device_model(){

  char *model_name;
  size_t size = 0;

  if (sysctlbyname("hw.model", NULL, &size, NULL, 0) < 0)
    perror("failed retrieving the model name: \n"); 

  model_name = malloc(size);

  if(sysctlbyname("hw.model", model_name, &size, NULL, 0) < 0){
    perror("failed retrieving the hostname: \n");
    free(model_name);
    return;
  }

  u_device->name = model_name;

}

void device_os_version() {

  char *os_version = NULL;
  size_t size = 0;

  if (sysctlbyname("kern.ostype", NULL, &size, NULL, 0) < 0)
    perror("failed retrieving the hostname: \n"); 

  os_version = malloc(size);

  if(sysctlbyname("kern.ostype", os_version, &size, NULL, 0) < 0){
    perror("failed retrieving the os version: \n");
    free(os_version);
    return ;
  }

  u_device->os_version = os_version;
}

#endif
