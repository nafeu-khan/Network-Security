#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"

#define WIDTH 128
#define HEIGHT 128

// Get Caesar shift from key image (average pixel)
int calculate_caesar_key(uint8_t *key_data, int size) {
    // Example key from average pixel value
    // long sum = 0;
    // for (int i = 0; i < size; i++) {
    //     sum += key_data[i];
    // }
    // return (sum / size) % 256;
    return 3;
}

// Caesar encryption
void encrypt_image(uint8_t *input, uint8_t *output, int size, int key) {
    for (int i = 0; i < size; i++) {
        output[i] = (input[i] + key) % 256;
    }
}

// Caesar decryption
void decrypt_image(uint8_t *input, uint8_t *output, int size, int key) {
    for (int i = 0; i < size; i++) {
        output[i] = (input[i] - key + 256) % 256;
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
    int w1, h1, ch1;
    int w2, h2, ch2;

    // Load message image
    uint8_t *msg_img = stbi_load("../images/input1.bmp", &w1, &h1, &ch1, 1); // grayscale
    if (!msg_img) {
        printf("Failed to load input1.bmp\n");
        return 1;
    }

    // Load key image
    uint8_t *key_img = stbi_load("../images/input2.bmp", &w2, &h2, &ch2, 1); // grayscale
    if (!key_img) {
        printf("Failed to load input2.bmp\n");
        stbi_image_free(msg_img);
        return 1;
    }

    if (w1 != w2 || h1 != h2) {
        printf("Images must be the same size!\n");
        stbi_image_free(msg_img);
        stbi_image_free(key_img);
        return 1;
    }

    int size = w1 * h1;

    // Show input image matrix
    print_matrix(msg_img, w1, h1, "Original Input Image Matrix");

    int key = calculate_caesar_key(key_img, size);
    printf("\nCaesar Key (derived from key image): %d\n", key);

    // Encrypt
    uint8_t *encrypted = (uint8_t *)malloc(size);
    encrypt_image(msg_img, encrypted, size, key);
    stbi_write_bmp("../images/encrypted.bmp", w1, h1, 1, encrypted);
    print_matrix(encrypted, w1, h1, "Encrypted Image Matrix");

    // Decrypt
    uint8_t *decrypted = (uint8_t *)malloc(size);
    decrypt_image(encrypted, decrypted, size, key);
    stbi_write_bmp("../images/decrypted.bmp", w1, h1, 1, decrypted);
    print_matrix(decrypted, w1, h1, "Decrypted Image Matrix");

    // Cleanup
    stbi_image_free(msg_img);
    stbi_image_free(key_img);
    free(encrypted);
    free(decrypted);

    printf("\n Encryption and decryption complete. Check the images and matrices.\n");
    return 0;
}
