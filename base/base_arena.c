#include "base_arena.h"

local_internal inline b8
is_aligned(umm x)
{
    return (x & (x - 1)) == 0;
}

local_internal inline u64
align_up_pow(u64 n, u64 p)
{
    /*
   * TODO(nasr): there is bug here grrr
   * were checking if the memory is aligned but the function
   * is used to align the memory
   * make a seperate function that checks for that alignment
   * check(p && ((p & (p - 1)) == 0));
   * */
    return (n + (p - 1)) & ~(p - 1);
}

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

/**
 * TODO(nasr): remove stdlib
 * */
/* credits gingerbill arena allocaters article */
local_internal inline u64
align_stub(u64 ptr, u64 align)
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

local_internal mem_arena *
arena_create(u64 capacity)
{
    mem_arena *arena = (mem_arena *)mmap(
    0,
    capacity,
    PROT_READ | PROT_WRITE | PROT_EXEC,
    MAP_SHARED | MAP_ANONYMOUS,
    -1,
    0);

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
arena_destroy(mem_arena *arena)
{
    munmap(arena, arena->capacity);
}

local_internal void *
arena_alloc(mem_arena *arena, u64 size)
{
    u64 pos_aligned = ALIGN_UP_POW2(arena->pos, ARENA_ALIGN);
    u64 new_pos     = pos_aligned + size;

    if (new_pos > arena->capacity)
    {
        assert(0);
        return NULL;
    }

    arena->pos = new_pos;
    u8 *out    = (u8 *)arena + pos_aligned;

    memset(out, 0, size);
    return out;
}

local_internal void
arena_pop(mem_arena *arena, u64 size)
{
    size = MIN(size, arena->pos - sizeof(*arena));
    arena->pos -= size;
}

local_internal void
arena_pop_to(mem_arena *arena, u64 pos)
{
    u64 size = pos < arena->pos ? arena->pos - pos : 0;
    arena_pop(arena, size);
}

local_internal void
arena_clear(mem_arena *arena)
{
    arena_pop_to(arena, sizeof(*arena));
}

local_internal void
arena_resize(mem_arena *arena)
{
    check(is_aligned(arena->pos));
}

local_internal temp_arena
temp_arena_begin(mem_arena *arena)
{
    temp_arena temp;
    temp.arena       = arena;
    temp.prev_offset = arena->prev_pos;
    temp.offset      = arena->pos;

    return temp;
}

local_internal void
temp_arena_end(temp_arena temp)
{
    temp.arena->prev_pos = temp.prev_offset;
    temp.arena->pos      = temp.offset;
}

local_internal void
stack_create(u64 size)
{
    /*TODO(nasr)*/
}

local_internal void
stack_destroy(u64 size)
{
}
