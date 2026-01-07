#include "arena.h"
#include "base.h"
#include "resources.h"
#include <string.h>
#include <stdio.h>

char *
check_distro_version(mem_arena *arena)
{
  Device *device = device_create((arena));
  device_read(device);

  i32 len = sizeof(device->os_version);
  char distro_unparsed[len];

  memcpy(distro_unparsed, device->os_version, len);

  i32 word_idx = 0;
  char *buffer = (char *)arena_push(arena, len, 1);

  while (distro_unparsed[word_idx] != '\0')
  {
    if (distro_unparsed[word_idx] != ' ')
    {
      buffer[word_idx] = distro_unparsed[word_idx];
    }

    ++word_idx;
  }

  buffer[word_idx] = '\0';

#define DISTRO_ARCH

  arena_destroy(arena);

  return buffer;
}

i32
main()
{
  mem_arena *arena = arena_create(KiB(4));
  printf("%s", check_distro_version(arena));

  return 0;
}
