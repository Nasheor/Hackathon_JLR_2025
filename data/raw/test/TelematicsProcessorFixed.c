
/*
 * File: TelematicsProcessorFixed.c
 * Description: Secure handling of telematics packet parsing and storage
 * Context: Gateway controller for vehicle cloud reporting (Hardened)
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

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

/* Fixed CWE-119: memcpy bounded by buffer size */
void storePacketRaw_fixed(const TelematicsPacket* pkt) {
    if (!pkt || pkt->length > PAYLOAD_MAX_LEN) return;

    uint8_t space_left = TELEMETRY_BUFFER_SIZE - storage_offset;
    uint8_t to_copy = (pkt->length < space_left) ? pkt->length : space_left;
    memcpy(telemetry_storage + storage_offset, pkt->payload, to_copy);
    storage_offset += to_copy;
}

/* Fixed CWE-787: Valid pointer calculation */
void storePacketDangerously_fixed(const TelematicsPacket* pkt) {
    if (!pkt || pkt->length > PAYLOAD_MAX_LEN || storage_offset >= TELEMETRY_BUFFER_SIZE) return;

    uint8_t offset = (pkt->length < (TELEMETRY_BUFFER_SIZE - 1)) ? pkt->length : (TELEMETRY_BUFFER_SIZE - 1);
    uint8_t* write_ptr = (uint8_t*)(telemetry_storage + offset);
    memcpy(write_ptr, pkt->payload, (offset < PAYLOAD_MAX_LEN) ? offset : PAYLOAD_MAX_LEN);
}

/* Fixed CWE-788: index bounds check */
uint8_t readPayloadByte_fixed(const TelematicsPacket* pkt, uint8_t index) {
    if (!pkt || index >= PAYLOAD_MAX_LEN) return 0;
    return pkt->payload[index];
}

/* Fixed CWE-122: safer realloc and bounds check */
char* resizeAndCopyPayload_fixed(const char* src, uint8_t len) {
    if (!src || len > 100) return NULL;  // enforce upper bound

    char* buffer = (char*)malloc(10);
    if (!buffer) return NULL;

    char* new_buf = (char*)realloc(buffer, len);
    if (!new_buf) {
        free(buffer);
        return NULL;
    }

    buffer = new_buf;
    memcpy(buffer, src, len);
    return buffer;
}

/* Safe storage handler remains unchanged */
void storePacketSafely(const TelematicsPacket* pkt) {
    if (!pkt || storage_offset >= TELEMETRY_BUFFER_SIZE) return;

    uint8_t room_left = TELEMETRY_BUFFER_SIZE - storage_offset;
    uint8_t bytes_to_copy = (pkt->length < room_left) ? pkt->length : room_left;

    memcpy(telemetry_storage + storage_offset, pkt->payload, bytes_to_copy);
    storage_offset += bytes_to_copy;
}
