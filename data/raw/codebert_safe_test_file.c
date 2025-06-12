#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>

#define PAYLOAD_SIZE 128
#define MAC_SIZE 32
#define MAX_PAYLOAD_SIZE 256
#define SHARED_KEY_FILE "shared_key.secure"

typedef struct {
    char* payload;
    uint16_t length;
} RxPayload;

typedef struct {
    uint8_t mac[MAC_SIZE];
    char* payload;
} VerifiedMessage;

// Read shared key from a secure source (simulated as file)
int read_shared_key(uint8_t* key_buf, size_t* key_len) {
    FILE* fp = fopen(SHARED_KEY_FILE, "rb");
    if (!fp) {
        perror("Failed to open key file");
        return 0;
    }

    *key_len = fread(key_buf, 1, 64, fp);  // Max key size
    fclose(fp);

    if (*key_len == 0) {
        fprintf(stderr, "Key file is empty\n");
        return 0;
    }

    return 1;
}

int generate_mac(const uint8_t* data, size_t len, uint8_t* out_mac, const uint8_t* key, size_t key_len) {
    unsigned int mac_len = 0;
    if (!HMAC(EVP_sha256(), key, key_len, data, len, out_mac, &mac_len)) {
        fprintf(stderr, "HMAC generation failed\n");
        return 0;
    }
    return 1;
}

int verify_mac(const uint8_t* payload, size_t len, const uint8_t* recv_mac, const uint8_t* key, size_t key_len) {
    uint8_t computed_mac[MAC_SIZE];
    if (!generate_mac(payload, len, computed_mac, key, key_len)) {
        return 0;
    }

    // Constant-time comparison to avoid timing attacks
    return CRYPTO_memcmp(computed_mac, recv_mac, MAC_SIZE) == 0;
}

RxPayload* receive_payload() {
    RxPayload* rx = (RxPayload*)malloc(sizeof(RxPayload));
    if (!rx) return NULL;

    rx->length = PAYLOAD_SIZE;

    if (rx->length > MAX_PAYLOAD_SIZE) {
        free(rx);
        return NULL;
    }

    rx->payload = (char*)calloc(rx->length, 1);
    if (!rx->payload) {
        free(rx);
        return NULL;
    }

    // Simulate safe data reception
    for (int i = 0; i < rx->length; i++) {
        rx->payload[i] = 'A';
    }

    return rx;
}

VerifiedMessage* process_payload(RxPayload* rx, const uint8_t* key, size_t key_len) {
    if (!rx || !rx->payload || rx->length > MAX_PAYLOAD_SIZE)
        return NULL;

    VerifiedMessage* msg = (VerifiedMessage*)malloc(sizeof(VerifiedMessage));
    if (!msg) return NULL;

    msg->payload = (char*)calloc(rx->length, 1);
    if (!msg->payload) {
        free(msg);
        return NULL;
    }

    memcpy(msg->payload, rx->payload, rx->length);

    if (!generate_mac((uint8_t*)msg->payload, rx->length, msg->mac, key, key_len)) {
        free(msg->payload);
        free(msg);
        return NULL;
    }

    return msg;
}

int main() {
    uint8_t shared_key[64];
    size_t key_len = 0;

    if (!read_shared_key(shared_key, &key_len)) {
        fprintf(stderr, "Failed to load shared key\n");
        return EXIT_FAILURE;
    }

    RxPayload* received = receive_payload();
    if (!received) {
        fprintf(stderr, "Payload reception failed\n");
        return EXIT_FAILURE;
    }

    VerifiedMessage* msg = process_payload(received, shared_key, key_len);
    if (!msg) {
        fprintf(stderr, "Payload processing failed\n");
        free(received->payload);
        free(received);
        return EXIT_FAILURE;
    }

    printf("Verifying MAC...\n");

    if (verify_mac((uint8_t*)msg->payload, received->length, msg->mac, shared_key, key_len)) {
        printf("MAC Verified Successfully.\n");
    } else {
        printf("MAC Verification Failed.\n");
    }

    // Cleanup
    free(received->payload);
    free(received);
    free(msg->payload);
    free(msg);

    return EXIT_SUCCESS;
}
