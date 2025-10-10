#ifdef __APPLE__

#include <stdlib.h>
#include <stddef.h>
#include "ram.h"
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/sysctl.h>

void size();
void av_size();
void* ram_i();


ram_s data;

#define D 1073741824


void size() {
    int64_t size;
    size_t len = sizeof(size);
    if (sysctlbyname("hw.memsize", &size, &len, NULL, 0) < 0)
        perror("error in retrieving the memory size");

    data.total = size / D;
    return NULL;
}

void av_size() {
    int64_t size;
    size_t len = sizeof(size);
    if (sysctlbyname("hw.memsize_usable", &size, &len, NULL, 0) < 0)
        perror("error in retrieving the available memory size");

    data.available = size / D;
    return NULL;
}

void ram_info() {
    size();
    av_size();

    printf("available ram: %LF\n", data.available);
    printf("total ram: %LF\n", data.total);

    return NULL;
}

void* ram_i(){

    printf("thread is working");
    return NULL;
}

#endif
