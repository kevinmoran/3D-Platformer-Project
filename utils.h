#pragma once

#include <stdio.h>
#include <stdint.h>
typedef int8_t int8;
typedef int8_t i8;
typedef int16_t int16;
typedef int16_t i16;
typedef int32_t int32;
typedef int32_t i32;
typedef int64_t int64;
typedef int64_t i64;
typedef uint8_t uint8;
typedef uint8_t u8;
typedef uint16_t uint16;
typedef uint16_t u16;
typedef uint32_t uint32;
typedef uint32_t u32;
typedef uint64_t uint64;
typedef uint64_t u64;
typedef u8 bool8;
typedef u32 bool32;
typedef float f32;
typedef double f64;

#define internal static
#define global_variable static
#define local_persist static

//Custom assert function which pauses program in debugger instead of crashing
#if defined(__clang__) || defined(__GNUC__)
#define _BREAKPOINT_CALL __asm__ volatile("int $0x03")
#elif defined(_MSC_VER_)
#define _BREAKPOINT_CALL __debugbreak()
#endif

#define assert(exp) \
{if(!(exp)) { \
    printf("Assertion failed in %s, Line %d:\n%s\n...", __FILE__, __LINE__, #exp); \
	_BREAKPOINT_CALL; \
}}

void copy_memory(void *dest, void *src, size_t n)
{
    uint8* src_bytes = (uint8*)src;
    uint8* dest_bytes = (uint8*)dest;
    assert(((dest_bytes - src_bytes) > n) || ((src_bytes - dest_bytes) > n));
 
    for (uint32 i = 0; i < n; ++i)
        dest_bytes[i] = src_bytes[i];
}
