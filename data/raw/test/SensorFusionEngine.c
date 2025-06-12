
/*
 * File: SensorFusionEngine.c
 * Description: Processes multi-sensor data from ADAS system for lane assist and collision avoidance.
 * Context: ADAS Sensor Fusion Core Loop - Vulnerable Revision
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define SENSOR_BUFFER_SIZE 64
#define MAX_SENSORS 8
#define SENSOR_ID_LEN 4

typedef struct {
    uint8_t sensor_id[SENSOR_ID_LEN];
    uint8_t data[16];
    uint8_t data_len;
} SensorPacket;

static uint8_t fusion_buffer[SENSOR_BUFFER_SIZE];
static SensorPacket sensor_inputs[MAX_SENSORS];

/* CWE-788: Out-of-bounds read - index not validated */
uint8_t readSensorIDByte(uint8_t sensor_index, uint8_t byte_index) {
    return sensor_inputs[sensor_index].sensor_id[byte_index];  // Vulnerable: no bounds check
}

/* CWE-787: Out-of-bounds write - offset unchecked */
void writeToFusionBuffer(const SensorPacket* packet, uint8_t offset) {
    if (!packet) return;

    memcpy(fusion_buffer + offset, packet->data, packet->data_len);  // Vulnerable: offset + length not validated
}

void processSensors() {
    for (uint8_t i = 0; i < MAX_SENSORS; ++i) {
        if (sensor_inputs[i].data_len > 0) {
            writeToFusionBuffer(&sensor_inputs[i], i * 10);
        }
    }

    // Simulated debug access to sensor ID
    uint8_t id_byte = readSensorIDByte(10, 2);  // Invalid sensor index
}
