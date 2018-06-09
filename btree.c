#include "btree.h"
#include <stdio.h>
#include <stdlib.h>

/* Função que cria a árvore */
void criar_arvore(Barv* arvore, int ordem){
  arvore = (Barv*) malloc(sizeof(Barv));
  
  arvore->raiz = (NoB*) malloc(sizeof(NoB));
  arvore->raiz->chaves = NULL;
  arvore->raiz->NRR = NULL;
  arvore->raiz->folha = 1;
  arvore->raiz->n_nos = 0;
  arvore->raiz->nob = NULL;
  
  arvore->ordem = ordem;
}

NoB* aloca_no(int ordem){
  int i;
  NoB* novo = (NoB*) malloc(sizeof(NoB));
  novo->chaves = (char**) malloc((ordem - 1)*sizeof(char*));
  novo->NRR = (int*) malloc((ordem - 1)*sizeof(int));
  novo->filhos = (NoB*) malloc(ordem*sizeof(NoB));
    
  for(i = 0; i < ordem - 1; i++){
    novo->chaves[i] = (char*) malloc(T_CHAVE*sizeof(char));
    strcpy(novo->chaves[i], "zzzzzzzz");
    novo->NRR[i] = -1;
    novo->filhos[i] = NULL;
  }

  novo->filhos[i] = NULL;
}
