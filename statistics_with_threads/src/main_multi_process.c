#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define N 10000
#define MAX_VAL 100

int numeros[N];

// Função básica para calcular a média
double media(int numeros[]) {
  long int soma = 0;
  for (int i = 0; i < N; i++) {
    soma += numeros[i];
  }
  return (double) soma / N;
}

// Método de ordenação couting sort
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

// Gerador de números aleatórios
void gerador_numeros(int numeros[]) {
  for (int i = 0; i < N; i++) {
    numeros[i] = rand() % (MAX_VAL + 1);
  }
}

int main(int argc, char const *argv[])
{
  srand(time(NULL));
  
  // Processo pai gera a lista de números
  gerador_numeros(numeros);

  // Pipes para comunicação: [0]=leitura, [1]=escrita
  int pipe_media[2], pipe_mediana[2], pipe_desvio[2];
  pipe(pipe_media);
  pipe(pipe_mediana);
  pipe(pipe_desvio);

  clock_t inicio_criacao = clock();

  // Processo 1: Calcula média
  pid_t pid1 = fork();
  if (pid1 == 0) {
    close(pipe_media[0]);
    double resultado = media(numeros);
    write(pipe_media[1], &resultado, sizeof(double));
    close(pipe_media[1]);
    exit(0);
  }

  // Processo 2: Calcula mediana
  pid_t pid2 = fork();
  if (pid2 == 0) {
    close(pipe_mediana[0]);
    int numeros_copia[N];
    memcpy(numeros_copia, numeros, N * sizeof(int));
    double resultado = mediana(numeros_copia);
    write(pipe_mediana[1], &resultado, sizeof(double));
    close(pipe_mediana[1]);
    exit(0);
  }

  // Processo 3: Calcula desvio padrão
  pid_t pid3 = fork();
  if (pid3 == 0) {
    close(pipe_desvio[0]);
    double resultado = desvio_padrao(numeros);
    write(pipe_desvio[1], &resultado, sizeof(double));
    close(pipe_desvio[1]);
    exit(0);
  }

  clock_t fim_criacao = clock();

  // Processo pai fecha escrita e lê resultados
  close(pipe_media[1]);
  close(pipe_mediana[1]);
  close(pipe_desvio[1]);

  double resultado_media, resultado_mediana, resultado_desvio;
  read(pipe_media[0], &resultado_media, sizeof(double));
  read(pipe_mediana[0], &resultado_mediana, sizeof(double));
  read(pipe_desvio[0], &resultado_desvio, sizeof(double));

  close(pipe_media[0]);
  close(pipe_mediana[0]);
  close(pipe_desvio[0]);

  // Aguarda processos filhos
  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);
  waitpid(pid3, NULL, 0);

  clock_t fim_execucao = clock();

  double tempo_criacao = ((double)(fim_criacao - inicio_criacao)) / CLOCKS_PER_SEC;
  double tempo_total = ((double)(fim_execucao - inicio_criacao)) / CLOCKS_PER_SEC;

  // Processo pai exibe os resultados
  printf("=== VERSÃO COM 3 PROCESSOS ===\n");
  printf("Média: %.2f\n", resultado_media);
  printf("Mediana: %.2f\n", resultado_mediana);
  printf("Desvio Padrão: %.2f\n", resultado_desvio);
  printf("\nTempo de criação dos processos: %.6f segundos\n", tempo_criacao);
  printf("Tempo total (criação + execução): %.6f segundos\n", tempo_total);
  
  return 0;
}
