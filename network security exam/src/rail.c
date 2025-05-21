#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"

#define NUM_RAILS 3  // Number of rails for the cipher

// Utility to print a 10×10 region of a grayscale image
void print_matrix(uint8_t *data, int width, int height, const char *label) {
    printf("\n%s (First 10×10 pixels):\n", label);
    for (int i = 0; i < 10 && i < height; i++) {
        for (int j = 0; j < 10 && j < width; j++) {
            printf("%3d ", data[i * width + j]);
        }
        printf("\n");
    }
}

// Rail-Fence encryption: builds a zigzag index order
void rail_fence_encrypt(uint8_t *in, uint8_t *out, int size) {
    int rail = 0, dir = 1;
    int *counts   = calloc(NUM_RAILS, sizeof(int));
    int **rails   = malloc(NUM_RAILS * sizeof(int*));
    for (int r = 0; r < NUM_RAILS; r++)
        rails[r] = malloc(size * sizeof(int));

    // assign each position to a rail
    for (int i = 0; i < size; i++) {
        rails[rail][counts[rail]++] = i;
        if (rail == 0)       dir =  1;
        else if (rail == NUM_RAILS-1) dir = -1;
        rail += dir;
    }

    // write out in rail order
    int idx = 0;
    for (int r = 0; r < NUM_RAILS; r++)
        for (int k = 0; k < counts[r]; k++)
            out[idx++] = in[ rails[r][k] ];

    for (int r = 0; r < NUM_RAILS; r++) free(rails[r]);
    free(rails);
    free(counts);
}

// Rail-Fence decryption: reverse the zigzag index mapping
void rail_fence_decrypt(uint8_t *in, uint8_t *out, int size) {
    int rail = 0, dir = 1;
    int *counts   = calloc(NUM_RAILS, sizeof(int));
    int **rails   = malloc(NUM_RAILS * sizeof(int*));
    for (int r = 0; r < NUM_RAILS; r++)
        rails[r] = malloc(size * sizeof(int));

    // rebuild the same rail pattern
    for (int i = 0; i < size; i++) {
        rails[rail][counts[rail]++] = i;
        if (rail == 0)       dir =  1;
        else if (rail == NUM_RAILS-1) dir = -1;
        rail += dir;
    }

    // read ciphertext back into original positions
    int idx = 0;
    for (int r = 0; r < NUM_RAILS; r++)
        for (int k = 0; k < counts[r]; k++)
            out[ rails[r][k] ] = in[idx++];

    for (int r = 0; r < NUM_RAILS; r++) free(rails[r]);
    free(rails);
    free(counts);
}

int main() {
    int w, h, ch;

    // Load as grayscale
    uint8_t *input = stbi_load("../images/input1.bmp", &w, &h, &ch, 1);
    if (!input) {
        fprintf(stderr, " Failed to load input1.bmp\n");
        return 1;
    }
    int size = w * h;

    // Buffers
    uint8_t *encrypted = malloc(size);
    uint8_t *decrypted = malloc(size);

    // Encrypt then immediately decrypt
    rail_fence_encrypt(input, encrypted, size);
    rail_fence_decrypt(encrypted, decrypted, size);

    // Write only the decrypted (recovered) image
    stbi_write_bmp("../images/decrypted_rail_fence.bmp", w, h, 1, decrypted);

    // Print the recovered 10×10 pixel block
    print_matrix(decrypted, w, h, "Decrypted (Original) Image Matrix");

    // Cleanup
    stbi_image_free(input);
    free(encrypted);
    free(decrypted);

    printf("\n Rail Fence decrypt-only mode complete. See decrypted_rail_fence.bmp\n");
    return 0;
}
