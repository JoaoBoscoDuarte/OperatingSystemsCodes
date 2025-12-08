# Divisão de Processos em Árvore

## Enunciado da Questão

Faça um programa que divida em 7 processos formando uma árvore de acordo com a figura. Cada processo de N1 a N4 deve executar um comando diferente do terminal do Linux a sua escolha (usando o comando `execl` do `unistd.h`). 

Quando todos os processos N terminarem, os processos F1 e F2 deverão imprimir uma mensagem indicando que eles são processos filhos, e imprimir seus PID e o do seu pai P1. Apenas quando eles terminarem, o processo P1 deve imprimir a mensagem que ele é o pai e seu PID, seguida de uma mensagem de encerramento do programa.

Comente sobre a ordem de execução dos processos N1 - N4.

## Descrição
Programa em C que cria uma árvore de 7 processos utilizando `fork()` e executa comandos Linux em processos folha usando `execl()`.

## Estrutura da Árvore
```
        P1 (Processo Pai)
       /  \
      F1   F2 (Processos Filhos)
     / \   / \
    N1 N2 N3 N4 (Processos Netos - executam comandos)
```

## Lógica do Código

### 1. Criação dos Processos
- **P1** (processo principal) cria **F1** usando `fork()`
- **F1** cria **N1** e **N2** usando `fork()`
- **P1** cria **F2** usando `fork()`
- **F2** cria **N3** e **N4** usando `fork()`

### 2. Execução dos Comandos
Cada processo neto (N1-N4) executa um comando Linux diferente usando `execl()`:

- **N1**: `ls -la` - Lista todos os arquivos do diretório atual com detalhes
- **N2**: `pwd` - Exibe o caminho completo do diretório atual
- **N3**: `top -b -n 1 | head -n 17` - Mostra os 10 primeiros processos do sistema
- **N4**: `ps` - Lista os processos em execução

### 3. Sincronização com wait()
A função `wait(NULL)` garante a ordem de execução:

1. **F1** aguarda N1 e N2 terminarem antes de imprimir sua mensagem
2. **F2** aguarda N3 e N4 terminarem antes de imprimir sua mensagem
3. **P1** aguarda F1 e F2 terminarem antes de imprimir a mensagem final

### 4. Ordem de Execução
```
N1, N2, N3, N4 executam (ordem não-determinística)
    ↓
F1 imprime (após N1 e N2)
F2 imprime (após N3 e N4)
    ↓
P1 imprime e encerra
```

## Comandos Utilizados

### N1: `ls -la`
- **ls**: Lista arquivos e diretórios
- **-l**: Formato longo (permissões, proprietário, tamanho, data)
- **-a**: Mostra arquivos ocultos (que começam com .)

### N2: `pwd`
- **pwd**: Print Working Directory
- Exibe o caminho absoluto do diretório atual

### N3: `top -b -n 1 | head -n 17`
- **top**: Mostra processos em execução em tempo real
- **-b**: Modo batch (não-interativo)
- **-n 1**: Executa apenas 1 iteração
- **| head -n 17**: Pega apenas as primeiras 17 linhas (cabeçalho + 10 processos)

### N4: `ps`
- **ps**: Process Status
- Lista os processos em execução no terminal atual

## Funções Utilizadas

### fork()
Cria um novo processo (filho) duplicando o processo atual (pai).
- Retorna 0 no processo filho
- Retorna PID do filho no processo pai

### execl()
Substitui o processo atual por um novo programa.
- Formato: `execl(caminho, arg0, arg1, ..., NULL)`
- Se bem-sucedido, nunca retorna (processo é substituído)

### wait()
Faz o processo pai aguardar a finalização de um processo filho.
- Evita processos zumbis
- Garante sincronização na ordem de execução

### getpid()
Retorna o PID (Process ID) do processo atual.

### getppid()
Retorna o PID do processo pai (Parent Process ID).

## Compilação e Execução

```bash
# Compilar
gcc main.c -o main

# Executar
./main
```

## Observações sobre a Ordem de Execução dos Processos N1-N4

A ordem de execução dos processos N1, N2, N3 e N4 é **não-determinística**, ou seja, não é possível prever qual processo executará primeiro. Isso ocorre porque:

1. **Escalonamento do SO**: O sistema operacional decide qual processo executar com base em seu algoritmo de escalonamento
2. **Concorrência**: Os 4 processos podem executar simultaneamente em sistemas multi-core
3. **Fatores externos**: Carga do sistema, prioridades e outros processos influenciam a ordem

Portanto, em diferentes execuções, a ordem de saída dos comandos pode variar.
