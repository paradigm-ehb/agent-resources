#include <systemd/sd-bus-vtable.h>
#include <systemd/sd-bus.h>

#include "base/base.h"

local_internal sd_bus *
create_session_bus_connection()
{
    sd_bus *bus;
    sd_bus_default_user(&bus);
    return bus;
}

local_internal sd_bus *
create_system_bus_connection()
{
    sd_bus *bus;
    sd_bus_default_system(&bus);
    return bus;
}

local_internal sd_bus_slot *
create_systemd_object(sd_bus *bus, const sd_bus_vtable *vtable)
{
    sd_bus_slot *slot = NULL;

    const char *path      = "/org/freedesktop/systemd1";
    const char *interface = "org.freedesktop.systemd1.Manager";

    sd_bus_add_object_vtable(
    bus,
    &slot,
    path,
    interface,
    vtable,
    NULL);

    return slot;
}

local_internal sd_bus_slot *
create_logind_object(sd_bus *bus, const sd_bus_vtable *vtable)
{
    sd_bus_slot *slot = NULL;

    const char *path      = "/org/freedesktop/login1";
    const char *interface = "org.freedesktop.login1";

    sd_bus_add_object_vtable(
    bus,
    &slot,
    path,
    interface,
    vtable,
    NULL);

    return slot;
}
