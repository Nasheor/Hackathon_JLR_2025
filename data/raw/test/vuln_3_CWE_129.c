
/*
 * File: vuln_3_CWE_129.c
 * Description: Vulnerable CAN handler index without bounds check.
 * CWE-129: Improper Validation of Array Index
 */

#include <stdint.h>
#include <stdio.h>

#define NUM_HANDLERS 5
#define CAN_MSG_BASE 0x100

typedef void (*HandlerFunc)(const uint8_t*, uint8_t);

void handleRPM(const uint8_t* data, uint8_t len) { printf("RPM\n"); }
void handleSpeed(const uint8_t* data, uint8_t len) { printf("Speed\n"); }
void handlePressure(const uint8_t* data, uint8_t len) { printf("Pressure\n"); }
void handleSteering(const uint8_t* data, uint8_t len) { printf("Steering\n"); }
void handleDoor(const uint8_t* data, uint8_t len) { printf("Door\n"); }

HandlerFunc handlers[NUM_HANDLERS] = {
    handleRPM, handleSpeed, handlePressure, handleSteering, handleDoor
};

void processMessage(uint16_t msg_id, const uint8_t* data, uint8_t len) {
    uint16_t index = msg_id - CAN_MSG_BASE;
    handlers[index](data, len); // ❌ Vulnerable: no bounds check on index
}
