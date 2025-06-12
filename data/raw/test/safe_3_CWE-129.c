
/*
 * File: safe_3_CWE-129.c
 * Description: Safe handling of CAN message index mapping.
 * CWE-129: Proper Validation of Array Index
 */

#include <stdint.h>
#include <stdio.h>

#define NUM_HANDLERS 5
#define CAN_MSG_BASE 0x100

typedef void (*HandlerFunc)(const uint8_t*, uint8_t);

void handleRPM(const uint8_t* data, uint8_t len) { printf("RPM OK\n"); }
void handleSpeed(const uint8_t* data, uint8_t len) { printf("Speed OK\n"); }
void handlePressure(const uint8_t* data, uint8_t len) { printf("Pressure OK\n"); }
void handleSteering(const uint8_t* data, uint8_t len) { printf("Steering OK\n"); }
void handleDoor(const uint8_t* data, uint8_t len) { printf("Door OK\n"); }

HandlerFunc handlers[NUM_HANDLERS] = {
    handleRPM, handleSpeed, handlePressure, handleSteering, handleDoor
};

void processMessage(uint16_t msg_id, const uint8_t* data, uint8_t len) {
    if (msg_id >= CAN_MSG_BASE && msg_id < CAN_MSG_BASE + NUM_HANDLERS) {
        uint16_t index = msg_id - CAN_MSG_BASE;
        if (index < NUM_HANDLERS) {
            handlers[index](data, len);
        }
    }
}
