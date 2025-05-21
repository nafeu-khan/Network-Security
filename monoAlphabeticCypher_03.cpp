#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 100

// Define the encryption and decryption maps
char encryptionMap[27] = "QWERTYUIOPASDFGHJKLZXCVBNM"; // Example mapping
char decryptionMap[27]; // This will store the reverse mapping

// Function to generate the decryption map
void createDecryptionMap() {
    for (int i = 0; i < 26; i++) {
        decryptionMap[encryptionMap[i] - 'A'] = 'A' + i;
    }
}

// Function to encrypt text using Monoalphabetic Cipher
void encrypt(char *text) {
    int len = strlen(text);
    for (int i = 0; i < len; i++) {
        if (isupper(text[i])) {
            text[i] = encryptionMap[text[i] - 'A'];
        } else if (islower(text[i])) {
            text[i] = tolower(encryptionMap[text[i] - 'a']);
        }
    }
}

// Function to decrypt text using Monoalphabetic Cipher
void decrypt(char *text) {
    int len = strlen(text);
    for (int i = 0; i < len; i++) {
        if (isupper(text[i])) {
            text[i] = decryptionMap[text[i] - 'A'];
        } else if (islower(text[i])) {
            text[i] = tolower(decryptionMap[text[i] - 'a']);
        }
    }
}

int main() {
    char text[MAX_LEN];
    int choice;

    printf("Enter text: ");
    fgets(text, MAX_LEN, stdin);
    text[strcspn(text, "\n")] = '\0';  // Remove newline character

    createDecryptionMap(); // Prepare the decryption map

    printf("Choose an option: \n1. Encrypt\n2. Decrypt\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        encrypt(text);
        printf("Encrypted Text: %s\n", text);
    } else if (choice == 2) {
        decrypt(text);
        printf("Decrypted Text: %s\n", text);
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}
