
/*
 * File: CanDiagHandler.c
 * Description: Handles CAN diagnostics and control commands for Powertrain Module
 * Origin: Jaguar Land Rover Embedded Software Team
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CAN_PAYLOAD 8
#define DTC_LOG_SIZE 64
#define MODULE_ID 0x1A

typedef enum {
    CMD_NONE = 0x00,
    CMD_READ_DTC = 0x01,
    CMD_WRITE_CONF = 0x02,
    CMD_RESET_ECU = 0x03
} DiagCommand;

typedef struct {
    uint8_t message_id;
    uint8_t payload[MAX_CAN_PAYLOAD];
    uint8_t length;
} CanMessage;

static char dtc_log[DTC_LOG_SIZE];
static uint8_t dtc_index = 0;

/* Unsafe: No bounds check on length */
void handleIncomingCanUnsafe(const CanMessage* msg) {
    if (!msg) return;

    if (msg->length > 0) {
        memcpy(dtc_log + dtc_index, msg->payload, msg->length);  // POTENTIAL CWE-119
        dtc_index += msg->length;
    }
}

/* Safe: Bounds checked */
void handleIncomingCanSafe(const CanMessage* msg) {
    if (!msg || msg->length == 0 || dtc_index >= DTC_LOG_SIZE) return;

    uint8_t space_remaining = DTC_LOG_SIZE - dtc_index;
    uint8_t to_copy = (msg->length < space_remaining) ? msg->length : space_remaining;
    memcpy(dtc_log + dtc_index, msg->payload, to_copy);
    dtc_index += to_copy;
}

void processDiagnosticCommand(DiagCommand cmd, const CanMessage* msg) {
    switch (cmd) {
        case CMD_READ_DTC:
            handleIncomingCanSafe(msg);
            break;
        case CMD_WRITE_CONF:
            handleIncomingCanUnsafe(msg);
            break;
        case CMD_RESET_ECU:
            dtc_index = 0;
            memset(dtc_log, 0, DTC_LOG_SIZE);
            break;
        default:
            break;
    }
}
