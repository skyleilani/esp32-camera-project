#include "system_logger.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

void system_logger_log_message(const char *format, ...)
{
    uint8_t error_flags; // TO_DO: Not yet integrated
    uint32_t timestamp; // TO_DO: Not yet integrated
    
    va_list args;
    va_start(args, format);

    vprintf(format, args);
    va_end(args);
}

void system_log_info(const char, *format, ...){
    return;
}

void system_log_warn(const char, *format, ...){
    return;
}

void system_log_error(const char, *format, ...){
    return;
}