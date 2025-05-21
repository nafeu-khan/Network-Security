#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"

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

// Modified Caesar encryption
void encrypt_image(uint8_t *input, uint8_t *key, uint8_t *output, int size) {
    for (int i = 0; i < size; i++) {
        output[i] = (input[i] + key[i]) % 256;
    }
}

// Modified Caesar decryption
void decrypt_image(uint8_t *input, uint8_t *key, uint8_t *output, int size) {
    for (int i = 0; i < size; i++) {
        output[i] = (input[i] - key[i] + 256) % 256;
    }
}

int main() {
    int w1, h1, ch1;
    int w2, h2, ch2;

    // Load message image
    uint8_t *msg_img = stbi_load("../images/input1.bmp", &w1, &h1, &ch1, 1); // grayscale
    if (!msg_img) {
        printf(" Failed to load input1.bmp\n");
        return 1;
    }

    // Load key image
    uint8_t *key_img = stbi_load("../images/input2.bmp", &w2, &h2, &ch2, 1); // grayscale
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

    // Show key 
    printf("\nKey Image Dimensions: %d x %d\n", w2, h2);
    printf("Key Image Channel: %d\n", ch2);
    printf("Key Image Size: %d bytes\n", size);
    printf("Key Image Data (First 10x10 pixels):\n");
    for (int i = 0; i < 10 && i < h2; i++) {
        for (int j = 0; j < 10 && j < w2; j++) {
            printf("%3d ", key_img[i * w2 + j]);
        }
        printf("\n");
    }
    
    // Show input image matrix
    print_matrix(msg_img, w1, h1, "Original Input Image Matrix");

    // Encrypt
    uint8_t *encrypted = (uint8_t *)malloc(size);
    encrypt_image(msg_img, key_img, encrypted, size);
    stbi_write_bmp("../images/encrypted_modified_caesar.bmp", w1, h1, 1, encrypted);
    print_matrix(encrypted, w1, h1, "Encrypted Image Matrix");

    // Decrypt
    uint8_t *decrypted = (uint8_t *)malloc(size);
    decrypt_image(encrypted, key_img, decrypted, size);
    stbi_write_bmp("../images/decrypted_modified_caesar.bmp", w1, h1, 1, decrypted);
    print_matrix(decrypted, w1, h1, "Decrypted Image Matrix");

    // Cleanup
    stbi_image_free(msg_img);
    stbi_image_free(key_img);
    free(encrypted);
    free(decrypted);

    printf("\n Modified Caesar encryption and decryption complete.\n");
    return 0;
}
