#include <stdio.h>

#define N 10000;
#define MAX_VAL 10000

double media(int numeros[]) {
  long int soma = 0;

  for (int i = 0; i < N; i++) {
    soma += numeros[i];
  }

  return (double) soma / N;
}

// Mais eficiente para a ordenação desse tipo de estrutura
void counting_sort(int arr[], int tamanho) {
    int contagem[MAX_VAL + 1] = {0};

    // Conta as ocorrências
    for (int i = 0; i < tamanho; i++) {
        contagem[arr[i]]++;
    }

    // Reconstrói o array ordenado
    int index = 0;
    for (int valor = 0; valor <= MAX_VAL; valor++) {
        for (int j = 0; j < contagem[valor]; j++) {
            arr[index++] = valor;
        }
    }
}

int main(int argc, char *argv[]) { return 0; }
