#ifdef __gnu_linux__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/sysinfo.h>


typedef struct
{
  unsigned long mem_size;
} ram;

unsigned long get_total(void);
unsigned long get_usage(void);
void *ram_i(void *a);


unsigned long get_total(void){

    struct sysinfo info;

    if (sysinfo(&info) != 0) {
        perror("sysinfo");
        return 1;
    }

    long total_ram = info.totalram * info.mem_unit;
    return total_ram;

}

unsigned long get_usage(void)
{
    struct sysinfo info;

    if (sysinfo(&info) != 0) {
        perror("sysinfo");
        return 1;
    }

    long total_ram = info.totalram * info.mem_unit;
    long free_ram = info.freeram * info.mem_unit;

    return total_ram - free_ram;
  }

void *ram_i(void *a){

    printf("thread is working");
    return NULL;
}



#endif
