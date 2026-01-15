#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>

#include "libres/resources.h"
#include "base/.c"

/**
 * Retrieve listening service to port x
 * do not use /etc/services
 * /etc/service is used as a dict
 * use /proc/net/tcp and /proc/net/udp
 * */

i8
port_read_tcp4(Device *device)
{
  char endofline[] = "\n";
  if (!device)
  {
    return ERR_INVALID;
  }

  mem_arena *temp_arena = arena_create(KiB(8));

  char *buffer;
  int file = open("/etc/tcp", O_RDONLY);
  if (file == -1)
  {
    return ERR_IO;
  }

  buffer = PUSH_ARRAY(temp_arena, char, BUFFER_SIZE_SMALL);
  do
  {
    read(file, buffer, 1);
  } while (!compare_string(buffer, endofline));

  arena_destroy(temp_arena);
  close(file);
  return OK;
}

i8
port_read_tcp6(Device *device)
{
  if (!device)
  {
    return ERR_INVALID;
  }

  mem_arena *temp_arena = arena_create(KiB(8));

  char *buffer;
  int file = open("/etc/tcp6", O_RDONLY);
  if (file == -1)
  {
    return ERR_IO;
  }

  buffer = PUSH_ARRAY(temp_arena, char, BUFFER_SIZE_SMALL);
  do
  {
    read(file, buffer, 1);
  } while (!compare_string(buffer, "\n"));

  arena_destroy(temp_arena);
  close(file);
  return OK;
}

/**
 * Think of a way to merge these 2 functions
 * both read tcp
 * one v4 and one v6
 * */
