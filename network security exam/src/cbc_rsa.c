#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"

// RSA parameters (small primes for demo)
const uint32_t RSA_N = 3233;    // modulus n = p*q (61*53)
const uint32_t RSA_E = 17;      // public exponent
const uint32_t RSA_D = 2753;    // private exponent

// Modular exponentiation
uint32_t mod_exp(uint32_t base, uint32_t exp, uint32_t mod) {
    uint64_t result = 1;
    uint64_t base_mod = base % mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base_mod) % mod;
        }
        base_mod = (base_mod * base_mod) % mod;
        exp >>= 1;
    }
    return (uint32_t)result;
}

// Print matrix (first 10x10 pixels)
void print_matrix_uint8(uint8_t *data, int width, int height, const char *label) {
    printf("\n%s (First 10x10 pixels):\n", label);
    for (int i = 0; i < 10 && i < height; i++) {
        for (int j = 0; j < 10 && j < width; j++) {
            printf("%4d ", data[i * width + j]);
        }
        printf("\n");
    }
}

// Print matrix (first 10x10 pixels) of uint32_t, showing only lower 8 bits (for visualization)
void print_matrix_uint32(uint32_t *data, int width, int height, const char *label) {
    printf("\n%s (First 10x10 pixels - lower 8 bits):\n", label);
    for (int i = 0; i < 10 && i < height; i++) {
        for (int j = 0; j < 10 && j < width; j++) {
            printf("%4d ", data[i * width + j] & 0xFF);
        }
        printf("\n");
    }
}

// RSA encrypt a byte (plaintext < RSA_N)
uint32_t rsa_encrypt_byte(uint8_t plaintext) {
    return mod_exp(plaintext, RSA_E, RSA_N);
}

// RSA decrypt a ciphertext block
uint8_t rsa_decrypt_uint32(uint32_t ciphertext) {
    return (uint8_t)mod_exp(ciphertext, RSA_D, RSA_N);
}

// CBC encryption with RSA per pixel
void encrypt_cbc_rsa(uint8_t *plaintext, uint32_t *ciphertext, int size) {
    uint32_t prev_cipher = 0; // IV

    for (int i = 0; i < size; i++) {
        uint32_t block = (uint32_t)(plaintext[i] ^ (prev_cipher & 0xFF)); // XOR with lowest byte of prev_cipher
        uint32_t encrypted = rsa_encrypt_byte((uint8_t)block);
        ciphertext[i] = encrypted;
        prev_cipher = encrypted;
    }
}

// CBC decryption with RSA per pixel
void decrypt_cbc_rsa(uint32_t *ciphertext, uint8_t *plaintext, int size) {
    uint32_t prev_cipher = 0; // IV

    for (int i = 0; i < size; i++) {
        uint8_t decrypted = rsa_decrypt_uint32(ciphertext[i]);
        plaintext[i] = decrypted ^ (prev_cipher & 0xFF);
        prev_cipher = ciphertext[i];
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

    print_matrix_uint8(img, w, h, "Original Image Matrix");

    // Allocate ciphertext array (uint32_t)
    uint32_t *encrypted = (uint32_t *)malloc(sizeof(uint32_t) * size);
    if (!encrypted) {
        printf(" Memory allocation failed\n");
        stbi_image_free(img);
        return 1;
    }

    // Encrypt
    encrypt_cbc_rsa(img, encrypted, size);

    // For visualization, save encrypted image (only lower 8 bits)
    uint8_t *enc_vis = (uint8_t *)malloc(size);
    for (int i = 0; i < size; i++) {
        enc_vis[i] = (uint8_t)(encrypted[i] & 0xFF);
    }
    stbi_write_bmp("../images/encrypted_cbc_rsa.bmp", w, h, 1, enc_vis);
    print_matrix_uint32(encrypted, w, h, "Encrypted Image Matrix");

    // Decrypt
    uint8_t *decrypted = (uint8_t *)malloc(size);
    decrypt_cbc_rsa(encrypted, decrypted, size);

    stbi_write_bmp("../images/decrypted_cbc_rsa.bmp", w, h, 1, decrypted);
    print_matrix_uint8(decrypted, w, h, "Decrypted Image Matrix");

    // Cleanup
    stbi_image_free(img);
    free(encrypted);
    free(enc_vis);
    free(decrypted);

    printf("\n CBC RSA encryption and decryption complete.\n");
    return 0;
}
