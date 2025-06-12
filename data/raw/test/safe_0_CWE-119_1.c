
/*
 * File: safe_0_CWE-119_1.c
 * Description: Safe handling for buffer copy.
 * CWE-119: Proper restriction of operations within memory buffer bounds
 */

#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define BUFFER_SIZE 64

void safeHandler_1(const char* input, uint8_t len) {
    char buffer[BUFFER_SIZE];
    if (len < BUFFER_SIZE) {
        memcpy(buffer, input, len);
        buffer[len] = '\0';
    } else {
        memcpy(buffer, input, BUFFER_SIZE - 1);
        buffer[BUFFER_SIZE - 1] = '\0';
    }
    printf("Safe Output: %s\n", buffer);
}
