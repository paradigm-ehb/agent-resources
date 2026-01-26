/**
 * TODO(nasr): remove stdlib
 * */
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#include "base_arena.h"

/* credits gingerbill arena allocaters article */
local_internal inline u64
align(u64 ptr, u64 align)
{
  umm p, a, modulo;

  p = ptr;
  a = (umm)align;

  modulo = p & (a - 1);

  if (modulo != 0)
  {
    p += a - modulo;
  }

  return p;
}

local_internal global_arena *
arena_create(u64 capacity)
{
    global_arena *arena = (global_arena *)mmap(0, capacity, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (arena == MAP_FAILED)
    {
        return NULL;
    }

    arena->capacity = capacity;
    arena->pos      = sizeof(*arena);
    arena->prev_pos = sizeof(*arena);

    return arena;
}

/*
 * make it a void pointer to allow implicit conversion
 * */
local_internal void
arena_destroy(global_arena *arena)
{
    munmap(arena, arena->capacity);
}

local_internal void *
arena_push(global_arena *arena, u64 size, b32 non_zero)
{
    u64 pos_aligned = ALIGN_UP_POW2(arena->pos, ARENA_ALIGN);
    u64 new_pos     = pos_aligned + size;

    if (new_pos > arena->capacity)
    {
        assert(0);
        return NULL;
    }

    arena->pos = new_pos;
    /*
   * cast to u8 to be able to do pointer arithemtic
   * */
    u8 *out = (u8 *)arena + pos_aligned;

    if (!non_zero)
    {
        memset(out, 0, size);
    }
    return out;
}

local_internal void
arena_pop(global_arena *arena, u64 size)
{
    size = MIN(size, arena->pos - sizeof(*arena));
    arena->pos -= size;
}

local_internal void
arena_pop_to(global_arena *arena, u64 pos)
{
    u64 size = pos < arena->pos ? arena->pos - pos : 0;
    arena_pop(arena, size);
}

local_internal void
arena_clear(global_arena *arena)
{
    arena_pop_to(arena, sizeof(*arena));
}

local_internal temp_arena
temp_arena_begin(mem_arena *arena)
{
  temp_arena temp;
  temp.arena = arena;
  temp.prev_offset = arena->prev_pos;
  temp.offset = arena->pos;

  return temp;
}

local_internal void
temp_arena_end(temp_arena temp)
{
	temp.arena->prev_pos = temp.prev_offset;
	temp.arena->pos = temp.offset;

}
