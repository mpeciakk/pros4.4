#ifndef PROS_LOG_HPP
#define PROS_LOG_HPP

#include <stdarg.h>

void printf(const char* __restrict format, va_list parameters, bool serial = false);

void log(int level, const char* __restrict format, ...);
void __log(const char* __restrict format, ...);
void klog(int level, const char* __restrict format, ...);

[[noreturn]] void panic(const char* message);

#endif
