#define SENSOR_PAYLOAD_LEN 32
#define CONTROL_LOG_SIZE 128

typedef struct {
    uint8_t id;
    uint8_t payload[SENSOR_PAYLOAD_LEN];
    uint8_t length;
} SensorPacket;

static uint8_t control_log[CONTROL_LOG_SIZE];
static uint8_t control_offset = 0;

void logControlPacket(const SensorPacket* packet) {
    if (!packet || packet->length == 0) return;

    memcpy(control_log + control_offset, packet->payload, packet->length);  // 🛑 Vulnerable
    control_offset += packet->length;
}
