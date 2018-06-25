#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int T_CHAVE = 9;

/* Função que cria a árvore */
Barv* criar_arvore(int ordem){
  int i;
  Barv* arvore;
  arvore = (Barv*) malloc(sizeof(Barv));
  
  //Alocação da raiz da árvore
  arvore->raiz = (NoB*) malloc(sizeof(NoB));
  arvore->raiz->chaves = (char**) malloc((2*ordem - 1)*sizeof(char*));
  arvore->raiz->NRR = (int*) malloc((2*ordem - 1)*sizeof(int));
  arvore->raiz->folha = 1;
  arvore->raiz->n_nos = 0;
  arvore->raiz->filhos = (NoB**) malloc(2*ordem*sizeof(NoB*));

  for(i = 0; i < 2*ordem - 1; i++){
    arvore->raiz->chaves[i] = (char*) malloc(T_CHAVE*sizeof(char));
    arvore->raiz->filhos[i] = NULL;
  }

  arvore->raiz->filhos[i] = NULL;
  
  arvore->ordem = ordem;
  return arvore;
}

/* Função que aloca um novo nó */
NoB* aloca_no(int ordem){
  int i;
  NoB* novo = (NoB*) malloc(sizeof(NoB));
  novo->chaves = (char**) malloc((2*ordem - 1)*sizeof(char*));
  novo->NRR = (int*) malloc((2*ordem - 1)*sizeof(int));
  novo->filhos = (NoB**) malloc(2*ordem*sizeof(NoB*));
    
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

  //z é o novo nó criado com a repartição
  NoB* z = aloca_no(ordem);

  //Y é o nó que será repartido
  NoB* y = pai->filhos[i];

  z->folha = y->folha;

  //A quantidade de chaves de z será a quantidae mínima de chaves que um nó pode ter 
  z->n_nos = ordem - 1;

  //Transfere-se metade das chaves de y para z
  for(j = 0; j < ordem - 1; j++){
    strcpy(z->chaves[j], y->chaves[j + ordem]);
    z->NRR[j] = y->NRR[j + ordem];
  }

  //Caso y não seja folha transfere-se metade dos filhos de z para y
  if(!y->folha){
    for(j = 0; j < ordem; j++){
      z->filhos[j] = y->filhos[j + ordem];
    }
  }

  //A quantidade de chaves de y também será a quantidade mínima de chaves que um nó pode ter
  y->n_nos = ordem - 1;

  //Reorganiza-se os filhos do nó pai para inserir o novo filho 
  for(j = pai->n_nos; j >= i + 1; j--){
    pai->filhos[j + 1] = pai->filhos[j];
  }

  //Liga-se o novo filho ao nó pai
  pai->filhos[i + 1] = z;

  //Reorganiza-se as chave do pai para inserir a chave promovida
  for(j = pai->n_nos - 1; j >= i; j--){
    strcpy(pai->chaves[j + 1], pai->chaves[j]);
    pai->NRR[j + 1] = pai->NRR[j];
  }

  //Insere-se no nó pai, a chave promovida
  strcpy(pai->chaves[i], y->chaves[ordem - 1]);
  pai->NRR[i] = y->NRR[ordem - 1];
  pai->n_nos++;
}

void insere_arvore_naocheia(NoB* no, char* chave, int NRR, int ordem){
	
  //Numero de nós é decrementado por 1 pois em C vetores começam em 0
  int i = no->n_nos - 1;

  //Caso em que o nó é folha(não tem filhos)
	if(no->folha){

    //Percorre as chaves do final para o começo até encontrar a posição certa de inserir a chave ou chegar à ultima chave 
    //Enquanto percorre, vai passando todas as chaves para a posição à direita
		while(i >= 0 && strcmp(no->chaves[i],chave) > 0){ 
			strcpy(no->chaves[i+1], no->chaves[i]);
			no->NRR[i+1] = no->NRR[i];
			i--;
		}

    //Insere a chave na posição encontrada
		strcpy(no->chaves[i+1], chave);
		no->NRR[i+1] = NRR;
		no->n_nos++;
	}

  //Caso em que o nó não é folha
	else{

    //Econtra a posição do filho que vai ter a nova chave
		while(i >= 0 && strcmp(no->chaves[i],chave) > 0){
			i--;
		}
		i++;

    //Caso em que deseja-se inserir a nova chave em um filho cheio
		if(no->filhos[i]->n_nos == 2*ordem -1){

      //Reparte-se o filho parra caber a nova chave
			repartir_filho(no, i, ordem);

      //Decide-se quais dos dois novos filhos iremos entrar para inserir a nova chave
			if(strcmp(no->chaves[i], chave) < 0){ 
				i = i + 1;
			}			
		}

    //Insere-se a chave no filho decidido no if anterior
		insere_arvore_naocheia(no->filhos[i], chave, NRR, ordem);
	}
}

/* Função que insere uma nova chave no nó */
void inserir_btree(Barv* arv, char* chave, int NRR){
  
  NoB* r = arv->raiz;

  //Caso em que deseja-se inserir um novo nó em uma raiz vazia
  if(arv->raiz->n_nos == 0){
    strcpy(arv->raiz->chaves[0], chave);
    arv->raiz->NRR[0] = NRR;
    arv->raiz->n_nos++;
  }

  //Caso em que a raiz não é vazia
  else{

    //Caso em que o nó raiz está cheio
    if(r->n_nos == 2*arv->ordem - 1){

      //Criação de um novo nó que se tornará a nova raiz
      NoB* s = aloca_no(arv->ordem);
      arv->raiz = s;
      s->folha = 0;
      s->n_nos = 0;

      //A antiga raiz se torna filha da nova raiz
      s->filhos[0] = r;

      //Reparte-se a raiz para que ela tenha 2 filhos
      repartir_filho(s, 0, arv->ordem);

      //Insere-se o novo nó
      insere_arvore_naocheia(s, chave, NRR, arv->ordem);
    }

    //Caso em que a raiz não está cheia
    else{
      insere_arvore_naocheia(r, chave, NRR, arv->ordem);
    }
  } /* else */
}

void libera_arvore(Barv* arv, NoB* raiz){
  int i;

  if(!raiz->folha){
    for (i = 0; i < raiz->n_nos + 1; i++){
      libera_arvore(arv, raiz->filhos[i]);
    }
  }

  for(i = 0; i < 2*arv->ordem - 1; i++){
    free(raiz->chaves[i]);
  }

  free(raiz->chaves);
  free(raiz->NRR);
  free(raiz->filhos);
  free(raiz);
}
