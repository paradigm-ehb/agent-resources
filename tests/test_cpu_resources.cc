#include "base/base.h"
#include "base/base_arena.h"
#include "libres/resources.h"

#include "base/base.c"
#include "base/base_arena.c"
#include "libres/resources.cc"

/*
 * Test case:
 * test if the retrieved disks aren't 0
 */
local_internal void
test_cpu_create()
{
  mem_arena *arena = arena_create(MiB(1));
  Cpu *cpu = cpu_create(arena);

  test_assert(cpu != NULL);
}

local_internal void
test_cpu_read_returns_ok()
{
  mem_arena *arena = arena_create(MiB(4));
  Cpu *cpu = cpu_create(arena);

  test_assert(cpu_read(cpu) == ERR_OK);
}

local_internal void
test_cpu_model_present()
{
  mem_arena *arena = arena_create(MiB(4));
  Cpu *cpu = cpu_create(arena);

  cpu_read(cpu);

  test_assert(cpu->model[0] != '\0');
}

local_internal void
test_cpu_cores_positive()
{
  mem_arena *arena = arena_create(MiB(4));
  Cpu *cpu = cpu_create(arena);

  cpu_read(cpu);

  test_assert(cpu->cores > 0);
}

local_internal void
test_cpu_frequency_present()
{
  mem_arena *arena = arena_create(MiB(4));
  Cpu *cpu = cpu_create(arena);

  cpu_read(cpu);

  test_assert(cpu->frequency[0] != '\0');
}

local_internal void
test_cpu_usage_read()
{
  mem_arena *arena = arena_create(MiB(4));
  Cpu *cpu = cpu_create(arena);

  test_assert(cpu_read_usage(cpu) == ERR_OK);
}

local_internal void
test_cpu_usage_values_valid()
{
  mem_arena *arena = arena_create(MiB(4));
  Cpu *cpu = cpu_create(arena);

  cpu_read_usage(cpu);

  test_assert(cpu->total_time > 0);
  test_assert(cpu->idle_time <= cpu->total_time);
}

local_internal void
test_cpu_usage_monotonic()
{
  mem_arena *arena = arena_create(MiB(4));
  Cpu *cpu = cpu_create(arena);

  cpu_read_usage(cpu);
  u64 total1 = cpu->total_time;
  u64 idle1  = cpu->idle_time;

  sleep(1);

  cpu_read_usage(cpu);
  u64 total2 = cpu->total_time;
  u64 idle2  = cpu->idle_time;

  test_assert(total2 >= total1);
  test_assert(idle2  >= idle1);
}

local_internal void
test_cpu_read_idempotent()
{
  mem_arena *arena = arena_create(MiB(4));
  Cpu *cpu = cpu_create(arena);

  cpu_read(cpu);

  char model_copy[sizeof(cpu->model)];
  memcpy(model_copy, cpu->model, sizeof(cpu->model));

  cpu_read(cpu);

  test_assert(strcmp(cpu->model, model_copy) == 0);
}

local_internal void
test_cpu_read_null()
{
#ifdef DEBUG
  test_assert(cpu_read(NULL) == ERR_INVALID);
#endif
}

local_internal void
test_cpu_vendor_present()
{
#if defined(__i386__) || defined(__x86_64__)
  mem_arena *arena = arena_create(MiB(4));
  Cpu *cpu = cpu_create(arena);

  cpu_read(cpu);

  test_assert(cpu->vendor[0] != '\0');
#endif
}

int
main()
{
  test_cpu_create();
  test_cpu_read_returns_ok();
  test_cpu_model_present();
  test_cpu_vendor_present();
  test_cpu_cores_positive();
  test_cpu_frequency_present();
  test_cpu_usage_read();
  test_cpu_usage_values_valid();
  test_cpu_usage_monotonic();
  test_cpu_read_idempotent();
  test_cpu_read_null();

  return 0;
}
