#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"

#define MATRIX_SIZE 16  // 16x16 = 256

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

// Build position table for 256 values in 16x16 matrix
void build_playfair_matrix(int pos[256][2]) {
    for (int i = 0; i < 256; i++) {
        pos[i][0] = i / MATRIX_SIZE;  // row
        pos[i][1] = i % MATRIX_SIZE;  // column
    }
}

// Modified Playfair encryption
void encrypt_image(uint8_t *input, uint8_t *output, int size, int pos[256][2]) {
    for (int i = 0; i < size; i += 2) {
        uint8_t a = input[i];
        uint8_t b = (i + 1 < size) ? input[i + 1] : 0;

        int row1 = pos[a][0], col1 = pos[a][1];
        int row2 = pos[b][0], col2 = pos[b][1];

        if (row1 == row2) {
            output[i] = row1 * MATRIX_SIZE + ((col1 + 1) % MATRIX_SIZE);
            output[i + 1] = row2 * MATRIX_SIZE + ((col2 + 1) % MATRIX_SIZE);
        } else if (col1 == col2) {
            output[i] = ((row1 + 1) % MATRIX_SIZE) * MATRIX_SIZE + col1;
            output[i + 1] = ((row2 + 1) % MATRIX_SIZE) * MATRIX_SIZE + col2;
        } else {
            output[i] = row1 * MATRIX_SIZE + col2;
            output[i + 1] = row2 * MATRIX_SIZE + col1;
        }
    }
}

// Modified Playfair decryption
void decrypt_image(uint8_t *input, uint8_t *output, int size, int pos[256][2]) {
    for (int i = 0; i < size; i += 2) {
        uint8_t a = input[i];
        uint8_t b = (i + 1 < size) ? input[i + 1] : 0;

        int row1 = pos[a][0], col1 = pos[a][1];
        int row2 = pos[b][0], col2 = pos[b][1];

        if (row1 == row2) {
            output[i] = row1 * MATRIX_SIZE + ((col1 - 1 + MATRIX_SIZE) % MATRIX_SIZE);
            output[i + 1] = row2 * MATRIX_SIZE + ((col2 - 1 + MATRIX_SIZE) % MATRIX_SIZE);
        } else if (col1 == col2) {
            output[i] = ((row1 - 1 + MATRIX_SIZE) % MATRIX_SIZE) * MATRIX_SIZE + col1;
            output[i + 1] = ((row2 - 1 + MATRIX_SIZE) % MATRIX_SIZE) * MATRIX_SIZE + col2;
        } else {
            output[i] = row1 * MATRIX_SIZE + col2;
            output[i + 1] = row2 * MATRIX_SIZE + col1;
        }
    }
}

int main() {
    int w, h, ch;

    // Load image
    uint8_t *img = stbi_load("../images/input1.bmp", &w, &h, &ch, 1); // grayscale
    if (!img) {
        printf(" Failed to load input1.bmp\n");
        return 1;
    }

    int size = w * h;

    // Show input image matrix
    print_matrix(img, w, h, "Original Input Image Matrix");

    // Build Playfair matrix positions
    int pos[256][2];
    build_playfair_matrix(pos);

    // Encrypt
    uint8_t *encrypted = (uint8_t *)malloc(size);
    encrypt_image(img, encrypted, size, pos);
    stbi_write_bmp("../images/encrypted_playfair.bmp", w, h, 1, encrypted);
    print_matrix(encrypted, w, h, "Encrypted Image Matrix");

    // Decrypt
    uint8_t *decrypted = (uint8_t *)malloc(size);
    decrypt_image(encrypted, decrypted, size, pos);
    stbi_write_bmp("../images/decrypted_playfair.bmp", w, h, 1, decrypted);
    print_matrix(decrypted, w, h, "Decrypted Image Matrix");

    // Cleanup
    stbi_image_free(img);
    free(encrypted);
    free(decrypted);

    printf("\n Playfair encryption and decryption complete.\n");
    return 0;
}
