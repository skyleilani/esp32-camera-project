#include <stdio.h>
#include <stdarg.h>

#ifndef SYSTEM_LOGGER_H
#define SYSTEM_LOGGER_H

void system_logger_log_message(const char* format, ...);

#endif // SYSTEM_LOGGER_H