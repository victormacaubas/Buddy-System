//O segundo trecho de código é um arquivo de cabeçalho (header) chamado "buddy.h". Ele contém as declarações das funções e a definição da estrutura struct buddy, que foram utilizadas no primeiro trecho de código.Essas declarações permitem que outras partes do código possam utilizar as funções e estruturas do Buddy System sem precisar conhecer sua implementação interna. 
#ifndef BUDDY_H

#define BUDDY_H
#include <stdio.h>
#include <stdlib.h>

struct buddy;//É a definição da estrutura que representa o sistema Buddy. A implementação completa da estrutura não é fornecida neste arquivo, apenas a declaração é feita. A definição completa está no arquivo ".c".

struct buddy *buddy_new(unsigned num_of_fragments);//Declaração da função responsável por criar uma nova instância do Buddy System. Ela recebe como argumento o número de fragmentos de memória a serem gerenciados e retorna um ponteiro para a estrutura struct buddy.
int buddy_alloc(struct buddy *self, size_t size);//Declaração da função responsável por alocar um bloco de memória do tamanho especificado no Buddy System. Ela recebe um ponteiro para a estrutura struct buddy e o tamanho desejado do bloco, e retorna o deslocamento (offset) do bloco alocado.
void buddy_free(struct buddy *self, int offset);//Declaração da função responsável por liberar um bloco de memória previamente alocado. Ela recebe um ponteiro para a estrutura struct buddy e o deslocamento (offset) do bloco a ser liberado.
void buddy_dump(struct buddy *self);// Declaração da função responsável por imprimir uma representação visual da árvore de alocação do Buddy System e os tamanhos máximos dos blocos disponíveis. Ela recebe um ponteiro para a estrutura struct buddy.
int buddy_size(struct buddy *self, int offset);//Declaração da função responsável por retornar o tamanho de um bloco de memória alocado com base no deslocamento (offset) fornecido. Ela recebe um ponteiro para a estrutura struct buddy e o deslocamento do bloco alocado.

#endif /* end of include guard: BUDDY_H */
