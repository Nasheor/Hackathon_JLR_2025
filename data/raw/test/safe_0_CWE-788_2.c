
/*
 * File: safe_0_CWE-788_2.c
 * Description: Safe read from buffer with bounds check.
 * CWE-788: Out-of-bounds Read - Fixed Version
 */

#include <stdint.h>
#include <stdio.h>

#define SENSOR_FRAME_SIZE 32

uint8_t readSensorByteSafe_2(uint8_t* frame, uint8_t index) {
    if (index < SENSOR_FRAME_SIZE) {
        return frame[index];
    } else {
        printf("Index out of bounds, returning 0.\n");
        return 0;
    }
}
