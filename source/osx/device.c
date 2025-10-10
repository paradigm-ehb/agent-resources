#ifdef __APPLE__

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <device.h>

device_s device_info;

void* device_name(){

    char *name;
    size_t size = 0;

    if (sysctlbyname("kern.hostname", NULL, &size, NULL, 0) < 0)
        perror("failed retrieving the hostname: \n"); 

    name = malloc(size);

    if(sysctlbyname("kern.hostname", name, &size, NULL, 0) < 0){
        perror("failed retrieving the hostname: \n");
        free(name);
        return NULL;
    }

    device_info.name = name;
    return NULL;
}


void* device_model(){

    char *model_name;
    size_t size = 0;

    if (sysctlbyname("hw.model", NULL, &size, NULL, 0) < 0)
        perror("failed retrieving the hostname: \n"); 

    model_name = malloc(size);

    if(sysctlbyname("hw.model", model_name, &size, NULL, 0) < 0){
        perror("failed retrieving the hostname: \n");
        free(model_name);
        return NULL;
    }

    device_info.model = model_name;
    return NULL;

}

void* device_up_time(){


    
    return NULL;
}

void* device_os_version(){

    char *os_version;
    size_t size = 0;

    if (sysctlbyname("kern.ostype", NULL, &size, NULL, 0) < 0)
        perror("failed retrieving the hostname: \n"); 

    os_version = malloc(size);

    if(sysctlbyname("kern.ostype", os_version, &size, NULL, 0) < 0){
        perror("failed retrieving the os version: \n");
        free(os_version);
        return NULL;
    }

    device_info.model = os_version;
    return NULL;


}

void* get_device_info(){
    device_name();
    device_model();
    printf("device name: %s\ndevice model: %s\n", device_info.name, device_info.model);
    return NULL;
}

#endif
