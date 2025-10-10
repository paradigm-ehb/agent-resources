#ifndef DEVICE_H 
#define DEVICE_H

#ifdef __APPLE__

typedef struct{
    char* name;
} device_s;

extern device_s device_info;

void* get_device_info();


#endif
#endif
