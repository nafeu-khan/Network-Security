#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"

#define COL_KEY_LEN 4
int key[COL_KEY_LEN] = {3, 1, 4, 2}; // Order in which to read columns

// Utility function to print matrix
void print_matrix(uint8_t *data, int width, int height, const char *label) {
    printf("\n%s (First 10x10 pixels):\n", label);
    for (int i = 0; i < 10 && i < height; i++) {
        for (int j = 0; j < 10 && j < width; j++) {
            printf("%3d ", data[i * width + j]);
        }
        printf("\n");
    }
}

void columnar_encrypt(uint8_t *input, uint8_t *output, int total_pixels) {
    int rows = (total_pixels + COL_KEY_LEN - 1) / COL_KEY_LEN;
    uint8_t **matrix = malloc(rows * sizeof(uint8_t *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = calloc(COL_KEY_LEN, sizeof(uint8_t));
    }

    // Fill the matrix row-wise
    for (int i = 0; i < total_pixels; i++) {
        matrix[i / COL_KEY_LEN][i % COL_KEY_LEN] = input[i];
    }

    // Rearrange based on key
    int index = 0;
    for (int k = 1; k <= COL_KEY_LEN; k++) {
        for (int j = 0; j < COL_KEY_LEN; j++) {
            if (key[j] == k) {
                for (int i = 0; i < rows; i++) {
                    output[index++] = matrix[i][j];
                }
            }
        }
    }

    for (int i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);
}

void columnar_decrypt(uint8_t *input, uint8_t *output, int total_pixels) {
    int rows = (total_pixels + COL_KEY_LEN - 1) / COL_KEY_LEN;
    uint8_t **matrix = malloc(rows * sizeof(uint8_t *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = calloc(COL_KEY_LEN, sizeof(uint8_t));
    }

    // Fill columns based on key
    int index = 0;
    for (int k = 1; k <= COL_KEY_LEN; k++) {
        for (int j = 0; j < COL_KEY_LEN; j++) {
            if (key[j] == k) {
                for (int i = 0; i < rows; i++) {
                    matrix[i][j] = input[index++];
                }
            }
        }
    }

    // Read row-wise
    index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < COL_KEY_LEN; j++) {
            if (index < total_pixels) {
                output[index++] = matrix[i][j];
            }
        }
    }

    for (int i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);
}

int main() {
    int width, height, channels;

    // Load grayscale image
    uint8_t *input = stbi_load("../images/input1.bmp", &width, &height, &channels, 1);
    if (!input) {
        printf(" Failed to load input image.\n");
        return 1;
    }

    int total_pixels = width * height;
    printf("\nImage Dimensions: %d x %d\n", width, height);
    printf("Total Pixels: %d\n", total_pixels);

    print_matrix(input, width, height, "Original Image Matrix");

    // Encrypt
    uint8_t *encrypted = (uint8_t *)malloc(total_pixels);
    columnar_encrypt(input, encrypted, total_pixels);
    stbi_write_bmp("../images/encrypted_columnar.bmp", width, height, 1, encrypted);
    print_matrix(encrypted, width, height, "Encrypted Image Matrix");

    // Decrypt
    uint8_t *decrypted = (uint8_t *)malloc(total_pixels);
    columnar_decrypt(encrypted, decrypted, total_pixels);
    stbi_write_bmp("../images/decrypted_columnar.bmp", width, height, 1, decrypted);
    print_matrix(decrypted, width, height, "Decrypted Image Matrix");

    // Cleanup
    stbi_image_free(input);
    free(encrypted);
    free(decrypted);

    printf("\n Columnar Transposition encryption and decryption complete.\n");
    return 0;
}
