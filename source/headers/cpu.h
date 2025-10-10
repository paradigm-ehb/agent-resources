#ifndef CPU_H
#define CPU_H

#ifdef __gnu_linux__


void cpu_name();
void cpu_freq();
void cpu_temperature();
void *cpu_i(void *a);

#endif
#ifdef __APPLE__

void *cpu_name();
void *cpu_threads();
void *cpu_info();


typedef struct {

    int frequency;
    char* name;
    int threads;

} cpu_s;


#endif
#endif
