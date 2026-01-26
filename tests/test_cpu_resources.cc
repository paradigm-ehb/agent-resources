#include "base/base.h"
#include "base/base_arena.h"
#include "libres/resources.h"

/* unity build */
#include "base/base.c"
#include "base/base_arena.c"
#include "libres/resources.cc"

/*
 * Test: cpu_create returns a valid pointer
 */
local_internal void
test_cpu_create()
{
    mem_arena *arena = arena_create(MiB(1));
    cpu       *c     = cpu_create(arena);


    test(c != NULL);
}

/*
 * Test: cpu_read returns ERR_OK
 */
local_internal void
test_cpu_read_returns_ok()
{
    mem_arena *arena = arena_create(MiB(4));
    cpu       *c     = cpu_create(arena);

    test(cpu_read(c) != ERR_OK);
}

/*
 * Test: CPU model string is populated
 */
local_internal void
test_cpu_model_present()
{
    mem_arena *arena = arena_create(MiB(4));
    cpu       *c     = cpu_create(arena);

    cpu_read(c);

    test(c->model[0] != '\0');
}

/*
 * Test: CPU core count is > 0
 */
local_internal void
test_cpu_cores_positive()
{
    mem_arena *arena = arena_create(MiB(4));
    cpu       *c     = cpu_create(arena);

    cpu_read(c);

    test(c->cores > 0);
}

/*
 * Test: CPU frequency string is populated
 */
local_internal void
test_cpu_frequency_present()
{
    mem_arena *arena = arena_create(MiB(4));
    cpu       *c     = cpu_create(arena);

    cpu_read(c);

    test(c->frequency[0] != '\0');
}

/*
 * Test: cpu_read is idempotent for static fields
 */
local_internal void
test_cpu_read_idempotent()
{
    mem_arena *arena = arena_create(MiB(4));
    cpu       *c     = cpu_create(arena);

    cpu_read(c);

    char model_copy[sizeof(c->model)];
    memcpy(model_copy, c->model, sizeof(c->model));

    cpu_read(c);

    test(strcmp(c->model, model_copy) == 0);
}

/*
 * Test: cpu_read(NULL) returns ERR_INVALID (debug only)
 */
local_internal void
test_cpu_read_null()
{
#ifdef DEBUG
    test(cpu_read(NULL) == ERR_INVALID);
#endif
}

/*
 * Test: CPU vendor present on x86
 */
local_internal void
test_cpu_vendor_present()
{
#if defined(__i386__) || defined(__x86_64__)
    mem_arena *arena = arena_create(MiB(4));
    cpu       *c     = cpu_create(arena);

    cpu_read(c);

    test(c->vendor[0] != '\0');
#endif
}

int
main(void)
{
  test_cpu_create();
  test_cpu_read_returns_ok();
  test_cpu_model_present();
  test_cpu_vendor_present();
  test_cpu_cores_positive();
  test_cpu_frequency_present();
  test_cpu_read_idempotent();
  test_cpu_read_null();

    return 0;
}
