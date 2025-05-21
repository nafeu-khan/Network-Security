#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"

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

// ElGamal parameters (small prime for demo)
const uint32_t P = 30803;      // A prime number > 255 (choose suitable prime)
const uint32_t G = 2;          // Generator
const uint32_t X = 7897;       // Private key (randomly chosen < P)
uint32_t Y;                    // Public key Y = G^X mod P

// Compute public key Y
void elgamal_init() {
    Y = mod_exp(G, X, P);
}

// Print matrix uint8_t
void print_matrix_uint8(uint8_t *data, int width, int height, const char *label) {
    printf("\n%s (First 10x10 pixels):\n", label);
    for (int i = 0; i < 10 && i < height; i++) {
        for (int j = 0; j < 10 && j < width; j++) {
            printf("%4d ", data[i * width + j]);
        }
        printf("\n");
    }
}

// Encrypt one byte with ElGamal CFB mode
// input: plaintext byte M, previous ciphertext C_prev
// output: ciphertext byte C_out, next feedback vector
uint8_t elgamal_encrypt_byte(uint8_t M, uint32_t *feedback) {
    // For demo, fixed ephemeral key k = 12345 (should be random per block in real)
    uint32_t k = 12345;

    // Calculate S = g^k mod p
    uint32_t S = mod_exp(G, k, P);

    // Calculate shared secret = Y^k mod p
    uint32_t shared_secret = mod_exp(Y, k, P);

    // XOR plaintext with low 8 bits of (shared_secret XOR feedback low 8 bits)
    uint8_t C = M ^ ((shared_secret & 0xFF) ^ (*feedback & 0xFF));

    // Update feedback with S (low 8 bits)
    *feedback = S;

    return C;
}

// Decrypt one byte with ElGamal CFB mode
// input: ciphertext byte C, previous ciphertext feedback
// output: plaintext byte M, next feedback vector
uint8_t elgamal_decrypt_byte(uint8_t C, uint32_t *feedback) {
    // Fixed ephemeral key k = 12345 (same as encryption)
    uint32_t k = 12345;

    // Calculate S = g^k mod p
    uint32_t S = mod_exp(G, k, P);

    // Calculate shared secret = Y^k mod p
    uint32_t shared_secret = mod_exp(Y, k, P);

    // XOR ciphertext with low 8 bits of (shared_secret XOR feedback low 8 bits)
    uint8_t M = C ^ ((shared_secret & 0xFF) ^ (*feedback & 0xFF));

    // Update feedback with S
    *feedback = S;

    return M;
}

int main() {
    int w, h, ch;

    elgamal_init();

    // Load grayscale image
    uint8_t *img = stbi_load("../images/input1.bmp", &w, &h, &ch, 1);
    if (!img) {
        printf(" Failed to load input1.bmp\n");
        return 1;
    }
    int size = w * h;

    print_matrix_uint8(img, w, h, "Original Image Matrix");

    // Encrypt
    uint8_t *encrypted = (uint8_t *)malloc(size);
    if (!encrypted) {
        printf(" Memory allocation failed\n");
        stbi_image_free(img);
        return 1;
    }
    uint32_t feedback_enc = 0;  // IV for encryption (0 for demo)
    for (int i = 0; i < size; i++) {
        encrypted[i] = elgamal_encrypt_byte(img[i], &feedback_enc);
    }

    stbi_write_bmp("../images/encrypted_cfb_elgamal.bmp", w, h, 1, encrypted);
    print_matrix_uint8(encrypted, w, h, "Encrypted Image Matrix");

    // Decrypt
    uint8_t *decrypted = (uint8_t *)malloc(size);
    if (!decrypted) {
        printf(" Memory allocation failed\n");
        free(encrypted);
        stbi_image_free(img);
        return 1;
    }
    uint32_t feedback_dec = 0;  // IV for decryption (must match encryption)
    for (int i = 0; i < size; i++) {
        decrypted[i] = elgamal_decrypt_byte(encrypted[i], &feedback_dec);
    }

    stbi_write_bmp("../images/decrypted_cfb_elgamal.bmp", w, h, 1, decrypted);
    print_matrix_uint8(decrypted, w, h, "Decrypted Image Matrix");

    // Cleanup
    stbi_image_free(img);
    free(encrypted);
    free(decrypted);

    printf("\n ElGamal CFB encryption and decryption complete.\n");
    return 0;
}
