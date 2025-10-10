#include <pthread.h>
#include "headers/cpu.h"
#include "headers/ram.h"
#include "headers/disk.h"
#include "headers/device.h"
#include <curl/curl.h>
#include <cjson/cJSON.h>

void handler(char *url, cJSON object) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, object);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

void setup_mt() {
    pthread_t cpu_t,ram_t; // disk_t , device_t;

    pthread_create(&cpu_t, NULL, cpu_i, NULL);
    pthread_create(&ram_t, NULL, ram_i, NULL);
//    pthread_create(&disk_t, NULL, disk_i, NULL);
//    pthread_create(&device_t, NULL, device_i, NULL);

    pthread_join(cpu_t, NULL);
    pthread_join(ram_t, NULL);
 //   pthread_join(disk_t, NULL);
 //   pthread_join(device_t, NULL);
}

#ifdef __APPLE__

int main(int argc, char** argv) {

    setup_mt();
    handler("api.nsrddyn.com");

    return 0;
}

#endif

#ifdef __gnu_linux

void setup_mt(){

   pthread_t cpu;
   pthread_t ram;
   pthread_t disk;
   pthread_t device;

   pthread_create(&cpu, NULL, cpu_i, NULL);
   pthread_create(&ram, NULL, ram_i, NULL);
   pthread_create(&disk, NULL, disk_i, NULL);
   pthread_create(&device, NULL, device_i, NULL);

}

int main(int argc, char** argv) {

    setup_mt();

    return 0;
}

#endif
