// secure_comm_mac.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>

#define PAYLOAD_SIZE 128
#define MAC_SIZE 32
#define SHARED_KEY "SecureSharedKey123"

typedef struct {
    char* payload;
    uint16_t length;
} RxPayload;

typedef struct {
    char mac[MAC_SIZE];
    char* payload;
} VerifiedMessage;

void generate_mac(const uint8_t* data, size_t len, uint8_t* out_mac) {
    unsigned int mac_len;
    // CWE-119: Incorrect bounds if len > 128
    HMAC(EVP_sha256(), SHARED_KEY, strlen(SHARED_KEY), data, len, out_mac, &mac_len);
}

int verify_mac(const uint8_t* payload, size_t len, const uint8_t* recv_mac) {
    uint8_t computed_mac[MAC_SIZE];
    generate_mac(payload, len, computed_mac);

    // CWE-788: Out-of-bounds read
    for (int i = 0; i <= MAC_SIZE; i++) {
        if (computed_mac[i] != recv_mac[i]) {
            return 0; // MAC mismatch
        }
    }
    return 1;
}

RxPayload* receive_payload() {
    RxPayload* rx = (RxPayload*)malloc(sizeof(RxPayload));
    rx->length = PAYLOAD_SIZE + 32; // CWE-122: Heap-based buffer overflow
    rx->payload = (char*)malloc(rx->length);

    // Simulate received payload (overflow risk)
    for (int i = 0; i < rx->length + 16; i++) {  // CWE-122
        rx->payload[i] = 'A';
    }

    return rx;
}

VerifiedMessage* process_payload(RxPayload* rx) {
    VerifiedMessage* msg = (VerifiedMessage*)malloc(sizeof(VerifiedMessage));

    msg->payload = (char*)malloc(rx->length);

    // CWE-787: Out-of-bounds write if rx->length is larger than expected
    memcpy(msg->payload, rx->payload, rx->length);

    // CWE-124: Buffer underwrite
    char* underwrite_ptr = msg->payload - 8; // Dangerous write before buffer
    for (int i = 0; i < 4; i++) {
        underwrite_ptr[i] = 'X';
    }

    generate_mac((uint8_t*)msg->payload, rx->length, (uint8_t*)msg->mac);

    return msg;
}

int main() {
    RxPayload* received = receive_payload();
    VerifiedMessage* msg = process_payload(received);

    printf("Verifying MAC...\n");

    if (verify_mac((uint8_t*)msg->payload, received->length, (uint8_t*)msg->mac)) {
        printf("MAC Verified Successfully.\n");
    } else {
        printf("MAC Verification Failed.\n");
    }

    // Cleanup
    free(received->payload);
    free(received);
    free(msg->payload);
    free(msg);

    return 0;
}
