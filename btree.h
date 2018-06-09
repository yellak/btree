#ifndef __BTREE__H_
#define __BTREE__H_

const int T_CHAVE = 9;

typedef struct nob{
  char chaves**;
  int NRR*;
  int folha;
  int n_nos;
  struct nob** filhos;
} NoB;

typedef struct barv{
  NoB* raiz;
  int ordem;
} Barv;

void criar_arvore(Barv* arvore, int ordem);

NoB* aloca_no(int ordem);

void repartir_filho(NoB* pai, int i, int ordem);

void insere_arvore_naocheia(NoB* no, char* chave, int NRR, int ordem);

void inserir_btree(Barv* arv, char* chave, int NRR);

#endif
