#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"

// Create monoalphabetic key map from key image
void generate_key_map(uint8_t *key_img, uint8_t *key_map, int size) {
    int used[256] = {0};
    int idx = 0;

    for (int i = 0; i < size && idx < 256; i++) {
        if (!used[key_img[i]]) {
            key_map[idx++] = key_img[i];
            used[key_img[i]] = 1;
        }
    }

    for (int i = 0; i < 256; i++) {
        if (!used[i]) {
            key_map[idx++] = i;
            used[i] = 1;
        }
    }
}

// Invert the key map
void invert_key_map(uint8_t *key_map, uint8_t *inv_key_map) {
    for (int i = 0; i < 256; i++) {
        inv_key_map[key_map[i]] = i;
    }
}

// Encrypt using monoalphabetic cipher
void encrypt_image(uint8_t *input, uint8_t *output, uint8_t *key_map, int size) {
    for (int i = 0; i < size; i++) {
        output[i] = key_map[input[i]];
    }
}

// Decrypt using inverse key map
void decrypt_image(uint8_t *input, uint8_t *output, uint8_t *inv_key_map, int size) {
    for (int i = 0; i < size; i++) {
        output[i] = inv_key_map[input[i]];
    }
}

// Utility to print matrix
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
    int w1, h1, ch1, w2, h2, ch2;

    uint8_t *msg_img = stbi_load("../images/input1.bmp", &w1, &h1, &ch1, 1);
    if (!msg_img) {
        printf(" Failed to load input1.bmp\n");
        return 1;
    }

    uint8_t *key_img = stbi_load("../images/input2.bmp", &w2, &h2, &ch2, 1);
    if (!key_img) {
        printf(" Failed to load input2.bmp\n");
        stbi_image_free(msg_img);
        return 1;
    }

    if (w1 != w2 || h1 != h2) {
        printf(" Images must be the same size!\n");
        stbi_image_free(msg_img);
        stbi_image_free(key_img);
        return 1;
    }

    int size = w1 * h1;

    // Generate key map
    uint8_t key_map[256], inv_key_map[256];
    generate_key_map(key_img, key_map, size);
    invert_key_map(key_map, inv_key_map);

    // Show matrices
    printf("\nKey Image Data (First 10x10 pixels):\n");
    for (int i = 0; i < 10 && i < h2; i++) {
        for (int j = 0; j < 10 && j < w2; j++) {
            printf("%3d ", key_img[i * w2 + j]);
        }
        printf("\n");
    }

    print_matrix(msg_img, w1, h1, "Original Input Image Matrix");

    // Encrypt
    uint8_t *encrypted = malloc(size);
    encrypt_image(msg_img, encrypted, key_map, size);
    stbi_write_bmp("../images/encrypted_monoalphabetic.bmp", w1, h1, 1, encrypted);
    print_matrix(encrypted, w1, h1, "Encrypted Image Matrix");

    // Decrypt
    uint8_t *decrypted = malloc(size);
    decrypt_image(encrypted, decrypted, inv_key_map, size);
    stbi_write_bmp("../images/decrypted_monoalphabetic.bmp", w1, h1, 1, decrypted);
    print_matrix(decrypted, w1, h1, "Decrypted Image Matrix");

    // Cleanup
    stbi_image_free(msg_img);
    stbi_image_free(key_img);
    free(encrypted);
    free(decrypted);

    printf("\n Monoalphabetic encryption and decryption complete.\n");
    return 0;
}
