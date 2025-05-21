#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 100

// Function to encrypt text using Modified Caesar Cipher
void encrypt(char *text, int key) {
    int len = strlen(text);
    for (int i = 0; i < len; i++) {
        char ch = text[i];

        // Calculate dynamic shift based on position
        int shift = (key + i) % 26;

        if (isupper(ch)) {
            text[i] = ((ch - 'A' + shift) % 26) + 'A';
        } else if (islower(ch)) {
            text[i] = ((ch - 'a' + shift) % 26) + 'a';
        }
    }
}

// Function to decrypt text using Modified Caesar Cipher
void decrypt(char *text, int key) {
    int len = strlen(text);
    for (int i = 0; i < len; i++) {
        char ch = text[i];

        // Reverse the shift based on position
        int shift = (key + i) % 26;

        if (isupper(ch)) {
            text[i] = ((ch - 'A' - shift + 26) % 26) + 'A';
        } else if (islower(ch)) {
            text[i] = ((ch - 'a' - shift + 26) % 26) + 'a';
        }
    }
}

int main() {
    char text[MAX_LEN];
    int key, choice;

    printf("Enter text: ");
    fgets(text, MAX_LEN, stdin);
    text[strcspn(text, "\n")] = '\0';  // Remove newline character

    printf("Enter key: ");
    scanf("%d", &key);

    printf("Choose an option: \n1. Encrypt\n2. Decrypt\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        encrypt(text, key);
        printf("Encrypted Text: %s\n", text);
    } else if (choice == 2) {
        decrypt(text, key);
        printf("Decrypted Text: %s\n", text);
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}
