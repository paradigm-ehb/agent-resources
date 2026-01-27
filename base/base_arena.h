#ifndef ARENA_H
#define ARENA_H

#include "base.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * Arena Helper macro's
 * */

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define ALIGN_UP_POW2(n, p) align_up_pow((u64)(n), (u64)(p))

#define PUSH_STRUCT(arena, T) (T *)arena_push((arena), sizeof(T), 0)
#define PUSH_STRUCT_NZ(arena, T) (T *)arena_push((arena), sizeof(T), 1)
#define PUSH_ARRAY(arena, T, n) (T *)arena_push((arena), sizeof(T) * (n), 0)
#define PUSH_ARRAY_NZ(arena, T, n) (T *)arena_push((arena), sizeof(T) * (n), 1)

#define ARENA_ALIGN (sizeof(void *))

typedef struct mem_arena  global_arena;
typedef struct temp_arena temp_arena;

struct mem_arena
{
    u64 capacity;
    u64 pos;
    u64 prev_pos;
};

struct temp_arena
{
    global_arena *arena;
    umm           offset;
    umm           prev_offset;
};

local_internal global_arena *
arena_create(u64 capacity);

local_internal void
arena_destroy(global_arena *arena);

local_internal void *
arena_push(global_arena *arena, u64 size, b32 non_zero);

local_internal void
arena_pop(global_arena *arena, u64 size);

local_internal void
arena_pop_to(global_arena *arena, u64 pos);

local_internal void
arena_clear(global_arena *arena);

local_internal void
temp_arena_create();

#endif
