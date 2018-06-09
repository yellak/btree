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

#endif
