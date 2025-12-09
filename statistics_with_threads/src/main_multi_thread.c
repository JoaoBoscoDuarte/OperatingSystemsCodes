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

// Gerador de 10000 numeros aleatorios em um intervalo de [0 a 100]
void gerador_numeros(int numeros[]) {
  for (int i = 0; i < N; i++) {
    numeros[i] = rand() % (MAX_VAL + 1);
  }
}

// Thread 1: Calcula média
void* thread_media(void* arg) {
  long int soma = 0;

  for (int i = 0; i < N; i++) {
    soma += numeros[i];
  }

  resultado_media = (double) soma / N;
  return NULL;
}

// Thread 2: Calcula mediana (ordena internamente)
void* thread_mediana(void* arg) {
    counting_sort(numeros_copia);
    resultado_mediana = (numeros_copia[4999] + numeros_copia[5000]) / 2.0;
    return NULL;
}

// Thread 3: Calcula desvio padrão
void* thread_desvio(void* arg) {
    long int soma = 0;
    for (int i = 0; i < N; i++) {
        soma += numeros[i];
    }
    double m = (double) soma / N;
    
    double soma_quadrados = 0.0;
    for (int i = 0; i < N; i++) {
        double diff = numeros[i] - m;
        soma_quadrados += diff * diff;
    }

    resultado_desvio = sqrt(soma_quadrados / N);
    return NULL;
}

int main(int argc, char const *argv[])
{
  srand(time(NULL));
  
  // Thread principal gera a lista de números
  gerador_numeros(numeros);
  memcpy(numeros_copia, numeros, N * sizeof(int));

  pthread_t t1, t2, t3;
  
  clock_t inicio_criacao = clock();
  
  // Criação das threads
  pthread_create(&t1, NULL, thread_media, NULL);
  pthread_create(&t2, NULL, thread_mediana, NULL);
  pthread_create(&t3, NULL, thread_desvio, NULL);
  
  clock_t fim_criacao = clock();
  
  // Aguarda execução das threads
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);

  clock_t fim_execucao = clock();
  
  double tempo_criacao = ((double)(fim_criacao - inicio_criacao)) / CLOCKS_PER_SEC;
  double tempo_total = ((double)(fim_execucao - inicio_criacao)) / CLOCKS_PER_SEC;

  // Thread principal exibe os resultados
  printf("=== VERSÃO COM 3 THREADS (PARALELO) ===\n");
  printf("Média: %.2f\n", resultado_media);
  printf("Mediana: %.2f\n", resultado_mediana);
  printf("Desvio Padrão: %.2f\n", resultado_desvio);
  printf("\nTempo de criação das threads: %.6f segundos\n", tempo_criacao);
  printf("Tempo total (criação + execução): %.6f segundos\n", tempo_total);
  
  return 0;
}
