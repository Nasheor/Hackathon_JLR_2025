
/*
 * File: consolidated_vuln.c
 * Description: Contains multiple vulnerable functions across different CWEs.
 * CWE-119, CWE-122, CWE-124, CWE-787, CWE-788
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 64
#define HEAP_SIZE 64
#define LOG_SIZE 64
#define ACTUATOR_BUFFER_SIZE 32
#define SENSOR_FRAME_SIZE 32

// CWE-119: No bounds check on memcpy
void diagnosticHandler_vuln(const char* input, uint8_t len) {
    char buffer[BUFFER_SIZE];
    memcpy(buffer, input, len);  // ❌
    buffer[len] = '\0';
    printf("CWE-119 vulnerable: %s\n", buffer);
}

// CWE-122: Unsafe heap usage without checking len
void heapHandler_vuln(const char* input, size_t len) {
    char* buffer = (char*)malloc(HEAP_SIZE);
    if (!buffer) return;
    memcpy(buffer, input, len);  // ❌
    buffer[len] = '\0';
    printf("CWE-122 vulnerable: %s\n", buffer);
    free(buffer);
}

// CWE-124: No validation on negative offset
void logWriter_vuln(const char* data, int offset, int len) {
    char log[LOG_SIZE];
    memcpy(&log[offset], data, len);  // ❌
    printf("CWE-124 vulnerable: log write done\n");
}

// CWE-787: No bounds check before write
void actuatorUpdater_vuln(uint8_t index, uint8_t value) {
    uint8_t buffer[ACTUATOR_BUFFER_SIZE];
    buffer[index] = value;  // ❌
    printf("CWE-787 vulnerable: write done\n");
}

// CWE-788: No bounds check before read
uint8_t sensorReader_vuln(uint8_t* frame, uint8_t index) {
    return frame[index];  // ❌
}
