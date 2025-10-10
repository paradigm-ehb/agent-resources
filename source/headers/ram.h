#ifndef RAM_H
#define RAM_H

#ifdef __APPLE__

void* size();
void* av_size();
void* ram_info();

typedef struct {
    long double total;
    long double available;
} ram_s;


#endif
#ifdef __gnu_linux__

void size();
void av_size();
void *ram_i(void *a);

#endif
#endif
