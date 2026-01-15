#ifndef BASE_H
#define BASE_H

#include <stdint.h>
#include <stdio.h>

#define OK 0
#define ERR_IO 1
#define ERR_PARSE 2
#define ERR_PERM 3
#define ERR_INVALID 4

#define BUFFER_SIZE_SMALL 128
#define BUFFER_SIZE_DEFAULT 256
#define BUFFER_SIZE_LARGE 512
#define PATH_MAX_LEN 4096

#define internal static
#define local_persist static
#define global_variable static

#define DEPRECATED __attribute__((__deprecated__))

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef i32 b32;
typedef i16 b16;
typedef u8 b8;

#define TRUE 1
#define FALSE 0

internal u64
parse_u64(char *buf, size_t len);

internal b8
is_numeric(char *s);

internal b8
compare_string(char *c1, char *c2);

#endif
