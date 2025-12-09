# Estatísticas com Threads e Processos

**Autor:** Bosco  
**Disciplina:** Sistemas Operacionais - 2024

---

## Sumário

1. [Objetivo](#objetivo)
2. [Estrutura do Projeto](#estrutura-do-projeto)
3. [Como Executar](#como-executar)
4. [Implementação](#implementação)
5. [Resultados e Análise](#resultados-e-análise)
6. [Conclusões](#conclusões)

---

## Objetivo

Este projeto implementa e compara o desempenho de **threads** e **processos** para cálculos estatísticos sobre uma lista de 10.000 números inteiros no intervalo [0,100].

São calculadas três estatísticas:
- **Média aritmética**
- **Mediana** (valor central após ordenação)
- **Desvio padrão**

Quatro versões são implementadas:
1. **1 Thread (Sequencial)** - Execução sequencial em uma única thread
2. **1 Processo (Sequencial)** - Execução sequencial em um único processo
3. **3 Threads (Paralelo)** - Três threads paralelas usando `pthread`
4. **3 Processos (Paralelo)** - Três processos paralelos usando `fork()` e pipes

---

## Estrutura do Projeto

```
statistics_with_threads/
├── src/                          # Código fonte
│   ├── main_single_thread.c      # Versão sequencial com 1 thread
│   ├── main_single_process.c     # Versão sequencial com 1 processo
│   ├── main_multi_thread.c       # Versão paralela com 3 threads
│   ├── main_multi_process.c      # Versão paralela com 3 processos
│   └── makefile                  # Compilação automatizada
├── bin/                          # Binários compilados
│   ├── single_thread
│   ├── single_process
│   ├── multi_thread
│   └── multi_process
├── benchmark/                    # Resultados dos testes
│   └── resultados.csv            # Dados de benchmark em CSV
├── benchmark.sh                  # Script de benchmark (20 execuções)
└── README.md                     # Este arquivo
```

---

## Como Executar

### Passo 1: Compilar

```bash
cd src
make
cd ..
```

Isso compila todos os programas e coloca os binários na pasta `bin/`.

### Passo 2: Executar Individualmente

```bash
./bin/single_thread    # Versão sequencial com 1 thread
./bin/single_process   # Versão sequencial com 1 processo
./bin/multi_thread     # Versão paralela com 3 threads
./bin/multi_process    # Versão paralela com 3 processos
```

### Passo 3: Executar Benchmark Completo

```bash
./benchmark.sh
```

Este script:
- Executa cada versão **20 vezes**
- Extrai apenas os **tempos de execução**
- Salva os resultados em `benchmark/resultados.csv`
- Formato: `single_thread,multi_thread,single_process,multi_process`

### Passo 4: Limpar Binários

```bash
cd src
make clean
cd ..
```

---

## Implementação

### 1. Versão com 1 Thread (Sequencial)

**Arquivo:** `main_single_thread.c`

**Características:**
- Usa `pthread_create()` para criar **1 thread** que executa todos os cálculos sequencialmente
- Thread principal gera os 10.000 números aleatórios
- Thread secundária executa:
  1. Cálculo da média
  2. Ordenação (counting sort) e cálculo da mediana
  3. Cálculo do desvio padrão
- **Comunicação:** Variáveis globais compartilhadas
- **Sincronização:** `pthread_join()` aguarda a thread terminar
- **Medição:** Tempo total de execução

**Fluxo:**
```
Main Thread → Gera números → Cria Thread → Thread calcula tudo → Join → Exibe resultados
```

---

### 2. Versão com 1 Processo (Sequencial)

**Arquivo:** `main_single_process.c`

**Características:**
- Execução **totalmente sequencial** em um único processo
- Não usa threads nem processos filhos
- Executa no processo principal:
  1. Geração dos números
  2. Cálculo da média
  3. Ordenação (counting sort) e cálculo da mediana
  4. Cálculo do desvio padrão
- **Comunicação:** Variáveis locais
- **Sincronização:** Não necessária
- **Medição:** Tempo total de execução

**Fluxo:**
```
Main Process → Gera números → Calcula média → Calcula mediana → Calcula desvio → Exibe
```

---

### 3. Versão com 3 Threads (Paralelo)

**Arquivo:** `main_multi_thread.c`

**Características:**
- Usa `pthread_create()` para criar **3 threads paralelas**
- Thread principal gera os números e cria cópia para evitar race condition
- **Thread 1:** Calcula a média (lê array original)
- **Thread 2:** Ordena cópia (counting sort) e calcula a mediana
- **Thread 3:** Calcula o desvio padrão (lê array original)
- **Comunicação:** Variáveis globais compartilhadas (`resultado_media`, `resultado_mediana`, `resultado_desvio`)
- **Sincronização:** `pthread_join()` aguarda todas as threads
- **Medição:** Tempo de criação das threads e tempo total

**Fluxo:**
```
Main Thread → Gera números → Cria 3 Threads paralelas
                              ├─ Thread 1: Média
                              ├─ Thread 2: Mediana
                              └─ Thread 3: Desvio
            → Join (aguarda) → Exibe resultados
```

---

### 4. Versão com 3 Processos (Paralelo)

**Arquivo:** `main_multi_process.c`

**Características:**
- Usa `fork()` para criar **3 processos filhos paralelos**
- Processo pai gera os números (compartilhados via fork)
- **Processo 1:** Calcula a média
- **Processo 2:** Cria cópia local, ordena (counting sort) e calcula a mediana
- **Processo 3:** Calcula o desvio padrão
- **Comunicação:** 3 pipes unidirecionais (IPC - Inter-Process Communication)
  - `pipe_media[2]`: Processo 1 → Pai
  - `pipe_mediana[2]`: Processo 2 → Pai
  - `pipe_desvio[2]`: Processo 3 → Pai
- **Sincronização:** `waitpid()` aguarda todos os processos filhos
- **Medição:** Tempo de criação dos processos e tempo total

**Fluxo:**
```
Processo Pai → Gera números → Cria 3 Pipes → Fork 3 processos
                                              ├─ Filho 1: Média → Pipe
                                              ├─ Filho 2: Mediana → Pipe
                                              └─ Filho 3: Desvio → Pipe
             → Lê dos Pipes → Waitpid (aguarda) → Exibe resultados
```

---

### Comparação das Implementações

| Aspecto | 1 Thread | 1 Processo | 3 Threads | 3 Processos |
|---------|----------|------------|-----------|-------------|
| **Paralelismo** | Não | Não | Sim | Sim |
| **Criação** | 1 thread | Nenhuma | 3 threads | 3 processos (fork) |
| **Memória** | Compartilhada | Local | Compartilhada | Isolada (cópia) |
| **Comunicação** | Variáveis globais | Variáveis locais | Variáveis globais | Pipes (IPC) |
| **Sincronização** | pthread_join | - | pthread_join | waitpid |
| **Overhead** | Mínimo | Mínimo | Médio | Alto |

---

## Resultados e Análise

### Tabela de Comparação (Tempos Médios)

| Versão | Tempo Criação (s) | Tempo Total (s) | Speedup |
|--------|-------------------|-----------------|---------|
| **1 Thread** | - | 0.000097 | 1.00x (baseline) |
| **1 Processo** | - | 0.000099 | 0.98x |
| **3 Threads** | 0.000207 | 0.000392 | 0.25x |
| **3 Processos** | 0.000305 | 0.000346 | 0.28x |

### Análise dos Resultados

#### 1. Versões Sequenciais (1 Thread vs 1 Processo)

Os tempos são praticamente **idênticos** (~0.000098s), pois ambas executam as mesmas operações sequencialmente sem overhead de paralelização.

#### 2. Versões Paralelas são MAIS LENTAS!

**Resultado surpreendente:** Ambas as versões paralelas (3 threads e 3 processos) são **4x mais lentas** que as sequenciais!

**Por quê?**

1. **Tarefas muito rápidas:** Com N=10.000, cada cálculo leva microsegundos
2. **Overhead de criação:** Criar threads/processos custa mais que executar sequencialmente
3. **Overhead de sincronização:** `pthread_join()` e `waitpid()` adicionam latência
4. **Context switching:** Trocar entre threads/processos tem custo de CPU
5. **Cache misses:** Paralelização pode causar mais falhas de cache

#### 3. Threads vs Processos (Paralelo)

**3 Processos são ligeiramente mais rápidos que 3 Threads** (0.000346s vs 0.000392s)

**Análise:**
- **Criação:** Processos são mais lentos (0.000305s vs 0.000207s)
- **Execução:** Processos compensam com melhor isolamento de memória
- **Comunicação:** Pipes são eficientes para dados pequenos (3 doubles)

#### 4. Quando Paralelismo Compensa?

Paralelismo seria vantajoso com:
- **N muito maior** (ex: 10.000.000 elementos)
- **Operações mais pesadas** (ex: cálculos complexos por elemento)
- **I/O bound tasks** (ex: leitura de múltiplos arquivos)
- **Múltiplos núcleos de CPU** disponíveis

---

## Conclusões

1. **Para tarefas pequenas e rápidas, a execução sequencial é mais eficiente** devido ao overhead de paralelização

2. **Threads têm menor overhead de criação** (0.000207s) que processos (0.000305s)

3. **Processos podem ser mais rápidos na execução total** quando o isolamento de memória beneficia o cache

4. **A escolha entre threads e processos depende do contexto:**
   - **Threads:** Melhor para compartilhamento de dados e menor overhead
   - **Processos:** Melhor para isolamento, segurança e estabilidade

5. **Paralelismo só compensa quando o ganho de execução paralela supera o overhead de criação e sincronização**

6. **Lei de Amdahl:** O speedup é limitado pela fração sequencial do código

---

## Referências

- TANENBAUM, A. S. Sistemas Operacionais Modernos. 4ª ed.
- SILBERSCHATZ, A. et al. Fundamentos de Sistemas Operacionais. 9ª ed.
- POSIX Threads Programming: https://computing.llnl.gov/tutorials/pthreads/
- Linux Manual Pages: fork(2), pipe(2), pthread_create(3)
