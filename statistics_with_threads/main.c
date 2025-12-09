#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 10000
#define MAX_VAL 100

// Função básica para calcular a média
double media(int numeros[]) {
  long int soma = 0;

  for (int i = 0; i < N; i++) {
    soma += numeros[i];
  }

  return (double) soma / N;
}

// Método de ordenação couting sort para ordenar a lista antes de calcular a mediana 
// mais eficiente para a ordenação desse tipo de estrutura (busca quase linear)
void counting_sort(int arr[]) {
    int contagem[MAX_VAL + 1] = {0};

    // Conta as ocorrências
    for (int i = 0; i < N; i++) {
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

// Função para calcular a mediana
double mediana(int numeros[]) {
  return (numeros[4999] + numeros[5000]) / 2.0;
}

// Função para calcular o desvio padrão 
double desvio_padrao(int v[]) {
    double m = media(v);
    double soma_quadrados = 0.0;

    for (int i = 0; i < N; i++) {
        double diff = v[i] - m;
        soma_quadrados += diff * diff;
    }

    double variancia = soma_quadrados / N;   // desvio padrão populacional
    return sqrt(variancia);
}

// Gerador de 10000 numeros aleatorios em um intervalo de [0 a 100]
void gerador_numeros(int numeros[]) {
  for (int i = 0; i < N; i++) {
    numeros[i] = rand() % (MAX_VAL + 1);
  }
}

int main(int argc, char const *argv[])
{
  srand(time(NULL));  // Inicializa gerador de números aleatórios
  
  // Define e gera uma lista de números inteiros
  int numeros[N];
  gerador_numeros(numeros);

  // Ordena a lista de numeros
  counting_sort(numeros);

  printf("Média: %f\n", media(numeros));
  printf("Mediana: %f\n", mediana(numeros));
  printf("Desvio Padrão: %f\n", desvio_padrao(numeros));
  return 0;
}
