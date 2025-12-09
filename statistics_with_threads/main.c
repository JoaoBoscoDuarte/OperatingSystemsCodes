#include <stdio.h>

#define LIMITE 10000;

double media(int numeros[]) {
  long int soma = 0;

  for (int i = 0; i < LIMITE; i++) {
    soma += numeros[i];
  }

  return (double) soma / LIMITE;
}

int main(int argc, char *argv[]) { return 0; }
