#ifndef ARENA_H
#define ARENA_H

#include "base.h"

/**
 * Arena Helper macro's
 * */

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define ALIGN_UP_POW2(n, p) (((u64)(n) + ((u64)(p) - 1)) & (~((u64)(p) - 1)))

#define PUSH_STRUCT(arena, T) (T *)arena_push((arena), sizeof(T), 0)
#define PUSH_STRUCT_NZ(arena, T) (T *)arena_push((arena), sizeof(T), 1)
#define PUSH_ARRAY(arena, T, n) (T *)arena_push((arena), sizeof(T) * (n), 0)
#define PUSH_ARRAY_NZ(arena, T, n) (T *)arena_push((arena), sizeof(T) * (n), 1)

#define ARENA_BASE_POS (sizeof(mem_arena))
#define ARENA_ALIGN (sizeof(void *))

typedef struct mem_arena mem_arena;

struct mem_arena
{
    u64 capacity;
    u64 pos;
};

local_internal mem_arena *
arena_create(u64 capacity);

local_internal void
arena_destroy(mem_arena *arena);

local_internal void *
arena_push(mem_arena *arena, u64 size, b32 non_zero);

local_internal void
arena_pop(mem_arena *arena, u64 size);

local_internal void
arena_pop_to(mem_arena *arena, u64 pos);

local_internal void
arena_clear(mem_arena *arena);

#endif
