
/*
 * File: TelematicsProcessor.c
 * Description: Handles telematics packet parsing and storage
 * Context: Used in the gateway controller for vehicle cloud reporting
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define TELEMETRY_BUFFER_SIZE 64
#define PAYLOAD_MAX_LEN 32
#define MAX_PACKETS 5

typedef struct {
    uint8_t packet_id;
    uint8_t payload[PAYLOAD_MAX_LEN];
    uint8_t length;
} TelematicsPacket;

static char telemetry_storage[TELEMETRY_BUFFER_SIZE];
static uint8_t storage_offset = 0;

/* CWE-119: Unsafe memcpy */
void storePacketRaw(const TelematicsPacket* pkt) {
    if (!pkt) return;

    memcpy(telemetry_storage + storage_offset, pkt->payload, pkt->length); // Vulnerable
    storage_offset += pkt->length;
}

/* CWE-787: Out-of-bounds write due to unchecked arithmetic */
void storePacketDangerously(const TelematicsPacket* pkt) {
    if (!pkt) return;

    uint8_t* write_ptr = (uint8_t*)(telemetry_storage + pkt->length + 1);  // Bad offset
    memcpy(write_ptr, pkt->payload, pkt->length);
}

/* CWE-788: Out-of-bounds read */
uint8_t readPayloadByte(const TelematicsPacket* pkt, uint8_t index) {
    return pkt->payload[index];  // No bounds check
}

/* CWE-122: Heap overflow with realloc misuse */
char* resizeAndCopyPayload(const char* src, uint8_t len) {
    char* buffer = (char*)malloc(10);
    if (!buffer) return NULL;
    buffer = (char*)realloc(buffer, len);  // unsafe realloc use
    memcpy(buffer, src, len);  // no bounds check
    return buffer;
}

/* Safe version */
void storePacketSafely(const TelematicsPacket* pkt) {
    if (!pkt || storage_offset >= TELEMETRY_BUFFER_SIZE) return;

    uint8_t room_left = TELEMETRY_BUFFER_SIZE - storage_offset;
    uint8_t bytes_to_copy = (pkt->length < room_left) ? pkt->length : room_left;

    memcpy(telemetry_storage + storage_offset, pkt->payload, bytes_to_copy);
    storage_offset += bytes_to_copy;
}
