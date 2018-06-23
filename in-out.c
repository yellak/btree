#include "btree.h"
#include "in-out.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *strupr(char *str)
{
  unsigned char *p = (unsigned char *)str;

  while (*p) {
     *p = toupper(*p);
      p++;
  }

  return str;
}

void le_arquivo_cria_arvore(Barv* arv, char* nome_arq){

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