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

void insere_arvore_naocheia(NoB* no, char* chave, int NRR, int ordem){
	int i = no->n_nos;
	if(no->folha){
		while(i >= 0 && strcmp(no->chave[i],chave) > 0){ //Isso pode estar errado
			strcpy(no->chave[i+1], no->chave[i]);
			no->NRR[i+1] = no->NRR[i];
			i--;
		}
		strcpy(no->chave[i+1], chave);
		no->NRR[i+1] = NRR;
		no->n_nos++;
	}
	else{
		while(i >= 0 && strcmp(no->chave[i],chave) > 0){ //Isso pode estar errado
			i--;
		}
		i++;
		if(no->filhos[i]->n_nos == 2*ordem -1){
			repartir_filho(no, i, ordem);
			if(strcmp(no->chave[i],chave) < 0){ //Pode estar errado tbm
				i = i + 1;
			}			
		}
		insere_arvore_naocheia(no->filhos[i], chave, NRR, ordem);
	}
}