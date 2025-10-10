#ifdef __APPLE__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include "cpu.h"

void* cpu_name();
void* cpu_thread_count();

cpu_s cpu;

void* cpu_name(){

    char *name;
    size_t len = 0;

    if (sysctlbyname("machdep.cpu.brand_string", NULL, &len, NULL, 0) < 0)
        perror("errorn in assigning the size for the cpu name variable\n");

    name = malloc(len);

    if(sysctlbyname("machdep.cpu.brand_string", name, &len, NULL, 0) < 0){
        perror("error in assigning the value to the cpu name variable\n");

        free(name);
        return NULL;
    }

    cpu.name = name;
    return NULL;
}

void* cpu_threads(){

    int count;
    size_t len = sizeof(count);
    if (sysctlbyname("machdep.cpu.thread_count", &count, &len, NULL, 0) < 0)
        perror("error in retrieving the cpu threads count\n");

    cpu.threads = count;
    return NULL;
}

void* cpu_info(){
    cpu_threads();
    cpu_name();

    printf("cpu name: %s\ncpu threads: %d\n", cpu.name, cpu.threads);
    return NULL;
}

#endif
