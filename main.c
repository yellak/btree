#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "btree.h"
#include "in-out.h"

/* Função que vai indicar o início do programa */
void inicio(void)
{
	printf("#################################################\n");
	printf("#     Trabalho 2 - Organização de arquivos      #\n");
	printf("#################################################\n");
}

void linha(void){
	printf("\n-----------------------------------------------------\n");
}

/* Aqui, vamos indicar como está estruturado o arquivo de índices */
void estrutura_ind(void){
	printf("\n-------------------------------------------------------------------------\n");
	printf("chave1 NRR1 chave2 NRR2 ... Ponteiros para os filhos no arquivo .bt");
	printf("\n-------------------------------------------------------------------------\n\n");
}

/* Função principal do programa, conterá toda a interface entre usuário e programa */
int main(){
	Barv* arv;
	char* nome_arq = "lista.txt"; /* Arquivo de dados */
	char* nome_indice = "indicelista.bt"; /* Arquivo de íncices */
	FILE* fp = fopen(nome_indice, "w+");

	/* Variáveis usadas para o caso de pesquisa ou de exclusão de registro */
	int n_seeks, posicao;
	char chave[9];

	/* Variáveis usadas para o caso de pesquisa de um registro */
	FILE* dados;
	Registro* reg = aloca_registro();

	/* Variável usada caso seja escolhida opção de remover */
	int resultado;

	inicio();

	/* Adquirindo o valor da ordem */
	int ordem;
	printf("\nInsira a ordem desejada para a árvore:\n");
	scanf("%d", &ordem);
	while(ordem < 3){ /* Ordem 2 ou menor não produzirá bons resultados */
		printf("Insira uma ordem maior que 2:\n");
		scanf("%d", &ordem);
	}

	/* Arqui alocamos uma árvore vazia */
	arv = criar_arvore(ordem);

	/* Lendo o arquivo, criando a árvore e salvando ela */
	ler_arquivo_cria_arvore(arv, nome_arq);
	salvar_arvore(arv, arv->raiz, fp);

	/* Imprimindo o arquivo resultante na tela */
	printf("\nArquivo com a árvore-B criada:\n\n");

	/* Indicando a estrutura da árvore vazia */
	printf("Estrutura:\n");
	printf("Cada linha é um nó da árvore e a raíz está na última linha com dados\n");

	estrutura_ind();
	
	printf("Observações:\n");
	printf("NRR é a posição da chave no arquivo de dados e os ponteiros são\n");
	printf("NRR's do arquivo de indíces\n\n");

	/* Imprimindo o arquivo de índices */
	ImprimirArquivo(fp);

	int escolha;
	do{
		printf("\n##################################################################\n\n");
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
				linha();
				insere_novo_registro(arv, nome_arq);

				//Voltando para o começo do arquivo
				fseek(fp, 0, SEEK_SET);	
				salvar_arvore(arv, arv->raiz, fp);

				printf("\nArquivo de índices após a adição:\n");
				/* Indicando a estrutura do arquivo de dados */
				estrutura_ind();
				ImprimirArquivo(fp);
				printf("\n");

				/* Limpando o buffer para esperar por uma tecla do usuário */
				setbuf(stdin, NULL);
				getchar();
				break;
			case 2:
				linha();
				printf("Qual a chave do registro que você deseja excluir?\n");
				do{
					scanf("%s", chave);
				} while(strlen(chave) > 8); /* A chave tem no máximo 8 caracteres não nulos */

				/* Removendo o registro no arquivo de dados, o registro removido será marcado
				   com um '*' no primeiro byte de sua respectiva linha */
				resultado = remove_registo(nome_arq, chave, ordem, arv, fp);

				if(resultado != -1){
					libera_arvore(arv, arv->raiz);
					free(arv);
					arv = criar_arvore(ordem);
					ler_arquivo_cria_arvore(arv, nome_arq);

					fclose(fp);
					remove(nome_indice);
					fp = fopen(nome_indice, "w+");
					salvar_arvore(arv, arv->raiz, fp);

					printf("\nArquivo de índices após a remoção:\n");
					estrutura_ind();
					ImprimirArquivo(fp);
					printf("\n");
				}

				setbuf(stdin, NULL);
				getchar();
				break;
			case 3:
				linha();
				printf("Qual a chave do registro que você deseja pesquisar?\n");
				do{
					scanf("%s", chave);
				} while(strlen(chave) > 8);

				/* Adiquirindo a posição do registro pesquisado no arquivo de dados */
				posicao = busca_chave(fp, chave, ordem, &n_seeks);

				printf("\n");
				if(posicao != -1){ /* O registro existe */
					dados = fopen(nome_arq, "r");
					/* Fazendo a leitura na posição encontrada */
					ler_registro_dados(dados, reg, posicao);

					/* Imprimindo dados obtidos */
					printf("Número de seeks para fazer a pesquisa: %d\n", n_seeks);
					printf("\nOs dados do registro procurado são:\n");
					printf("Nome: %s\n", reg->nome);
					printf("Matrícula: %s\n", reg->matricula);
					printf("Curso: %s\n", reg->curso);
					printf("Turma: %s\n", reg->turma);

					fclose(dados);
				}
				else{
					printf("Registro não encontrado, verifique se digitou a chave corretamente\n");
				}

				/* Limpando o buffer para esperar pelo usuário apertar uma tecla */
				setbuf(stdin, NULL);
				getchar();
				break;
			}
	} while(escolha != 0);

	/* Liberando todo o espaço alocado em memória */
	libera_arvore(arv, arv->raiz);
	free(arv);
	libera_registro(reg);
	fclose(fp);
	return 0;
}
