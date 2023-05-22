/* buddy.c
 *
 * Description:
 *   Implement buddy system for memory management
 *
 * Idea & part of the code are from https://github.com/wuwenbin/buddy2 
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "buddy.h"

struct buddy {
    size_t size;
    size_t longest[1];
};
// Funções auxiliares inline para calcular o filho esquerdo, filho direito e pai de um nó na árvore de alocação 
static inline int left_child(int index)
{
    
    return ((index << 1) + 1); // index * 2 + 1
}

static inline int right_child(int index)
{
   
    return ((index << 1) + 2); // index * 2 + 2
}

static inline int parent(int index)
{
   
    return (((index+1)>>1) - 1); // (index+1)/2 - 1
}
// Verifica se o número é uma potência de 2
static inline bool is_power_of_2(int index)
{
    return !(index & (index - 1)); 
}
// Macros para retornar o máximo e o mínimo entre dois valores
#define max(a, b) (((a)>(b))?(a):(b))
#define min(a, b) (((a)<(b))?(a):(b))

// Função wrapper para alocar memória usando malloc
static void *b_malloc(size_t size)
{
    void *tmp = NULL;

    tmp = malloc(size);
    if (tmp == NULL) {
        fprintf(stderr, "my_malloc: not enough memory, quit\n");
        exit(EXIT_FAILURE);
    }

    return tmp;
}

// Função wrapper para liberar memória usando free. Ela libera a memória alocada anteriormente
static void b_free(void *addr)
{
    free(addr);
}
// Função para calcular a próxima potência de 2 para um dado número
static inline unsigned next_power_of_2(unsigned size)
{
    /* depend on the fact that size < 2^32 */
    size -= 1;
    size |= (size >> 1);
    size |= (size >> 2);
    size |= (size >> 4);
    size |= (size >> 8);
    size |= (size >> 16);
    return size + 1;
}
//é usada para criar uma nova instância do Buddy System, alocando memória para a estrutura struct buddy e inicializando o array longest.
/** allocate a new buddy structure 
 * @param num_of_fragments number of fragments of the memory to be managed 
 * @return pointer to the allocated buddy structure */
struct buddy *buddy_new(unsigned num_of_fragments)
{
    struct buddy *self = NULL;
    size_t node_size;

    int i;

    if (num_of_fragments < 1 || !is_power_of_2(num_of_fragments)) {
        return NULL;
    }

    // Aloca uma estrutura buddy que representa uma árvore binária completa
    self = (struct buddy *) b_malloc(sizeof(struct buddy) 
                                     + 2 * num_of_fragments * sizeof(size_t));
    self->size = num_of_fragments;
    node_size = num_of_fragments * 2;
    
    // Inicializa o array *longest* para a estrutura buddy
    int iter_end = num_of_fragments * 2 - 1;
    for (i = 0; i < iter_end; i++) {
        if (is_power_of_2(i+1)) {
            node_size >>= 1;
        }
        self->longest[i] = node_size;
    }

    return self;
}
//  Esta função é responsável por liberar a memória alocada para a estrutura do sistema buddy. Ela chama b_free para liberar a memória apontada por self.
void buddy_destory(struct buddy *self)
{
    b_free(self);
}

// Esta função é usada internamente para escolher o nó filho com o menor valor "longest" que ainda é maior que o tamanho solicitado. Ela recebe como entrada o ponteiro self para a estrutura do buddy system, o índice do nó atual e o tamanho desejado. Ela compara os valores "longest" dos nós filhos esquerdo e direito e seleciona aquele com o menor valor. Se o tamanho do filho selecionado ainda for maior que o tamanho solicitado, ela escolhe o outro filho. A função retorna o índice do filho escolhido.
unsigned choose_better_child(struct buddy *self, unsigned index, size_t size)
{
    struct compound {
        size_t size;
        unsigned index;
    } children[2];
    children[0].index = left_child(index);
    children[0].size = self->longest[children[0].index];
    children[1].index = right_child(index);
    children[1].size = self->longest[children[1].index];

    int min_idx = (children[0].size <= children[1].size) ? 0: 1;

    if (size > children[min_idx].size) {
        min_idx = 1 - min_idx;
    }
    
    return children[min_idx].index;
}

// Esta função é usada para alocar um bloco de memória do tamanho especificado a partir do sistema buddy representado por self. Ela retorna o deslocamento a partir do início da memória gerenciada onde o bloco alocado começa. Se a alocação falhar, ela retorna -1. A função primeiro verifica se a estrutura do buddy system é válida e se o tamanho solicitado está dentro do tamanho de memória disponível. Em seguida, encontra o nó apropriado na árvore da estrutura buddy para alocar o bloco de memória. Ela seleciona iterativamente os nós filhos com base na função choose_better_child até encontrar um nó com um tamanho igual ao tamanho solicitado. Ela atualiza o valor "longest" do nó selecionado para 0 e ajusta os valores "longest" dos nós pais. Por fim, calcula o deslocamento do bloco alocado e o retorna.
int buddy_alloc(struct buddy *self, size_t size)
{
    if (self == NULL || self->size < size) {
        return -1;
    }
    size = next_power_of_2(size);

    unsigned index = 0;
    if (self->longest[index] < size) {
        return -1;
    }

    /* search recursively for the child */
    unsigned node_size = 0;
    for (node_size = self->size; node_size != size; node_size >>= 1) {
        /* choose the child with smaller longest value which is still larger
         * than *size* */
        /* TODO */
        index = choose_better_child(self, index, size);
    }

    /* update the *longest* value back */
    self->longest[index] = 0;
    int offset = (index + 1)*node_size - self->size;

    while (index) {
        index = parent(index);
        self->longest[index] = 
            max(self->longest[left_child(index)],
                self->longest[right_child(index)]);
    }

    return offset;
}
//Esta função é usada para liberar um bloco de memória alocado anteriormente no sistema buddy. Ela recebe o ponteiro self para a estrutura do buddy system e o deslocamento do bloco a ser liberado. A função verifica primeiro se a estrutura do buddy system é válida e se o deslocamento está dentro do intervalo da memória gerenciada. Em seguida, percorre a árvore para encontrar o nó correspondente ao deslocamento fornecido. Ela atualiza o valor "longest" do nó para o tamanho do nó correspondente e ajusta os valores "longest" dos nós pais.
void buddy_free(struct buddy *self, int offset)
{
    if (self == NULL || offset < 0 || offset > self->size) {
        return;
    }

    size_t node_size;
    unsigned index;

    /* get the corresponding index from offset */
    node_size = 1;
    index = offset + self->size - 1;

    for (; self->longest[index] != 0; index = parent(index)) {
        node_size <<= 1;    /* node_size *= 2; */

        if (index == 0) {
            break;
        }
    }

    self->longest[index] = node_size;

    while (index) {
        index = parent(index);
        node_size <<= 1;

        size_t left_longest = self->longest[left_child(index)];
        size_t right_longest = self->longest[right_child(index)];

        if (left_longest + right_longest == node_size) {
            self->longest[index] = node_size;
        } else {
            self->longest[index] = max(left_longest, right_longest);
        }
    }
}
//Esta função é usada para imprimir uma representação visual da árvore do sistema buddy e dos valores "longest" de cada nó. Ela imprime os valores "longest" em um formato tabular, mostrando os níveis e os tamanhos dos nós correspondentes. Também visualiza o status de alocação de cada nó usando os caracteres "-", "/" e "".
void buddy_dump(struct buddy *self)
{
    int len = self->size << 1;
    int max_col = self->size << 1; 
    int level = 0;
    int i,j;

    char cs[] = {'/', '\\'};
    int idx = 0;
    char c;

    for (i = 0, max_col=len, level=0; i < len-1; i++) {
        if (is_power_of_2(i+1)) {
            max_col >>= 1;
            level ++;
            idx = 0;
            printf("\n%d(%.2d): ", level, max_col);
        }

        printf("%*d", max_col, self->longest[i]);
    }

    for (i = 0, max_col=len, level=0; i < len-1; i++) {
        if (is_power_of_2(i+1)) {
            max_col >>= 1;
            level ++;
            idx = 0;
            printf("\n%d(%.2d): ", level, max_col);
        }

        if (self->longest[i] > 0) {
            c = '-';
        } else {
            c = cs[idx];
            idx ^= 0x1;
        }

        for (j = 0; j < max_col; j++) {
            printf("%c", c);
        }
    }
    printf("\n");
}
//Esta função retorna o tamanho do bloco de memória alocado no deslocamento
int buddy_size(struct buddy *self, int offset)
{
    unsigned node_size = 1;
    unsigned index = offset + self->size - 1;

    for (; self->longest[index]; index = parent(index)) {
        node_size >>= 1;
    }

    return node_size;
}
