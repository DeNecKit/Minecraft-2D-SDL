#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>
#include <assert.h>
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define ERROR_EXIT(...) do { \
    fprintf(stderr, "ERROR: " __VA_ARGS__); \
    exit(1); \
} while (0)

#define UNIMPLEMENTED() assert(0 && "Unimplemented")

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

#define COLOR(clr_int) (SDL_Color){ \
    clr_int >> 24, \
    (clr_int & 0xff0000) >> 16, \
    (clr_int & 0xff00) >> 8, \
    clr_int & 0xff }

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

typedef float    f32;
typedef double   f64;

typedef struct { s32 x, y; } vec2;
typedef struct { f32 x, y; } vec2f;
typedef struct { u8  x, y; } vec2b;

#endif // UTIL_H
