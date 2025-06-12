
/*
 * File: vuln_1_CWE_122_1.c
 * Description: Vulnerable heap buffer usage without bounds checking.
 * CWE-122: Heap-Based Buffer Overflow - Vulnerable Version
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

void handlePacketVuln_1(const char* input, size_t len) {
    char* buffer = (char*)malloc(64);  // may not be enough
    if (!buffer) return;

    memcpy(buffer, input, len);  // ❌ No validation of length
    buffer[len] = '\0';         // ❌ Possible write out of bounds
    printf("Vulnerable Output: %s\n", buffer);
    free(buffer);
}
