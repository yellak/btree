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

/* Função que aloca um novo nó */
NoB* aloca_no(int ordem){
  int i;
  NoB* novo = (NoB*) malloc(sizeof(NoB));
  novo->chaves = (char**) malloc((2*ordem - 1)*sizeof(char*));
  novo->NRR = (int*) malloc((2*ordem - 1)*sizeof(int));
  novo->filhos = (NoB*) malloc(2*ordem*sizeof(NoB));
    
  for(i = 0; i < 2*ordem - 1; i++){
    novo->chaves[i] = (char*) malloc(T_CHAVE*sizeof(char));
    //strcpy(novo->chaves[i], "zzzzzzzz");
    //novo->NRR[i] = -1;
    novo->filhos[i] = NULL;
  }

  novo->filhos[i] = NULL;

  return novo;
}

/* Função que parte uma árvore */
void repartir_filho(NoB* pai, int i, int ordem){
  int j;
  NoB* z = aloca_no(ordem);
  NoB* y = pai->filhos[i];

  z->folha = y->folha;
  z->n_nos = ordem - 1;

  for(j = 0; j < ordem - 1; j++){
    strcpy(z->chaves[j], y->chaves[j + ordem]);
    z->NRR[j] = y->NRR[j + ordem];
  }

  if(!y->folha){
    for(j = 0; j < ordem; j++){
      z->filhos[j] = y->filhos[j + ordem];
    }
  }

  y->n_nos = ordem - 1;

  for(j = pai->n_nos; j >= i + 1; j--){
    pai->filhos[j + 1] = pai->filhos[j];
  }

  x->filhos[i + 1] = z;

  for(j = pai->n_nos; j >= i; j--){
    strcpy(pai->chaves[j + 1], pai->chaves[j]);
  }

  strcpy(pai->chaves[i], y->chaves[ordem - 1]);
  pai->n_nos++;
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

/* Função que insere uma nova chave no nó */
void inserir_btree(Barv* arv, char* chave, int NRR){
  NoB* r = arv->raiz;

  if(r->n_nos == 2*arv->ordem - 1){
    NoB* s = aloca_no(arv->ordem);
    arv->raiz = s;
    s->folha = 0;
    s->n_nos = 0;
    s->filhos[0] = r;

    repartir_filho(s, 0, arv->ordem);
    insere_arvore_naocheia(s, chave, NRR, arv->ordem);
  }
  else{
    insere_arvore_naocheia(r, chave, NRR, arv->ordem);
  }
}