#include "utiles.h"
#include "stdio.h"

int minimo(int num1, int num2){
  if (num1 > num2){
    return num2;
  }
  return num1;
}

int dos_a_la(int a){
  int res = 1;
  for (int i = 0; i < a; i++){
    res = res * 2;
  }
  return res;
}
