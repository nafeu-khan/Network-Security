#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 100
#define MAX_N 10

int keyMatrix[MAX_N][MAX_N];  // Key matrix
int inverseKeyMatrix[MAX_N][MAX_N];  // Inverse key matrix

// Function to clean the input (remove spaces) and return length
int cleanInput(char *input, char *cleaned) {
    int len = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (isalpha(input[i])) {
            cleaned[len++] = toupper(input[i]);
        }
    }
    cleaned[len] = '\0';
    return len;
}

// Function to read key matrix
void readMatrix(int matrix[MAX_N][MAX_N], int n) {
    printf("Enter key matrix (%dx%d):\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
}

// Function to multiply matrix with vector and modulo 26
void matrixMultiply(int n, int matrix[MAX_N][MAX_N], int vector[MAX_N], int result[MAX_N]) {
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < n; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
        result[i] %= 26;  // Modulo 26 for letters
    }
}

// Function to compute the modular inverse of determinant modulo 26
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;  // No inverse found
}

// Function to compute determinant of matrix (only works for 2x2 in this example)
int determinant(int matrix[MAX_N][MAX_N], int n) {
    if (n == 2) {
        return (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);
    }
    // For larger matrices, recursive determinant can be implemented
    return 0;
}

// Function to compute inverse of 2x2 matrix
void inverseMatrix2x2(int matrix[MAX_N][MAX_N], int inverse[MAX_N][MAX_N]) {
    int det = determinant(matrix, 2) % 26;
    if (det < 0) det += 26; // Ensure positive determinant
    int detInverse = modInverse(det, 26);
    if (detInverse == -1) {
        printf("Matrix is not invertible under mod 26\n");
        return;
    }

    inverse[0][0] = matrix[1][1] * detInverse % 26;
    inverse[1][1] = matrix[0][0] * detInverse % 26;
    inverse[0][1] = (-matrix[0][1] + 26) * detInverse % 26;
    inverse[1][0] = (-matrix[1][0] + 26) * detInverse % 26;
}

// Hill Cipher Encryption
void encrypt(char *text, int n, char *encryptedText) {
    int len = strlen(text);
    for (int i = 0; i < len; i += n) {
        int vector[MAX_N], result[MAX_N];
        for (int j = 0; j < n; j++) {
            vector[j] = text[i + j] - 'A';
        }
        matrixMultiply(n, keyMatrix, vector, result);
        for (int j = 0; j < n; j++) {
            encryptedText[i + j] = result[j] + 'A';
        }
    }
    encryptedText[len] = '\0';
}

// Hill Cipher Decryption
void decrypt(char *text, int n, char *decryptedText) {
    int len = strlen(text);
    inverseMatrix2x2(keyMatrix, inverseKeyMatrix);  // This only works for 2x2 for now
    for (int i = 0; i < len; i += n) {
        int vector[MAX_N], result[MAX_N];
        for (int j = 0; j < n; j++) {
            vector[j] = text[i + j] - 'A';
        }
        matrixMultiply(n, inverseKeyMatrix, vector, result);
        for (int j = 0; j < n; j++) {
            decryptedText[i + j] = (result[j] + 26) % 26 + 'A';  // Ensure valid character
        }
    }
    decryptedText[len] = '\0';
}

int main() {
    char input[MAX_LEN], cleaned[MAX_LEN], result[MAX_LEN];
    int n;

    printf("Enter text: ");
    fgets(input, MAX_LEN, stdin);
    input[strcspn(input, "\n")] = '\0';  // Remove newline

    int cleanLen = cleanInput(input, cleaned);

    printf("Enter matrix dimension (n): ");
    scanf("%d", &n);

    if (cleanLen % n != 0) {
        printf("Error: Length of text (without spaces) must be divisible by %d\n", n);
        return 1;
    }

    readMatrix(keyMatrix, n);

    printf("Choose option:\n1. Encrypt\n2. Decrypt\nEnter choice: ");
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        encrypt(cleaned, n, result);
        printf("Encrypted Text: %s\n", result);
    } else if (choice == 2) {
        decrypt(cleaned, n, result);
        printf("Decrypted Text: %s\n", result);
    } else {
        printf("Invalid choice\n");
    }

    return 0;
}
