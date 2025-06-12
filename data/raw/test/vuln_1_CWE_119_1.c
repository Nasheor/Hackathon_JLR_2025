
/*
 * File: vuln_1_CWE_119_1.c
 * Description: Vulnerable buffer handling without bounds validation.
 * CWE-119: Improper restriction of operations within memory buffer bounds
 */

#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define BUFFER_SIZE 64

void vulnHandler_1(const char* input, uint8_t len) {
    char buffer[BUFFER_SIZE];
    memcpy(buffer, input, len);  // ❌ No bounds check
    buffer[len] = '\0';
    printf("Vulnerable Output: %s\n", buffer);
}
