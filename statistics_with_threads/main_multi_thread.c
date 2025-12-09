#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

#define N 10000
#define MAX_VAL 100

int numeros[N];
int numeros_copia[N];
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

// Thread 1: Calcula média
void* thread_media(void* arg) {
    resultado_media = media(numeros);
    return NULL;
}

// Thread 2: Calcula mediana (ordena internamente)
void* thread_mediana(void* arg) {
    resultado_mediana = mediana(numeros_copia);
    return NULL;
}

// Thread 3: Calcula desvio padrão
void* thread_desvio(void* arg) {
    resultado_desvio = desvio_padrao(numeros);
    return NULL;
}

int main(int argc, char const *argv[])
{
  srand(time(NULL));
  
  gerador_numeros(numeros);
  memcpy(numeros_copia, numeros, N * sizeof(int));

  clock_t inicio = clock();

  pthread_t t1, t2, t3;

  pthread_create(&t1, NULL, thread_media, NULL);
  pthread_create(&t2, NULL, thread_mediana, NULL);
  pthread_create(&t3, NULL, thread_desvio, NULL);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);

  clock_t fim = clock();
  double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

  printf("=== VERSÃO COM 3 THREADS (PARALELO) ===\n");
  printf("Média: %.2f\n", resultado_media);
  printf("Mediana: %.2f\n", resultado_mediana);
  printf("Desvio Padrão: %.2f\n", resultado_desvio);
  printf("Tempo de execução: %.6f segundos\n", tempo_execucao);
  
  return 0;
}
