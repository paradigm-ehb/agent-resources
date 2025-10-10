#include <stdlib.h>
#include "modules/cpu.h"
#include "modules/ram.h"
#include "modules/disk.h"
#include "modules/device.h"
#include <cjson/cJSON.h>

typedef struct {

   cpu_s* cpu;
   ram_s* ram;
   disk_s* disk;
   device_s* device;

} info;


cJSON convert_to_json(info info){

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cpu", info.cpu->name);
    cJSON_AddNumberToObject(root, "ram", info.ram->total);
    cJSON_AddStringToObject(root, "disk", info.disk->name);
    cJSON_AddStringToObject(root, "device", info.device->name);

    return *root;
}
