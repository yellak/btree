#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const int T_CHAVE = 9;

/* Função que cria a árvore */
Barv* criar_arvore(int ordem){
	int i;
	Barv* arvore;
	arvore = (Barv*) malloc(sizeof(Barv));
  
	//Alocação da raiz da árvore
	arvore->raiz = (NoB*) malloc(sizeof(NoB));
	arvore->raiz->chaves = (char**) malloc((ordem - 1)*sizeof(char*));
	arvore->raiz->NRR = (int*) malloc((ordem - 1)*sizeof(int));
	arvore->raiz->folha = 1;
	arvore->raiz->n_nos = 0;
	arvore->raiz->filhos = (NoB**) malloc(ordem*sizeof(NoB*));

	//Alocação de cada uma das strings das chaves e inicializando os filhos com NULL 
	for(i = 0; i < ordem - 1; i++){
		arvore->raiz->chaves[i] = (char*) malloc(T_CHAVE*sizeof(char));
		arvore->raiz->filhos[i] = NULL;
	}

	//Inicizalização do último filho com NULL
	arvore->raiz->filhos[i] = NULL;
  
	arvore->ordem = ordem;
	return arvore;
}

/* Função que aloca um novo nó */
NoB* aloca_no(int ordem){
	int i;
	NoB* novo = (NoB*) malloc(sizeof(NoB));
	novo->chaves = (char**) malloc((ordem - 1)*sizeof(char*));
	novo->NRR = (int*) malloc((ordem - 1)*sizeof(int));
	novo->filhos = (NoB**) malloc(ordem*sizeof(NoB*));
    
	//Alocação de cada uma das strings das chaves e inicializando os filhos com NULL 
	for(i = 0; i < ordem - 1; i++){
		novo->chaves[i] = (char*) malloc(T_CHAVE*sizeof(char));
		novo->filhos[i] = NULL;
	}
	//Inicizalização do último filho com NULL
	novo->filhos[i] = NULL;

	return novo;
}

/* Função que reparte um nó da árvore e promove uma chave */
void repartir_filho(NoB* pai, int i, int ordem){
	
	int j;

	//z é o novo nó criado com a repartição
	NoB* z = aloca_no(ordem);

	//y é o nó que será repartido
	NoB* y = pai->filhos[i];

	z->folha = y->folha;

	//A quantidade de chaves de z será a quantidae mínima de chaves que um nó pode ter 
	z->n_nos = ceil((double)ordem/2) - 1;

	//Transfere-se metade das chaves de y para z
	for(j = 0; j < (int)ceil((double)ordem/2) - 1; j++){
		strcpy(z->chaves[j], y->chaves[j + (int)(ordem/2)]);
		z->NRR[j] = y->NRR[j + (int)(ordem/2)];
	}

	//Caso y não seja folha(não tenha filhos), transfere-se metade dos filhos de z para y
	if(!y->folha){
		for(j = 0; j < (int)((ordem + 1)/2); j++){
			z->filhos[j] = y->filhos[j + (int)(ordem/2)];
		}
	}

	//A quantidade de chaves de y também será a quantidade mínima de chaves que um nó pode ter
	y->n_nos = ceil(ordem/2) - 1;

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
	strcpy(pai->chaves[i], y->chaves[(int)(ordem/2) - 1]);
	pai->NRR[i] = y->NRR[(int)(ordem/2) - 1];
	pai->n_nos++;
}

/* Função que insere uma nova chave em um nó que não está cheio */
void insere_arvore_naocheia(NoB* no, char* chave, int NRR, int ordem){

	//Numero de nós é decrementado por 1 pois em C vetores começam em 0 e precisaremos acessar a chave 0 em algum momento
	int i = no->n_nos - 1;

	//Caso em que o nó é folha(não tem filhos)
	if(no->folha){

		//Percorre as chaves do final para o começo até encontrar a posição certa de inserir a chave ou chegar à ultima chave 
		//Enquanto percorre, vai passando todas as chaves para a posição à direita
		while(i >= 0 && strcmp(no->chaves[i], chave) > 0){
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
		if(no->filhos[i]->n_nos == ordem - 1){

			//Reparte-se o filho para caber a nova chave
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

/* Função que insere uma nova chave na árvore */
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
		if(r->n_nos == arv->ordem - 1){

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

/* Função que libera todos os nós da árvore */
/* Essa função não libera a árvore em si */
void libera_arvore(Barv* arv, NoB* raiz){
	int i;

	/* Iteração para liberar todos os filhos de um nó recursivamente */
	if(!raiz->folha){
		for (i = 0; i < raiz->n_nos + 1; i++){
			libera_arvore(arv, raiz->filhos[i]);
		}
	}

	/* Loop para liberar todos as chaves de um nó */
	for(i = 0; i < arv->ordem - 1; i++){
		free(raiz->chaves[i]);
	}

	free(raiz->chaves);
	free(raiz->NRR);
	free(raiz->filhos);
	free(raiz);
}