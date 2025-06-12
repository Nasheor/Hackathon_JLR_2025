
/*
 * File: safe_0_CWE-787_2.c
 * Description: Safe writing to buffer with bounds check.
 * CWE-787: Out-of-bounds Write - Fixed Version
 */

#include <stdint.h>
#include <stdio.h>

#define ACTUATOR_BUFFER_SIZE 32

void updateActuatorSafe_2(uint8_t index, uint8_t value) {
    uint8_t actuator_buffer[ACTUATOR_BUFFER_SIZE];
    if (index < ACTUATOR_BUFFER_SIZE) {
        actuator_buffer[index] = value;
        printf("Actuator updated safely.\n");
    } else {
        printf("Invalid index, skipping write.\n");
    }
}
