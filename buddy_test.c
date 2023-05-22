#include "buddy.h"//Inclui o arquivo de cabeçalho "buddy.h" para ter acesso às declarações das funções e estruturas do Buddy System.
#include <stdio.h>//
#include <stdlib.h>
int main() {
  char cmd[80];//Declaração de uma string chamada "cmd" com tamanho de 80 caracteres. Ela será utilizada para armazenar os comandos fornecidos pelo usuário.
  int arg;//Declaração de uma variável inteira chamada "arg". Ela será utilizada para armazenar o argumento fornecido pelo usuário.
  struct buddy* buddy = NULL;//Declaração de um ponteiro para a estrutura struct buddy chamado "buddy". Inicialmente, é atribuído o valor NULL.
  buddy = buddy_new(64);//Cria uma nova instância do Buddy System com 64 fragmentos de memória e atribui o ponteiro retornado à variável "buddy".
  buddy_dump(buddy);//Imprime a representação visual da árvore de alocação do Buddy System e os tamanhos máximos dos blocos disponíveis.
  //O programa entra em um loop infinito utilizando for (;;). Isso significa que ele continuará executando até ser interrompido.
  for (;;) {
    scanf("%s %d", cmd, &arg);// Lê dois valores fornecidos pelo usuário: uma string (comando) armazenada em "cmd" e um inteiro (argumento) armazenado em "arg".

Em seguida, o programa verifica qual comando foi fornecido pelo usuário usando strcmp para comparar as strings.
    if (strcmp(cmd, "alloc") == 0) {//Se o comando for "alloc", ele chama buddy_alloc para alocar um bloco de memória do tamanho especificado pelo argumento "arg". O deslocamento (offset) do bloco alocado é impresso na tela e a representação visual da árvore de alocação é atualizada com buddy_dump.
      printf("allocated@%d\n", buddy_alloc(buddy, arg));
      buddy_dump(buddy);
    } else if (strcmp(cmd, "free") == 0) {//Se o comando for "free", ele chama buddy_free para liberar o bloco de memória no deslocamento especificado pelo argumento "arg". A representação visual da árvore de alocação é atualizada com buddy_dump.
      buddy_free(buddy, arg);
      buddy_dump(buddy);
    } else if (strcmp(cmd, "size") == 0) {//Se o comando for "size", ele chama buddy_size para obter o tamanho do bloco de memória no deslocamento especificado pelo argumento "arg". O tamanho é impresso na tela e a representação visual da árvore de alocação é atualizada com buddy_dump.
      printf("size: %d\n", buddy_size(buddy, arg));
      buddy_dump(buddy);
    } else
      buddy_dump(buddy);//Apenas a representação visual da árvore de alocação é impressa na tela com buddy_dump.
  }
}

