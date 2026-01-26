#include "base/base.h"
#include "base/base_arena.h"
/* unity build */
#include "base/base.c"
#include "base/base_arena.c"
#include <string.h>

/*
 * Test: arena_create returns a valid pointer
 */
local_internal void
test_arena_create()
{
    mem_arena *arena = arena_create(MiB(1));
    test(arena != NULL);
    arena_destroy(arena);
}

/*
 * Test: arena capacity is set correctly
 */
local_internal void
test_arena_capacity()
{
    u64 capacity = MiB(2);
    mem_arena *arena = arena_create(capacity);
    test(arena->capacity == capacity);
    arena_destroy(arena);
}

/*
 * Test: arena initial position is set to sizeof(mem_arena)
 */
local_internal void
test_arena_initial_pos()
{
    mem_arena *arena = arena_create(MiB(1));
    test(arena->pos == sizeof(mem_arena));
    test(arena->prev_pos == sizeof(mem_arena));
    arena_destroy(arena);
}

/*
 * Test: arena_push returns non-NULL pointer
 */
local_internal void
test_arena_push_returns_valid()
{
    mem_arena *arena = arena_create(MiB(1));
    void *ptr = arena_push(arena, 128, 0);
    test(ptr != NULL);
    arena_destroy(arena);
}

/*
 * Test: arena_push zeros memory by default
 */
local_internal void
test_arena_push_zeros_memory()
{
    mem_arena *arena = arena_create(MiB(1));
    u8 *ptr = (u8 *)arena_push(arena, 64, 0);
    b32 all_zero = TRUE;
    for (u64 i = 0; i < 64; i++)
    {
        if (ptr[i] != 0)
        {
            all_zero = FALSE;
            break;
        }
    }
    test(all_zero == TRUE);
    arena_destroy(arena);
}

/*
 * Test: arena_push with non_zero flag doesn't zero memory
 */
local_internal void
test_arena_push_non_zero()
{
    mem_arena *arena = arena_create(MiB(1));
    u8 *ptr = (u8 *)arena_push(arena, 64, 1);
    // Fill with non-zero values
    memset(ptr, 0xFF, 64);
    u8 *ptr2 = (u8 *)arena_push(arena, 64, 1);
    // The memory shouldn't be zeroed
    test(ptr2 != NULL);
    arena_destroy(arena);
}

/*
 * Test: arena_push updates position correctly
 */
local_internal void
test_arena_push_updates_pos()
{
    mem_arena *arena = arena_create(MiB(1));
    u64 initial_pos = arena->pos;
    arena_push(arena, 128, 0);
    test(arena->pos > initial_pos);
    arena_destroy(arena);
}

/*
 * Test: arena_push aligns allocations
 */
local_internal void
test_arena_push_alignment()
{
    mem_arena *arena = arena_create(MiB(1));
    void *ptr1 = arena_push(arena, 1, 0);
    void *ptr2 = arena_push(arena, 1, 0);
    test(((u64)ptr1 % ARENA_ALIGN) == 0);
    test(((u64)ptr2 % ARENA_ALIGN) == 0);
    arena_destroy(arena);
}

/*
 * Test: multiple pushes don't overlap
 */
local_internal void
test_arena_push_no_overlap()
{
    mem_arena *arena = arena_create(MiB(1));
    u32 *ptr1 = (u32 *)arena_push(arena, sizeof(u32), 0);
    u32 *ptr2 = (u32 *)arena_push(arena, sizeof(u32), 0);
    *ptr1 = 0xDEADBEEF;
    *ptr2 = 0xCAFEBABE;
    test(*ptr1 == 0xDEADBEEF);
    test(*ptr2 == 0xCAFEBABE);
    arena_destroy(arena);
}

/*
 * Test: PUSH_STRUCT macro works
 */
local_internal void
test_push_struct_macro()
{
    typedef struct test_struct
    {
        u64 a;
        u32 b;
        u16 c;
    } test_struct;

    mem_arena *arena = arena_create(MiB(1));
    test_struct *s = PUSH_STRUCT(arena, test_struct);
    test(s != NULL);
    test(s->a == 0);
    test(s->b == 0);
    test(s->c == 0);
    arena_destroy(arena);
}

/*
 * Test: PUSH_ARRAY macro works
 */
local_internal void
test_push_array_macro()
{
    mem_arena *arena = arena_create(MiB(1));
    u32 *arr = PUSH_ARRAY(arena, u32, 10);
    test(arr != NULL);
    for (int i = 0; i < 10; i++)
    {
        test(arr[i] == 0);
    }
    arena_destroy(arena);
}

/*
 * Test: arena_pop reduces position
 */
local_internal void
test_arena_pop()
{
    mem_arena *arena = arena_create(MiB(1));
    arena_push(arena, 256, 0);
    u64 pos_before = arena->pos;
    arena_pop(arena, 128);
    test(arena->pos == pos_before - 128);
    arena_destroy(arena);
}

/*
 * Test: arena_pop doesn't go below initial position
 */
local_internal void
test_arena_pop_safety()
{
    mem_arena *arena = arena_create(MiB(1));
    u64 initial_pos = arena->pos;
    arena_pop(arena, MiB(10)); // Try to pop more than allocated
    test(arena->pos == initial_pos);
    arena_destroy(arena);
}

/*
 * Test: arena_pop_to sets position correctly
 */
local_internal void
test_arena_pop_to()
{
    mem_arena *arena = arena_create(MiB(1));
    arena_push(arena, 512, 0);
    u64 target_pos = arena->pos;
    arena_push(arena, 256, 0);
    arena_pop_to(arena, target_pos);
    test(arena->pos == target_pos);
    arena_destroy(arena);
}

/*
 * Test: arena_clear resets arena
 */
local_internal void
test_arena_clear()
{
    mem_arena *arena = arena_create(MiB(1));
    u64 initial_pos = arena->pos;
    arena_push(arena, 1024, 0);
    arena_push(arena, 2048, 0);
    arena_clear(arena);
    test(arena->pos == initial_pos);
    arena_destroy(arena);
}

/*
 * Test: temp_arena_begin captures state
 */
local_internal void
test_temp_arena_begin()
{
    mem_arena *arena = arena_create(MiB(1));
    arena_push(arena, 256, 0);
    temp_arena temp = temp_arena_begin(arena);
    test(temp.arena == arena);
    test(temp.offset == arena->pos);
    arena_destroy(arena);
}

/*
 * Test: temp_arena_end restores state
 */
local_internal void
test_temp_arena_end()
{
    mem_arena *arena = arena_create(MiB(1));
    arena_push(arena, 256, 0);
    temp_arena temp = temp_arena_begin(arena);
    u64 saved_pos = arena->pos;
    arena_push(arena, 512, 0);
    arena_push(arena, 1024, 0);
    temp_arena_end(temp);
    test(arena->pos == saved_pos);
    arena_destroy(arena);
}

/*
 * Test: temp_arena nesting works
 */
local_internal void
test_temp_arena_nesting()
{
    mem_arena *arena = arena_create(MiB(1));
    u64 pos0 = arena->pos;
    arena_push(arena, 128, 0);
    temp_arena temp1 = temp_arena_begin(arena);
    u64 pos1 = arena->pos;
    arena_push(arena, 256, 0);
    temp_arena temp2 = temp_arena_begin(arena);
    u64 pos2 = arena->pos;
    arena_push(arena, 512, 0);
    temp_arena_end(temp2);
    test(arena->pos == pos2);
    temp_arena_end(temp1);
    test(arena->pos == pos1);
    arena_destroy(arena);
}

/*
 * Test: arena can be reused after clear
 */
local_internal void
test_arena_reuse()
{
    mem_arena *arena = arena_create(MiB(1));
    u64 *ptr1 = PUSH_STRUCT(arena, u64);
    *ptr1 = 42;
    arena_clear(arena);
    u64 *ptr2 = PUSH_STRUCT(arena, u64);
    *ptr2 = 84;
    test(*ptr2 == 84);
    arena_destroy(arena);
}

/*
 * Test: large allocation works
 */
local_internal void
test_arena_large_allocation()
{
    mem_arena *arena = arena_create(MiB(10));
    void *ptr = arena_push(arena, MiB(5), 0);
    test(ptr != NULL);
    arena_destroy(arena);
}

int
main(void)
{
    test_arena_create();
    test_arena_capacity();
    test_arena_initial_pos();
    test_arena_push_returns_valid();
    test_arena_push_zeros_memory();
    test_arena_push_non_zero();
    test_arena_push_updates_pos();
    test_arena_push_alignment();
    test_arena_push_no_overlap();
    test_push_struct_macro();
    test_push_array_macro();
    test_arena_pop();
    test_arena_pop_safety();
    test_arena_pop_to();
    test_arena_clear();
    test_temp_arena_begin();
    test_temp_arena_end();
    test_temp_arena_nesting();
    test_arena_reuse();
    test_arena_large_allocation();
    return 0;
}
