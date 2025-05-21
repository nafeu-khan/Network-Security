#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"

// RC4 state
typedef struct {
    uint8_t S[256];
    int i, j;
} RC4State;

// Initialize RC4 key scheduling algorithm
void rc4_init(RC4State *state, const uint8_t *key, int keylen) {
    for (int i = 0; i < 256; i++) {
        state->S[i] = i;
    }
    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + state->S[i] + key[i % keylen]) & 0xFF;
        uint8_t tmp = state->S[i];
        state->S[i] = state->S[j];
        state->S[j] = tmp;
    }
    state->i = 0;
    state->j = 0;
}

// Generate next byte of RC4 keystream
uint8_t rc4_get_byte(RC4State *state) {
    state->i = (state->i + 1) & 0xFF;
    state->j = (state->j + state->S[state->i]) & 0xFF;
    uint8_t tmp = state->S[state->i];
    state->S[state->i] = state->S[state->j];
    state->S[state->j] = tmp;
    uint8_t K = state->S[(state->S[state->i] + state->S[state->j]) & 0xFF];
    return K;
}

// Encrypt/Decrypt (same operation in RC4)
void rc4_crypt(uint8_t *input, uint8_t *output, int size, const uint8_t *key, int keylen) {
    RC4State state;
    rc4_init(&state, key, keylen);
    for (int i = 0; i < size; i++) {
        uint8_t keystream_byte = rc4_get_byte(&state);
        output[i] = input[i] ^ keystream_byte;
    }
}

// Utility to print matrix (first 10x10 pixels)
void print_matrix(uint8_t *data, int width, int height, const char *label) {
    printf("\n%s (First 10x10 pixels):\n", label);
    for (int i = 0; i < 10 && i < height; i++) {
        for (int j = 0; j < 10 && j < width; j++) {
            printf("%3d ", data[i * width + j]);
        }
        printf("\n");
    }
}

int main() {
    int w, h, ch;

    // Load grayscale image
    uint8_t *img = stbi_load("../images/input1.bmp", &w, &h, &ch, 1);
    if (!img) {
        printf(" Failed to load input1.bmp\n");
        return 1;
    }

    int size = w * h;

    // Key for RC4
    const char *key_str = "networksecuritylab";
    int keylen = strlen(key_str);
    const uint8_t *key = (const uint8_t *)key_str;

    // Show input matrix
    print_matrix(img, w, h, "Original Image Matrix");

    // Encrypt
    uint8_t *encrypted = (uint8_t *)malloc(size);
    rc4_crypt(img, encrypted, size, key, keylen);
    stbi_write_bmp("../images/encrypted_ecb_rc4.bmp", w, h, 1, encrypted);
    print_matrix(encrypted, w, h, "Encrypted Image Matrix");

    // Decrypt
    uint8_t *decrypted = (uint8_t *)malloc(size);
    rc4_crypt(encrypted, decrypted, size, key, keylen); // same function
    stbi_write_bmp("../images/decrypted_ecb_rc4.bmp", w, h, 1, decrypted);
    print_matrix(decrypted, w, h, "Decrypted Image Matrix");

    // Cleanup
    stbi_image_free(img);
    free(encrypted);
    free(decrypted);

    printf("\n ECB Mode RC4 encryption and decryption complete.\n");
    return 0;
}
