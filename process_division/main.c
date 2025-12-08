#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
  int f1, f2, n1, n2, n3, n4;

  // P1 cria F1
  f1 = fork();

  if (f1 == 0) {
    // Processo F1 cria N1 e N2
    n1 = fork();
    
    if (n1 == 0) {
      // Processo N1 executa comando
      execl("/bin/ls", "ls", "-la", NULL);
      printf("\n");
    }

    n2 = fork();
    
    if (n2 == 0) {
      // Processo N2 executa comando
      execl("/bin/pwd", "pwd", NULL);
    }

    // F1 aguarda N1 e N2
    wait(NULL);
    wait(NULL);
    
    printf("\n--- Processo filho F1 ---\n");
    printf("PID: %d, PID do pai: %d\n", getpid(), getppid());
    printf("\n");
    return 0;
  }

  // P1 cria F2
  f2 = fork();

  if (f2 == 0) {
    // Processo F2 cria N3 e N4
    n3 = fork();
    
    if (n3 == 0) {
      // Processo N3 executa comando
      execl("/bin/sh", "sh", "-c", "top -b -n 1 | head -n 17", NULL);
    }

    n4 = fork();
    
    if (n4 == 0) {
      // Processo N4 executa comando
      execl("/bin/ps", "ps", NULL);
    }

    // F2 aguarda N3 e N4
    wait(NULL);
    wait(NULL);

    printf("\n");
    printf("\n--- Processo filho F2 ---\n");
    printf("PID: %d, PID do pai: %d\n", getpid(), getppid());
    return 0;
  }

  // P1 aguarda F1 e F2
  wait(NULL);
  wait(NULL);

  printf("\n");
  printf("Processo pai P1\n");
  printf("PID: %d\n", getpid());
  printf("Programa encerrado.\n");

  return 0;
}
