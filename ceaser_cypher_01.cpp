#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to encrypt text using Caesar Cipher
void encrypt(char text[], int shift) {
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];

        if (isupper(ch))  
            text[i] = (ch - 'A' + (shift % 26) + 26) % 26 + 'A';
        else if (islower(ch))  
            text[i] = (ch - 'a' + (shift % 26) + 26) % 26 + 'a';
        else  
            text[i] = ch;  // Keep spaces, punctuation, numbers unchanged
    }
}

// Function to decrypt text using Caesar Cipher
void decrypt(char text[], int shift) {
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];

        if (isupper(ch))  
            text[i] = (ch - 'A' - (shift % 26) + 26) % 26 + 'A';
        else if (islower(ch))  
            text[i] = (ch - 'a' - (shift % 26) + 26) % 26 + 'a';
        else  
            text[i] = ch;  // Keep spaces, punctuation, numbers unchanged
    }
}

int main() {
    char text[100];
    int shift;

    // Input message
    printf("Enter message: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = 0;  // Remove newline

    // Input shift value
    printf("Enter shift value: ");
    scanf("%d", &shift);

    // Encrypt and display result
    encrypt(text, shift);
    printf("Encrypted: %s\n", text);

    // Decrypt and display result
    decrypt(text, shift);
    printf("Decrypted: %s\n", text);

    return 0;
}

