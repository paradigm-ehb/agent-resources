#include "base/base.h"

/**
 * Helper function to parse strings to int using ascii codes
 * */
local_internal u64
parse_u64(char *buf, size_t len)
{
    u64 value = 0;

    for (
    size_t buffer_idx = 0;
    buffer_idx < len;
    ++buffer_idx)
    {
        char c = buf[buffer_idx];
        if (c < '0' || c > '9')
        {
            break;
        }
        value = value * 10 + (c - '0');
    }

    return value;
}

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


/*
 * is_numeric - Check if a string contains only digits
 * @s: String to check
 *
 * Return: 1 if string contains only numeric characters, 0 otherwise
 */
local_internal b8
is_numeric(char *s)
{
    for (; *s; ++s)
    {
        if (*s < '0' || *s > '9')
        {
            return 0;
        }
    }
    return 1;
}

local_internal b8
compare_string(const char *c1, const char *c2)
{
    if (sizeof(c1) != sizeof(c2))
    {
        return -1;
    }

    for (
    u64 word_idx = 0;
    word_idx <= sizeof(*c1);
    ++word_idx)
    {
        if (*c1 != *c2)
        {
            return -1;
        }
        ++c1;
        ++c2;
    }

    return 0;
}
