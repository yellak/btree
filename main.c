#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "btree.h"
#include "in-out.h"

void inicio(void)
{
	printf("#################################################\n");
	printf("#     Trabalho 2 - Organização de arquivos      #\n");
	printf("#################################################\n");
}

int main(){
	Barv* arv;
	char* nome_arq = "lista.txt";
	char* nome_indice = "indicelista.bt";
	FILE* fp = fopen(nome_indice, "w+");

	inicio();
	
	int ordem;
	printf("\nInsira a ordem desejada para a árvore:\n");
	scanf("%d", &ordem);
	while(ordem < 3){
		printf("Insira uma ordem maior que 2:\n");
		scanf("%d", &ordem);
	}

	/* O algoritmo usado cria a árvore baseado na quantidade mínima de filhos
       que um nó pode ter, por isso é mandado a quantidade mínima de filhos
       para a função */
	arv = criar_arvore(ordem);

	/* Lendo o arquivo, criando a árvore e salvando ela */
	ler_arquivo_cria_arvore(arv, nome_arq);
	salvar_arvore(arv, arv->raiz, fp);

	/* Imprimindo o arquivo resultante na tela */
	printf("\nArquivo com a árvore-B criada:\n\n");
	printf("Estrutura:\n");
	printf("-------------------------------------------------------------------------\n");
	printf("chave1 NRR1 chave2 NRR2 ... Ponteiros para os filhos no arquivo .bt\n");
	printf("-------------------------------------------------------------------------\n\n");
	printf("Observações:\n");
	printf("NRR é a posição da chave no arquivo de dados e os ponteiros são\n");
	printf("NRR's do arquivo de indíces\n\n");
	ImprimirArquivo(fp);

	int escolha;
	do{
		printf("\n#######################################################\n");
		printf("Que operação você deseja fazer agora?\n");
		printf("1. Adicionar um novo registro\n");
		printf("2. Remover um registro existente\n");
		printf("3. Fazer a busca de um novo registro\n");
		printf("0. Sair\n");

		do{
			printf("Escolha uma opção válida:\n");
			scanf("%d", &escolha);
		} while(escolha < 0 || escolha > 3);

		switch(escolha)
			{
			case 1:
				insere_novo_registro(arv,nome_arq);

				//Voltando para o começo do arquivo
				fseek(fp, 0, SEEK_SET);	
				salvar_arvore(arv, arv->raiz, fp);
				ImprimirArquivo(fp);
				break;
			case 2:
				printf("Qual a primeira letra do registro que você deseja excluir?\n");
				break;
			case 3:
				printf("Qual a chave do registro que você deseja pesquisar?\n");
				break;
			}
	} while(escolha != 0);

	/* Liberando todo o espaço alocado em memória */
	libera_arvore(arv, arv->raiz);
	free(arv);
	fclose(fp);
	return 0;
}
