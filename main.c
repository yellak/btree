#include <stdio.h>
#include <stdlib.h>	
#include "btree.h"
#include "in-out.h"

int main(){
	Barv* arv;
	char* nome_arq = "lista.txt";
	char* nome_indice = "indicelista.bt";
	FILE* fp = fopen(nome_indice, "w+");

	int ordem;
	printf("\nInsira a ordem desejada para a árvore:\n");
	scanf("%d", &ordem);
	arv = criar_arvore(ordem/2);

	/* Lendo o arquivo, criando a árvore e salvando ela */
	ler_arquivo_cria_arvore(arv, nome_arq);
	salvar_arvore(arv, arv->raiz, fp);

	/* Imprimindo o arquivo resultante na tela */
	printf("\nArquivo com a árvore-B criada:\n\n");
	ImprimirArquivo(fp);

	int escolha;
	do{
		printf("\n#######################################################\n");
		printf("Que operação você deseja fazer agora?\n");
		printf("0. Adicionar um novo registro\n");
		printf("1. Remover um registro existente\n");
		printf("2. Fazer a busca de um novo registro\n");
		printf("3. Sair\n");

		do{
			printf("Escolha uma opção válida:\n");
			scanf("%d", &escolha);
		} while(escolha < 0 || escolha > 3);

		switch(escolha)
			{
			case 0:
				insere_novo_registro(arv,nome_arq);
				salvar_arvore(arv, arv->raiz, fp);
				ImprimirArquivo(fp);
				break;
			case 1:
				printf("Qual a primeira letra do registro que você deseja excluir?\n");
				break;
			case 2:
				printf("Qual a chave do registro que você deseja pesquisar?\n");
				break;
			}
	} while(escolha != 3);

	/* Liberando todo o espaço alocado em memória */
	libera_arvore(arv, arv->raiz);
	free(arv);
	fclose(fp);
	return 0;
}
