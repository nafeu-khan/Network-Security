#include <stdio.h>
#include <string.h>

#define ALPHABET_SIZE 62  // A-Z (0-25), a-z (26-51), 0-9 (52-61)

int charToIndex(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    return -1;
}

char indexToChar(int index) {
    if (index >= 0 && index <= 25) return 'A' + index;
    if (index >= 26 && index <= 51) return 'a' + (index - 26);
    if (index >= 52 && index <= 61) return '0' + (index - 52);
    return '?';
}

void vigenereEncrypt(char *plaintext, char *key, char *ciphertext) {
    int textLen = strlen(plaintext);
    int keyLen = strlen(key);
    for (int i = 0; i < textLen; i++) {

            if(plaintext[i]==' '){
                ciphertext[i]=' ';
                continue;
            }
        int textIdx = charToIndex(plaintext[i]);
        int keyIdx = charToIndex(key[i % keyLen]);
        if (textIdx == -1 || keyIdx == -1) {
            ciphertext[i] = plaintext[i];
        } else {
            ciphertext[i] = indexToChar((textIdx + keyIdx) % ALPHABET_SIZE);
        }
    }
    ciphertext[textLen] = '\0';
}

int main() {
    char plaintext[100], key[100], ciphertext[100];

    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0';

    printf("Enter the key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';

    vigenereEncrypt(plaintext, key, ciphertext);
    printf("Encrypted Text: %s\n", ciphertext);

    return 0;
}