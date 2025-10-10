#ifdef __APPLE__

#include <stdio.h>
#include <sys/param.h>
#include <sys/mount.h>
#include "disk.h"


void* disk_size();

disk_s disk_info;

void* disk_size(){

    struct statfs disk;

    statfs("/", &disk);

    disk_info.size = disk.f_bfree;
    return NULL;
}

void* get_disk_info(){

    printf("free disk space: %llu\n", disk_info.size);

    return NULL;
}

#endif
