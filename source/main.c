#include <pthread.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "lib.h"
#include <stdlib.h>
#include <stdio.h>

void *gather_cpu_information();
void *gather_ram_information();
void *gather_disk_information();
void *gather_device_information();

/*
cJSON convert_to_json() {

  cJSON *root = cJSON_CreateObject();
  cJSON_AddStringToObject(root, "cpu", info.cpu->name);
  cJSON_AddNumberToObject(root, "ram", info.ram->total);
  cJSON_AddStringToObject(root, "disk", info.disk->name);
  cJSON_AddStringToObject(root, "device", info.device->name);

  return *root;
}

void handler(char *url) {
  CURL *curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // TO DO
    // parse the object to a json and pass that as an argument to post it
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, NULL);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
          curl_easy_strerror(res));

    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
}


*/



#ifdef __APPLE__

int main(int argc, char** argv) {

#if __STDC_VERSION__
  printf("C standard version: %ld\n", __STDC_VERSION__);
#else
  printf("C standard not defined\n");
#endif

  // handler("api.nsrddyn.com");

  return 0;
}

#endif

#ifdef __gnu_linux

int main(int argc, char** argv) {


  return 0;
}

#endif

