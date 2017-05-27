#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


// int subString(char* word1, char* word2, int i, int j){
//   if (i == strlen(word1) || j == strlen(word2)){
//     return 0;
//   }
//   if (word1[i] == word2[j]){
//     return 1 + subString(word1, word2, i + 1, j + 1);
//   }
//   int a = subString(word1, word2, i + 1, j);
//   int b = subString(word1, word2, i, j + 1);
//   if (a < b){
//     return b;
//   }
//   return a;
// }

int subString(char* word1, char* word2, int i, int j, int** tabla){
  int resp;
  if (i == strlen(word1) || j == strlen(word2)){
    return 0;
  }
  if (word1[i] == word2[j]){
    if (tabla[i + 1][j + 1] != -1){
      return 1 + tabla[i + 1][j + 1];
    }
    resp = subString(word1, word2, i + 1, j + 1, tabla);
    tabla[i+1][j+1] = resp;
    return 1 + resp;
  }
  int a;
  if (tabla[i+1][j] != -1){
    a = tabla[i+1][j];
  }
  else {
    a = subString(word1, word2, i + 1, j, tabla);
    tabla[i+1][j] = a;
  }
  int b;
  if (tabla[i][j+1] != -1){
    b = tabla[i][j+1];
  }
  else {
    b = subString(word1, word2, i, j + 1, tabla);
    tabla[i][j+1] = b;
  }
  if (a < b){
    return b;
  }
  return a;
}

int main(int argc, char *argv[]) {
  char* word1 = argv[1];
  char* word2 = argv[2];

  // printf("Tamaño del input: %i\n", len);

  //Parte exponencial
  // printf("Tamaño del substring: %i\n", subString(word1, word2, 0, 0));

  //Parte polinommial
  int len1 = strlen(word1);
  int len2 = strlen(word2);
  int** tabla = malloc(sizeof(int*) * len1+1);
  for (int i = 0; i <= len1; i++){
    tabla[i] = malloc(sizeof(int) * len2+1);
    for (int j = 0; j <= len2; j++){
      tabla[i][j] = -1;
    }
  }
  printf("Tamaño del substring: %i\n", subString(word1, word2, 0, 0, tabla));
  for (int i = 0; i <= len1; i++){
    free(tabla[i]);
  }
  free(tabla);

  return 0;
}
