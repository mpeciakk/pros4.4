#ifndef PROS_STRING_HPP
#define PROS_STRING_HPP

#include <Lib/Types.hpp>

u32 strlen(const char* str);
void* memset(void* dest, char val, u32 count);
void memcpy(void* dest, void* src, u32 size);
int strcmp(const char* a, const char* b);
char* strcpy(char* strDest, const char* strSrc);
char* strcat(char* destination, const char* source);
char* strsep(char** stringp, const char* delim);
char* strdup(const char* src);
char* itos(u32 value, char* buffer, int base);
u32 countDigit(u32 n, u32 base = 10);

#endif
