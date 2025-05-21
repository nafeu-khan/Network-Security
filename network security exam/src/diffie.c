#include <stdio.h>
#include <stdint.h>
#include <math.h>

// Function to compute (base^exp) % mod
uint64_t power_mod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

int main() {
    // Public parameters (small for demo; use 2048-bit in practice)
    uint64_t p = 23;  // prime number
    uint64_t g = 5;   // primitive root modulo p

    printf("Public Parameters:\n");
    printf("Prime p = %llu\n", p);
    printf("Generator g = %llu\n", g);

    // Alice's private key
    uint64_t a_private = 6;
    // Bob's private key
    uint64_t b_private = 15;

    // Compute public keys
    uint64_t A = power_mod(g, a_private, p); // Alice's public key
    uint64_t B = power_mod(g, b_private, p); // Bob's public key

    printf("\nAlice Private Key: %llu\n", a_private);
    printf("Bob Private Key: %llu\n", b_private);

    printf("\nAlice Public Key (A): %llu\n", A);
    printf("Bob Public Key (B): %llu\n", B);

    // Compute shared secret
    uint64_t shared_A = power_mod(B, a_private, p); // Alice computes
    uint64_t shared_B = power_mod(A, b_private, p); // Bob computes

    printf("\nShared Secret Computed by Alice: %llu\n", shared_A);
    printf("Shared Secret Computed by Bob:   %llu\n", shared_B);

    if (shared_A == shared_B) {
        printf("\n Key exchange successful. Shared key established securely.\n");
    } else {
        printf("\n Key exchange failed. Shared keys do not match.\n");
    }

    return 0;
}
