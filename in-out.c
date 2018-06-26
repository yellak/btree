#include "btree.h"
#include "in-out.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *strupr(char *str){
	unsigned char *p = (unsigned char *)str;

	while (*p) {
		*p = toupper(*p);
		p++;
	}

	return str;
}

void ler_arquivo_cria_arvore(Barv* arv, char* nome_arq){

	FILE* fp = fopen(nome_arq, "r");
	int NRR = 0;
	int j;
	char str[55];
	char* chave = (char*) malloc(9*sizeof(char));
	char ch;

	while(!feof(fp)){
		fseek(fp, 54*NRR, SEEK_SET);
		fgets(str, 54, fp);

		for(j = 0; j < 3; j++){
			chave[j] = str[j];
		}

		chave = strupr(chave);

		for(j = 0; j < 5; j++){
			chave[j + 3] = str[41 + j];
		}

		chave[8] = '\0';

		inserir_btree(arv, chave, NRR);
		
		ch = fgetc(fp);
		ch = fgetc(fp);

		NRR++;
	}
	fclose(fp);
	free(chave);
}

/* Função que salva os índices */
int salvar_arvore(Barv* arv, NoB* raiz, FILE* fp){
	NoB* aux;
	int i = 0;
	int minha_posicao;
	int NRR_filhos[arv->ordem];

	while((raiz->filhos[i] != NULL) && (i < raiz->n_nos + 1)){
		NRR_filhos[i] = salvar_arvore(arv, raiz->filhos[i], fp);
		i++;
	}
  
	for( ; i < arv->ordem; i++){
		NRR_filhos[i] = -1;
	}

	minha_posicao = ftell(fp)/50;

	for(i = 0; i < arv->ordem - 1; i++){
		if(i < raiz->n_nos){
			fprintf(fp, "%s %4d ", raiz->chaves[i], raiz->NRR[i]);
		}
		else{
			fprintf(fp, "************* ");
		}
	}

	for(i = 0; i < arv->ordem; i++){
		fprintf(fp, "%4d ", NRR_filhos[i]);
	}

	fseek(fp, -1, SEEK_CUR);
	fprintf(fp, "\n");

	return minha_posicao;
}
