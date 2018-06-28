#include <stdio.h>
#include <string.h>

void remove_registo(char* nome_arq, char* chave, int ordem, Barv* arv){
	FILE* fp = fopen(nome_arq, "r");

	int NRR[ordem];
	int nseeks;
	int pos = busca_chave(fp, chave, ordem, &nseeks);

	fseek(fp, ((14*(arv->ordem - 1) + 5*arv->ordem) - 1)*pos, SEEK_SET);

	fputc('*', fp);

	libera_arvore(arv, arv->raiz);
	fclose(fp);

	ler_arquivo_cria_arvore(arv, nome_arq);

}