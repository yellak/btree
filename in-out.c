#include "btree.h"
#include "in-out.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void le_arquivo_cria_arvore(Barv* arv, char* nome_arq){

	FILE* fp = fopen(nome_arq, "r");
	int i = 0;
	int j;
	char str[55];
	char chave[9];
	char ch;

	while(!feof(fp)){
		fseek(fp, 54*i, SEEK_SET);
		fgets(str, 54, fp);

		for(j = 0; j < 3; j++){
			chave[j] = str[j];
		}

		for(j = 0; j < 5; j++){
			chave[j + 3] = str[41 + j];
		}

		chave[8] = '\0';
		ch = fgetc(fp);

		inserir_btree(arv, chave, i);

		i++;
	}
	fclose(fp);
}