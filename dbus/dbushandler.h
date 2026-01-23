#include <stdint.h>
#include <systemd/sd-bus.h>

typedef struct Process        Process;
typedef struct Service        Service;
typedef struct System         System;
typedef struct BusConnection  BusConnection;
typedef struct Unit           Unit;
typedef struct LoadedUnit     LoadedUnit;
typedef struct UnitFileChange UnitFileChange;

/**
 * NOTE(nasr): dbus object functionality is handled differently in C
 * You'll typically use the connection with object paths directly
 * */
struct
BusConnection
{
    BusConnection *conn;
};

struct
Process
{
    char    *name;
    uint32_t id;
};

struct
Service
{
    char    *name;
    uint32_t id;
    char    *owner;
};

struct
System
{
    char    *os;
    Process *processes;
    size_t   process_count;
    Service *services;
    size_t   service_count;
};

struct
Unit
{
    char *name;
    char *state;
};

struct
LoadedUnit
{
    char    *name;
    char    *description;
    char    *load_state;
    char    *sub_state;
    char    *active_state;
    char    *dep_unit;
    char    *object_path;
    uint32_t queued_job;
    char    *job_type;
    char    *job_path;
};

struct
UnitFileChange
{
    char *type_of_change;
    char *file_name_symlink;
    char *destination_symlink;
};
