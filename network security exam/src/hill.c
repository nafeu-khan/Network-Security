#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"

// Matrix size for Hill cipher
#define MATRIX_SIZE 3

// Compute determinant of 3x3 matrix modulo 256
int mod256(int x) {
    int r = x % 256;
    return r < 0 ? r + 256 : r;
}

int determinant(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int det = 0;
    det += matrix[0][0] * (matrix[1][1]*matrix[2][2] - matrix[1][2]*matrix[2][1]);
    det -= matrix[0][1] * (matrix[1][0]*matrix[2][2] - matrix[1][2]*matrix[2][0]);
    det += matrix[0][2] * (matrix[1][0]*matrix[2][1] - matrix[1][1]*matrix[2][0]);
    return mod256(det);
}

// Extended Euclidean Algorithm for modular inverse of determinant mod 256
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1)
            return x;
    }
    return -1; // No modular inverse if -1
}

// Calculate adjugate matrix of 3x3 matrix mod 256
void adjugate(int matrix[MATRIX_SIZE][MATRIX_SIZE], int adj[MATRIX_SIZE][MATRIX_SIZE]) {
    adj[0][0] = mod256(matrix[1][1]*matrix[2][2] - matrix[1][2]*matrix[2][1]);
    adj[0][1] = mod256(-(matrix[0][1]*matrix[2][2] - matrix[0][2]*matrix[2][1]));
    adj[0][2] = mod256(matrix[0][1]*matrix[1][2] - matrix[0][2]*matrix[1][1]);

    adj[1][0] = mod256(-(matrix[1][0]*matrix[2][2] - matrix[1][2]*matrix[2][0]));
    adj[1][1] = mod256(matrix[0][0]*matrix[2][2] - matrix[0][2]*matrix[2][0]);
    adj[1][2] = mod256(-(matrix[0][0]*matrix[1][2] - matrix[0][2]*matrix[1][0]));

    adj[2][0] = mod256(matrix[1][0]*matrix[2][1] - matrix[1][1]*matrix[2][0]);
    adj[2][1] = mod256(-(matrix[0][0]*matrix[2][1] - matrix[0][1]*matrix[2][0]));
    adj[2][2] = mod256(matrix[0][0]*matrix[1][1] - matrix[0][1]*matrix[1][0]);
}

// Multiply 3x3 matrix with 3x1 vector modulo 256
void matrix_multiply(int matrix[MATRIX_SIZE][MATRIX_SIZE], uint8_t *vec, uint8_t *result) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        int val = 0;
        for (int j = 0; j < MATRIX_SIZE; j++) {
            val += matrix[i][j] * vec[j];
        }
        result[i] = mod256(val);
    }
}

// Print matrix for debugging
void print_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE], const char* name) {
    printf("\n%s:\n", name);
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%4d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int w, h, ch;

    // Load grayscale image (1 channel)
    uint8_t *img = stbi_load("../images/input1.bmp", &w, &h, &ch, 1);
    if (!img) {
        printf(" Failed to load input1.bmp\n");
        return 1;
    }
    int size = w * h;

    printf("Image loaded: %d x %d, channels: %d\n", w, h, 1);

    // Hill cipher key matrix (must be invertible mod 256)
    int key[MATRIX_SIZE][MATRIX_SIZE] = {
        {6, 24, 1},
        {13, 16, 10},
        {20, 17, 15}
    };

    print_matrix(key, "Key Matrix");

    // Calculate determinant and its modular inverse
    int det = determinant(key);
    int det_inv = modInverse(det, 256);
    if (det_inv == -1) {
        printf(" Key matrix is not invertible mod 256. Choose another key.\n");
        stbi_image_free(img);
        return 1;
    }
    printf("Determinant mod 256: %d\n", det);
    printf("Modular inverse of determinant: %d\n", det_inv);

    // Calculate adjugate matrix
    int adj[MATRIX_SIZE][MATRIX_SIZE];
    adjugate(key, adj);

    print_matrix(adj, "Adjugate Matrix");

    // Calculate inverse key matrix = det_inv * adj mod 256
    int inv_key[MATRIX_SIZE][MATRIX_SIZE];
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            inv_key[i][j] = mod256(adj[i][j] * det_inv);
        }
    }
    print_matrix(inv_key, "Inverse Key Matrix");

    // Padding image size to multiple of 3
    int padded_size = size;
    if (padded_size % MATRIX_SIZE != 0) {
        padded_size += MATRIX_SIZE - (padded_size % MATRIX_SIZE);
    }

    uint8_t *padded_img = (uint8_t *)calloc(padded_size, sizeof(uint8_t));
    memcpy(padded_img, img, size);

    // Encrypt image
    uint8_t *encrypted = (uint8_t *)malloc(padded_size);
    for (int i = 0; i < padded_size; i += MATRIX_SIZE) {
        matrix_multiply(key, &padded_img[i], &encrypted[i]);
    }

    stbi_write_bmp("../images/encrypted_hill.bmp", w, h, 1, encrypted);
    printf("\nEncrypted image saved as encrypted_hill.bmp\n");

    // Decrypt image
    uint8_t *decrypted = (uint8_t *)malloc(padded_size);
    for (int i = 0; i < padded_size; i += MATRIX_SIZE) {
        matrix_multiply(inv_key, &encrypted[i], &decrypted[i]);
    }

    // Copy decrypted pixels back to original image size (ignore padding)
    stbi_write_bmp("../images/decrypted_hill.bmp", w, h, 1, decrypted);
    printf("Decrypted image saved as decrypted_hill.bmp\n");

    // Show first 10x10 pixels of original, encrypted, decrypted for verification
    printf("\nOriginal Image Matrix (First 10x10 pixels):\n");
    for (int i = 0; i < 10 && i < h; i++) {
        for (int j = 0; j < 10 && j < w; j++) {
            printf("%4d ", img[i * w + j]);
        }
        printf("\n");
    }

    printf("\nEncrypted Image Matrix (First 10x10 pixels):\n");
    for (int i = 0; i < 10 && i < h; i++) {
        for (int j = 0; j < 10 && j < w; j++) {
            printf("%4d ", encrypted[i * w + j]);
        }
        printf("\n");
    }

    printf("\nDecrypted Image Matrix (First 10x10 pixels):\n");
    for (int i = 0; i < 10 && i < h; i++) {
        for (int j = 0; j < 10 && j < w; j++) {
            printf("%4d ", decrypted[i * w + j]);
        }
        printf("\n");
    }

    // Cleanup
    stbi_image_free(img);
    free(padded_img);
    free(encrypted);
    free(decrypted);

    printf("\n Hill cipher encryption and decryption completed successfully.\n");
    return 0;
}
