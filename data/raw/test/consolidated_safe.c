
/*
 * File: consolidated_safe.c
 * Description: Secure implementations corresponding to each vulnerability.
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

// CWE-119: Safe memcpy with bounds check
void diagnosticHandler_safe(const char* input, uint8_t len) {
    char buffer[BUFFER_SIZE];
    if (len < BUFFER_SIZE) {
        memcpy(buffer, input, len);
        buffer[len] = '\0';
    } else {
        memcpy(buffer, input, BUFFER_SIZE - 1);
        buffer[BUFFER_SIZE - 1] = '\0';
    }
    printf("CWE-119 safe: %s\n", buffer);
}

// CWE-122: Check size before heap use
void heapHandler_safe(const char* input, size_t len) {
    if (len == 0 || len > 256) return;
    char* buffer = (char*)malloc(len + 1);
    if (!buffer) return;
    memcpy(buffer, input, len);
    buffer[len] = '\0';
    printf("CWE-122 safe: %s\n", buffer);
    free(buffer);
}

// CWE-124: Validate offset range
void logWriter_safe(const char* data, int offset, int len) {
    char log[LOG_SIZE];
    if (offset >= 0 && offset + len <= LOG_SIZE) {
        memcpy(&log[offset], data, len);
        printf("CWE-124 safe: log write done\n");
    }
}

// CWE-787: Bounds check before writing
void actuatorUpdater_safe(uint8_t index, uint8_t value) {
    uint8_t buffer[ACTUATOR_BUFFER_SIZE];
    if (index < ACTUATOR_BUFFER_SIZE) {
        buffer[index] = value;
        printf("CWE-787 safe: write done\n");
    }
}

// CWE-788: Bounds check before reading
uint8_t sensorReader_safe(uint8_t* frame, uint8_t index) {
    if (index < SENSOR_FRAME_SIZE) {
        return frame[index];
    } else {
        printf("CWE-788 safe: index out of bounds\n");
        return 0;
    }
}
