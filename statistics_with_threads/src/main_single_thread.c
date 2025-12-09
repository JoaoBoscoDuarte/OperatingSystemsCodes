#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 10000
#define MAX_VAL 100

double resultado_media;
double resultado_mediana;
double resultado_desvio;

// Função básica para calcular a média
double media(int numeros[]) {
  long int soma = 0;

  for (int i = 0; i < N; i++) {
    soma += numeros[i];
  }

  return (double) soma / N;
}

// Método de ordenação couting sort para ordenar a lista antes de calcular a mediana 
void counting_sort(int arr[]) {
    int contagem[MAX_VAL + 1] = {0};

    for (int i = 0; i < N; i++) {
        contagem[arr[i]]++;
    }

    int index = 0;
    for (int valor = 0; valor <= MAX_VAL; valor++) {
        for (int j = 0; j < contagem[valor]; j++) {
            arr[index++] = valor;
        }
    }
}

// Função para calcular a mediana
double mediana(int numeros[]) {
  counting_sort(numeros);
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

    double variancia = soma_quadrados / N;
    return sqrt(variancia);
}

// Gerador de 10000 numeros aleatorios em um intervalo de [0 a 100]
void gerador_numeros(int numeros[]) {
  for (int i = 0; i < N; i++) {
    numeros[i] = rand() % (MAX_VAL + 1);
  }
}

int main(void)
{
  srand(time(NULL));
  
  int numeros[N];
  // Thread principal gera a lista de números
  gerador_numeros(numeros);

  clock_t inicio = clock();

  // Calcula os resultados
  resultado_media = media(numeros);
  resultado_mediana = mediana(numeros);
  resultado_desvio = desvio_padrao(numeros);

  clock_t fim = clock();
  double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

  // Thread principal exibe os resultados
  printf("=== VERSÃO COM 1 THREAD (SEQUENCIAL) ===\n");
  printf("Média: %.2f\n", resultado_media);
  printf("Mediana: %.2f\n", resultado_mediana);
  printf("Desvio Padrão: %.2f\n", resultado_desvio);
  printf("\nTempo de execução: %.6f segundos\n", tempo_execucao);
  
  return 0;
}
