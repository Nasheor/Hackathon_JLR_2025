
/*
 * File: safe_0_CWE-124_2.c
 * Description: Safe access with pointer arithmetic for sensor log buffer.
 * CWE-124: Buffer Underwrite - Fixed Version
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define LOG_SIZE 64

void writeSensorLogSafe_2(const char* data, int offset, int len) {
    char log[LOG_SIZE];
    if (offset >= 0 && offset + len <= LOG_SIZE) {
        memcpy(&log[offset], data, len);
        printf("Log Updated Safely\n");
    } else {
        printf("Invalid offset, skipping write\n");
    }
}
