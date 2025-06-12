
/*
 * File: vuln_1_CWE_124_1.c
 * Description: Vulnerable write to memory before start of buffer.
 * CWE-124: Buffer Underwrite - Vulnerable Version
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define LOG_SIZE 64

void writeSensorLogVuln_1(const char* data, int offset, int len) {
    char log[LOG_SIZE];
    memcpy(&log[offset], data, len);  // ❌ No validation on offset — could underwrite
    printf("Log Updated (Vulnerable)\n");
}
