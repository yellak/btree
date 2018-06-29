#ifndef __BTREE__H_
#define __BTREE__H_

typedef struct nob{
  char **chaves; /* Vetor de strings que contem as chaves de um nó */
  int *NRR; /* Vetor de inteiros que armazena os nrrs dos registros */
  int folha; /* Variável booleana que define se um nó é folha ou não */
  int n_nos; /* Variável que armazena o número de chaves no nó */
  struct nob** filhos; /* Vetor de ponteiros para nós filhos */
} NoB;

typedef struct barv{
  NoB* raiz; /* Ponteiro para a raíz da árvore */
  int ordem; /* Inteiro que armazena a ordem da árvore */
} Barv;

Barv* criar_arvore(int ordem);

NoB* aloca_no(int ordem);

void repartir_filho(NoB* pai, int i, int ordem);

void insere_arvore_naocheia(NoB* no, char* chave, int NRR, int ordem);

void inserir_btree(Barv* arv, char* chave, int NRR);

void libera_arvore(Barv* arv, NoB* raiz);

#endif
