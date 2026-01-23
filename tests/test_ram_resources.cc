#include "base/base.h"
#include "base/base_arena.h"
#include "libres/resources.h"

#include "ase/base.c"
#include "base/base_arena.c"
#include "libres/resources.cc"

/*
  test if ram_create actually creates a ram object
 */
local_internal void
test_ram_create()
{
    mem_arena *arena = arena_create(KiB(1));
    Ram       *ram   = ram_create(arena);

    test(ram != NULL);
}

/*
  test if ram read actually reads data in the ram object
 */
local_internal void
test_ram_read_returns_ok()
{
    mem_arena *arena = arena_create(KiB(1));
    Ram       *ram   = ram_create(arena);

    test(ram_read(ram) == ERR_OK);
}

/*
  test if the values values are not zero
 */
local_internal void
test_ram_read_non_empty_values()
{
    mem_arena *arena = arena_create(KiB(1));
    Ram       *ram   = ram_create(arena);

    ram_read(ram);

    test(ram->total == 0);
    test(ram->free == 0);
}

int
main()
{
    test_ram_create();
    test_ram_read_returns_ok();

    return 0;
}
