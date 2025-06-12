
/*
 * File: vuln_1_CWE_787_2.c
 * Description: Unsafe writing to buffer without checking bounds.
 * CWE-787: Out-of-bounds Write - Vulnerable Version
 */

#include <stdint.h>
#include <stdio.h>

#define ACTUATOR_BUFFER_SIZE 32

void updateActuatorVuln_2(uint8_t index, uint8_t value) {
    uint8_t actuator_buffer[ACTUATOR_BUFFER_SIZE];
    actuator_buffer[index] = value;  // ❌ Vulnerability: no bounds check
    printf("Actuator updated (vulnerable).\n");
}
