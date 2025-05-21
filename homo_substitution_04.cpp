#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX_LEN 100

// Define homophonic mappings
const char *homophonicMap[26][3] = {
    {"Q", "W", "E"},  // A
    {"R", "T", "Y"},  // B
    {"U", "I", "O"},  // C
    {"P", "A", "S"},  // D
    {"D", "F", "G"},  // E
    {"H", "J", "K"},  // F
    {"L", "Z", "X"},  // G
    {"C", "V", "B"},  // H
    {"N", "M", "1"},  // I
    {"2", "3", "4"},  // J
    {"5", "6", "7"},  // K
    {"8", "9", "0"},  // L
    {"!", "@", "#"},  // M
    {"$", "%", "^"},  // N
    {"&", "*", "("},  // O
    {")", "_", "+"},  // P
    {"-", "=", "["},  // Q
    {"]", "{", "}"},  // R
    {";", ":", "'"},  // S
    {"<", ">", "?"},  // T
    {"/", "|", "\\"}, // U
    {"a", "b", "c"},  // V
    {"d", "e", "f"},  // W
    {"g", "h", "i"},  // X
    {"j", "k", "l"},  // Y
    {"m", "n", "o"}   // Z
};

// Reverse lookup table for decryption
char decryptionMap[128];  // ASCII size for fast lookup

// Function to initialize the decryption map
void createDecryptionMap() {
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 3; j++) {
            decryptionMap[(int)homophonicMap[i][j][0]] = 'A' + i;
        }
    }
}

// Function to encrypt text
void encrypt(char *text, char *encryptedText) {
    int len = strlen(text);
    srand(time(NULL)); // Seed the random number generator

    int index = 0;
    for (int i = 0; i < len; i++) {
        char ch = toupper(text[i]);  // Convert to uppercase
        if (ch >= 'A' && ch <= 'Z') {
            int choice = rand() % 3; // Pick one of the three mappings randomly
            encryptedText[index++] = homophonicMap[ch - 'A'][choice][0];
        } else {
            encryptedText[index++] = ch;  // Keep non-alphabet characters unchanged
        }
    }
    encryptedText[index] = '\0';  // Null-terminate the encrypted string
}

// Function to decrypt text
void decrypt(char *encryptedText, char *decryptedText) {
    int len = strlen(encryptedText);
    for (int i = 0; i < len; i++) {
        char ch = encryptedText[i];
        if (decryptionMap[(int)ch] != 0) {
            decryptedText[i] = decryptionMap[(int)ch];
        } else {
            decryptedText[i] = ch;  // Keep non-mapped characters unchanged
        }
    }
    decryptedText[len] = '\0';  // Null-terminate the decrypted string
}

int main() {
    char text[MAX_LEN], encryptedText[MAX_LEN], decryptedText[MAX_LEN];
    int choice;

    printf("Enter text: ");
    fgets(text, MAX_LEN, stdin);
    text[strcspn(text, "\n")] = '\0';  // Remove newline character

    createDecryptionMap(); // Initialize the decryption map

    printf("Choose an option: \n1. Encrypt\n2. Decrypt\nEnter choice: ");
    scanf("%d", &choice);
    getchar(); // Clear newline from buffer

    if (choice == 1) {
        encrypt(text, encryptedText);
        printf("Encrypted Text: %s\n", encryptedText);
    } else if (choice == 2) {
        decrypt(text, decryptedText);
        printf("Decrypted Text: %s\n", decryptedText);
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}

