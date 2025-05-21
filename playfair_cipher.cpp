#include <stdio.h>
#include <string.h>

char alphabet_list[] = "abcdefghiklmnopqrstuvwxyz";

// Function to remove spaces
void remove_spaces(char *text) {
   int i, j = 0;
   for (i = 0; text[i] != '\0'; i++) {
      if (text[i] != ' ') {
         text[j++] = text[i];
      }
   }
   text[j] = '\0';
}

// Function to generate key matrix
void generate_key_matrix(char *key, char matrix[5][5]) {
   char used[26] = {0};
   int i, j, index = 0;

   for (i = 0; key[i] != '\0'; i++) {
      if (!used[key[i] - 'a']) {
         used[key[i] - 'a'] = 1;
         matrix[index / 5][index % 5] = key[i];
         index++;
      }
   }

   for (i = 0; alphabet_list[i] != '\0'; i++) {
      if (!used[alphabet_list[i] - 'a']) {
         used[alphabet_list[i] - 'a'] = 1;
         matrix[index / 5][index % 5] = alphabet_list[i];
         index++;
      }
   }
}

// Function to search for an element in the matrix
void search_element(char matrix[5][5], char element, int *row, int *col) {
   for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
         if (matrix[i][j] == element) {
            *row = i;
            *col = j;
            return;
         }
      }
   }
}

// Function to encrypt using Playfair rules
void encrypt_playfair_cipher(char matrix[5][5], char *plaintext, char *ciphertext) {
   int len = strlen(plaintext);
   int i, row1, col1, row2, col2;

   for (i = 0; i < len; i += 2) {
      search_element(matrix, plaintext[i], &row1, &col1);
      search_element(matrix, plaintext[i + 1], &row2, &col2);

      if (row1 == row2) {
         ciphertext[i] = matrix[row1][(col1 + 1) % 5];
         ciphertext[i + 1] = matrix[row2][(col2 + 1) % 5];
      } else if (col1 == col2) {
         ciphertext[i] = matrix[(row1 + 1) % 5][col1];
         ciphertext[i + 1] = matrix[(row2 + 1) % 5][col2];
      } else {
         ciphertext[i] = matrix[row1][col2];
         ciphertext[i + 1] = matrix[row2][col1];
      }
   }
   ciphertext[len] = '\0';
}

int main() {
   char plaintext[] = "network security lab";
   char key[] = "desktoplab";
   char ciphertext[50];
   char matrix[5][5];

   remove_spaces(plaintext);
   generate_key_matrix(key, matrix);
   encrypt_playfair_cipher(matrix, plaintext, ciphertext);

   printf("The Key text: %s\n", key);
   printf("The Plain Text: %s\n", plaintext);
   printf("The CipherText: %s\n", ciphertext);

   return 0;
}