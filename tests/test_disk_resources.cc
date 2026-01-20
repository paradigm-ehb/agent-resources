#include "base/base.h"
#include "base/base_arena.h"
#include "libres/resources.h"

#include "base/base.c"
#include "base/base_arena.c"
#include "libres/resources.cc"


/*
 * Test case:
 * test if the retrieved disks aren't 0
 *
 * */
local_internal void
test_partition_count()
{
  mem_arena *arena = arena_create(MiB(8));

  Disk *disk = disk_create(arena);
  disk_read(disk, arena);

  test_assert((disk->part_count != 0) && "Failed to retrieve partition count");

}

/*
 * Test case:
 * test if the partitions get allocated
 * */
local_internal void
test_partition_creation()
{
  mem_arena *arena = arena_create(MiB(8));

  Disk *disk = disk_create(arena);
  disk_read(disk, arena);

  test_assert((disk->partitions != NULL));
}

/*
 * Test case:
 * test if the partition capacity is bigger then the partition count
 * */
local_internal void
test_partition_capacity_bigger_then_count()
{
  mem_arena *arena = arena_create(MiB(8));

  Disk *disk = disk_create(arena);
  disk_read(disk, arena);

  test_assert((disk->part_capacity >= disk->part_count));
}

/*
 * Test case:
 * test if the partition capacity is bigger then the partition count
 * */
local_internal void
test_individual_partition_non_null()
{
  mem_arena *arena = arena_create(MiB(8));

  Disk *disk = disk_create(arena);
  disk_read(disk, arena);

  for (
      size_t parition_index = 0;
      parition_index < disk->part_count;
      ++parition_index
      )
  {
    /*
     * TODO(nasr): iterate over partitions and check if not null
     * */
  }
}


int
main()
{
  test_partition_count();
  test_partition_creation();
  test_partition_capacity_bigger_then_count();
}
