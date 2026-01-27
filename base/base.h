#ifndef BASE_H
#define BASE_H

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

/* assert an expression and output the file and the line */

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

/* TODO(nasr): remove the stdio dep? */

#define test(expr) \
    do \
    { \
        if (!(expr)) \
        { \
            fprintf(stderr, RED " [FAILED] %s:%d: expr:%s test:%s\n" RESET, __FILE__, __LINE__, #expr, __func__); \
            _exit(0); \
        } \
        else \
        { \
            fprintf(stdout, GREEN "[PASSED] %s\n" RESET, __func__); \
        } \
    } while (0)

#define check(expr) \
    if (!(expr)) \
        fprintf(stderr, RED " [ERROR] %s:%d: expr:%s test:%s\n" RESET, __FILE__, __LINE__, #expr, __func__); \
    _exit(1);

#define global_variable static
#define local_persist static
#define local_internal static

#define ERR_OK 0
#define ERR_IO 1
#define ERR_PARSE 2
#define ERR_PERM 3
#define ERR_INVALID 4
#define null NULL

#define KiB(n) (((u64)(n)) << 10)
#define MiB(n) (((u64)(n)) << 20)
#define GiB(n) (((u64)(n)) << 30)

#define unused(x) (void)(x)

#define BUFFER_SIZE_SMALL 128
#define BUFFER_SIZE_DEFAULT 256
#define BUFFER_SIZE_LARGE 512
#define PATH_MAX_LEN 4096

#define DEPRECATED __attribute__((__deprecated__))

#define MemCpy(dest, src, len)  memcpy(dest, src, len)

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float  f32;
typedef double f64;

typedef i32 b32;
typedef i16 b16;
typedef u8  b8;

typedef uintptr_t umm;
typedef intptr_t  smm;

#define TRUE 1
#define FALSE 0

local_internal u64
parse_u64(char *buf, size_t len);

local_internal b8
is_numeric(char *s);


local_internal inline u64
align_up_pow(u64 n, u64 p);

#endif
