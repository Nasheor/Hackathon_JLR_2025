
/*
 * File: safe_0_CWE-122_1.c
 * Description: Safe dynamic memory allocation with proper bounds checking.
 * CWE-122: Heap-Based Buffer Overflow - Fixed Version
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

void handlePacketSafe_1(const char* input, size_t len) {
    if (len == 0 || len > 256) return; // enforce upper bound

    char* buffer = (char*)malloc(len + 1);
    if (!buffer) return;

    memcpy(buffer, input, len);
    buffer[len] = '\0';  // null-terminate safely
    printf("Safe Output: %s\n", buffer);
    free(buffer);
}
