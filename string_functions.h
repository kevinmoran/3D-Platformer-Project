#pragma once

#include <stddef.h> //size_t

size_t string_length(const char* s);
bool strings_are_equal(const char *s1, const char *s2);

void concat_strings_safe(const char *s1, const char *s2, char *dest, size_t dest_length);
// NB: UNSAFE, does not check for buffer overrun. Use at your own peril!
void concat_strings(const char *s1, const char *s2, char *dest);

void copy_string(const char* src, char* dest, size_t dest_length);
