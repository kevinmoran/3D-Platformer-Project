#include "string_functions.h"

size_t string_length(const char* s)
{
    const char* c = s;
    while(*c) ++c;

    return c - s;
}

bool strings_are_equal(const char *s1, const char *s2)
{
    while(*s1 == *s2++) 
        if(*s1++ == 0) return true;

    return false;
}

void concat_strings_safe(const char *s1, const char *s2, char *dest, size_t dest_length)
{
    size_t count = 0;
    while(*s1 && (count < dest_length - 1)){
        *dest++ = *s1++;
        ++count;
    }
    while(*s2 && (count < dest_length - 1)){
        *dest++ = *s2++;
        ++count;
    }
    *dest = '\0';
}

void concat_strings(const char *s1, const char *s2, char *dest)
{
    while(*s1){
        *dest++ = *s1++;
    }
    while(*s2){
        *dest++ = *s2++;
    }
    *dest = '\0';
}

void copy_string(const char* src, char* dest, size_t dest_length)
{
    size_t count = 0;
    while(src && (count < dest_length - 1)){
        *dest++ = *src++;
        ++count;
    }
    *dest = '\0';
}
