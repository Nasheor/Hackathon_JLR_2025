
/*
 * File: vuln_1_CWE_788_2.c
 * Description: Vulnerable read from buffer without checking bounds.
 * CWE-788: Out-of-bounds Read - Vulnerable Version
 */

#include <stdint.h>
#include <stdio.h>

#define SENSOR_FRAME_SIZE 32

uint8_t readSensorByteVuln_2(uint8_t* frame, uint8_t index) {
    return frame[index];  // ❌ Vulnerability: no bounds check
}
