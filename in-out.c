#include "btree.h"
#include "in-out.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const int TAM_REG_DADOS = 54;

/* Função que converte as letras minúsculas de uma string em maiúsculas */
char *strupr(char *str){
	unsigned char *p = (unsigned char *)str;

	while (*p) {
		*p = toupper(*p);
		p++;
	}

	return str;
}

/* Função que aloca um registro lido do arquivo*/
Registro* aloca_registro(void){
	Registro* novo = (Registro*) malloc(sizeof(Registro));
	novo->nome = (char*)malloc(41*sizeof(char));
	novo->matricula = (char*)malloc(7*sizeof(char));
	novo->curso = (char*)malloc(4*sizeof(char));
	novo->turma = (char*)malloc(2*sizeof(char));

	return novo;
}

/* Função que libera um registro */
void libera_registro(Registro* reg){
	free(reg->nome);
	free(reg->matricula);
	free(reg->curso);
	free(reg->turma);
	free(reg);
}

/* Função que lê um registro do arquivo de dados com base no NRR mandado */
void ler_registro_dados(FILE* fp, Registro* reg, int NRR){
	char registro[TAM_REG_DADOS];
	int i;

	/* Busca-se a posição do registro */
	fseek(fp, TAM_REG_DADOS*NRR, SEEK_SET);
	fgets(registro, TAM_REG_DADOS, fp);

	/* O nome são os 40 primeiros caracteres da linha */
	for(i = 0; i < 40; i++){
		reg->nome[i] = registro[i];
	}
	reg->nome[40] = '\0';

	/* A matricula está da posição 41 até 41 + 6 */
	for(i = 0; i < 6; i++){
		reg->matricula[i] = registro[i + 41];
	}
	reg->matricula[6] = '\0';

	/* O curso está na posição 48 até 48 + 4 */
	for(i = 0; i < 4; i++){
		reg->curso[i] = registro[i + 48];
	}
	reg->curso[3] = '\0';

	/* A turma é o penúltimo caracter da linha */
	reg->turma[0] = registro[52];
	reg->turma[1] = '\0';
}

/* Função que ler um único registro no arquivo de índices */
NoB* ler_registro_ind(FILE* fp, int NRR[], int ordem){
	/* Tamanho do registro no arquivo de dados */
	int TAM_REG = 14*(ordem - 1) + 5*ordem;
	int i, j;
	char chave_aux[14];	/* solução para quando a chave não existe */

	NoB* no = aloca_no(ordem);

	for(i = 0; i < ordem - 1; i++){
		/* Lemos o espaço de uma chave ou de uma sequência de asteriscos */
		fscanf(fp, "%s", chave_aux);
		if(chave_aux[0] != '*'){
			/* Se a chave existe, então ela vai está da posição 0 até a posição
			   8 do vetor auxiliar*/
			for(j = 0; j < 9; j++){
				no->chaves[i][j] = chave_aux[j];
			}
			no->chaves[i][8] = '\0';
			/* Lemos o NRR da chave adquirida */
			fscanf(fp, "%d ", &no->NRR[i]);
		}
		else{
			/* Se a chave não existe só copiamos os asteriscos para indicar que
			   que ela não existe */
			strcpy(no->chaves[i], "********");
			no->NRR[i] = -1;
		}
	}

	/* Lemos os filhos do nó */
	for(i = 0; i < ordem; i++){
		fscanf(fp, "%d", &NRR[i]);
	}

	return no;
}

/* Esta função irá ler o aquivo de dados e criar a árvore-B
   a partir dele */
void ler_arquivo_cria_arvore(Barv* arv, char* nome_arq){

	FILE* fp = fopen(nome_arq, "r");
	int NRR = 0;
	int j;
	char str[55]; /* Uma linha no arquivo de dados */
	char* chave = (char*) malloc(9*sizeof(char)); /* Só precisamos da chave aqui */
	char ch;

	while(!feof(fp)){
		fseek(fp, 54*NRR, SEEK_SET);
		fgets(str, 54, fp);

		/* Verificando se o registro foi apagado */
		if(str[0] != '*'){
			/* Pegando as três primeiras letras do nome da pessoa */
			for(j = 0; j < 3; j++){
				chave[j] = str[j];
			}

			/* Fazendo as letras da chave maiúsculas */
			chave = strupr(chave);

			/* Copiando a matrícula do aluno */
			for(j = 0; j < 5; j++){
				chave[j + 3] = str[41 + j];
			}

			chave[8] = '\0';

			/* Inserindo o registro na árvore */
			inserir_btree(arv, chave, NRR);
		}
		
		ch = fgetc(fp);
		ch = fgetc(fp);

		/* Passamos para a póxima linha */
		NRR++;
	}
	fclose(fp);
	free(chave);
}

/* Função que salva a árvore B no arquivo de índices */
int salvar_arvore(Barv* arv, NoB* raiz, FILE* fp){
	NoB* aux;
	int i = 0;
	int minha_posicao;
	int NRR_filhos[arv->ordem];

	/* Função de salvar árvore é chamada recursivamente para cada filho */
	while((raiz->filhos[i] != NULL) && (i < raiz->n_nos + 1)){
		NRR_filhos[i] = salvar_arvore(arv, raiz->filhos[i], fp);
		i++;
	}
  
	/* Os espaços de filhos que não existem são preenchidos com -1 */
	for( ; i < arv->ordem; i++){
		NRR_filhos[i] = -1;
	}

	/* Armazena-se a linha da página no arquivo dividindo a posição atual do ponteiro pelo tamanho de uma linha */
	minha_posicao = ftell(fp)/(14*(arv->ordem - 1) + 5*arv->ordem);


	for(i = 0; i < arv->ordem - 1; i++){

		/* Enquanto houverem chaves no nó, imprime-se a sua chave e NRR no arquivo */
		if(i < raiz->n_nos){
			fprintf(fp, "%s %4d ", raiz->chaves[i], raiz->NRR[i]);
		}
		/* Imprime-se ************* quando não há chave no espaço */
		else{
			fprintf(fp, "************* ");
		}
	}

	/* Imprime-se o NRR da página no arquivo de índices */
	for(i = 0; i < arv->ordem; i++){
		fprintf(fp, "%4d ", NRR_filhos[i]);
	}


	fseek(fp, -1, SEEK_CUR);
	fprintf(fp, "\n");

	/* Retorna a linha da página */
	return minha_posicao;
}

/* Função que imprime o arquivo de índices na tela */
void ImprimirArquivo(FILE* fp){
	int c, i = 0;
	/* Indo para o início do arquivo */
	rewind(fp);
	/* Imprimindo a linha 0 */
	printf("%2d. ", i);
	while( (c = getc(fp)) != EOF){
		putchar(c);
		if(c == '\n'){
			i++;
			/* Imprimindo a linha atual */
			printf("%2d. ", i);
		}
	}
}

/* Função responsavel por receber uma string e preencher o resto do seu
   espaço com espaço(" ") para facilitar a criação de chaves */
char* PreencheComEspaco(char* string, int tamanho){
	size_t prevlen = strlen(string);
	memset(string + prevlen,' ',tamanho - 1 - prevlen);
	string[tamanho-1] = '\0';
	return string;
}

/* Função que insere um novo registro na árvore */
void insere_novo_registro(Barv* arv, char* nome_arq){
	char* nome = (char*)malloc(41*sizeof(char));
	char* matricula = (char*)malloc(7*sizeof(char));
	char* curso = (char*)malloc(4*sizeof(char));
	char* turma = (char*)malloc(2*sizeof(char));
	char* chave = (char*)malloc(9*sizeof(char));
	int NRR;

	/* Procedimento de leitura do registro */
	do{
	printf("Digite o nome do novo registro (Até 40 caracteres):\n");
	scanf("\n%[^\n]", nome);
	} while(strlen(nome) > 40);

	do{
	printf("Digite a matrícula do novo registro:\n");
	scanf("%s", matricula);
	} while(strlen(matricula) > 7);

	do{
	printf("Digite o curso do novo registro (2 caracteres):\n");
	scanf("%s",curso);
	} while(strlen(curso) > 2);

	do{
		printf("Digite a turma do novo registro (1 caracter):\n");
		scanf("%s", turma);
	} while(strlen(turma) > 1);

	FILE* fp = fopen(nome_arq,"a+");

	/* Preenche-se o resto da string com espaço */
	nome = PreencheComEspaco(nome, 41);
	matricula = PreencheComEspaco(matricula, 7);
	curso = PreencheComEspaco(curso, 4);
	turma = PreencheComEspaco(turma, 2);

	/* Vamos para o fim do arquivo de dados */
	fseek(fp, 0, SEEK_END);

	/* Registra-se o NRR do novo registro */
	NRR = ftell(fp)/(14*(arv->ordem - 1) + 5*arv->ordem) - 1;
	fprintf(fp, "%s %s %s %s\n", nome, matricula, curso, turma);

	/* Procedimento de extrair a chave */
	for(int i = 0; i < 3; i++){
		chave[i] = nome[i];
	}

	chave = strupr(chave);

	for(int i = 0; i < 5; i++){
		chave[i + 3] = matricula[i];
	}

	chave[8] = '\0';

	/* Insere-se a chave na árvore B */
	inserir_btree(arv, chave, NRR);

	free(chave);
	free(nome);
	free(matricula);
	free(curso);
	free(turma);

	fclose(fp);
}

/* Função responsável por buscar uma chave no arquivo de índices */
int busca_chave(FILE* fp, char* chave, int ordem, int* n_seeks){
	int TAM_REG = 14*(ordem - 1) + 5*ordem;
	int continua = 1, NRR[ordem], i, j;
	fseek(fp, - TAM_REG - 1, SEEK_END);

	*n_seeks = 1;

	while(continua)
		{
			i = 0;
			NoB* no = ler_registro_ind(fp, NRR, ordem);

			while(i < ordem -1){
				/* Se for 0 achamos a chave */
				if(strcmp(no->chaves[i], chave) == 0){
					return no->NRR[i];
				}
				/* Se for menor que zero chave vem antes da segunda na ordem alfabética */
				else if(strcmp(chave, no->chaves[i]) < 0){
					/* Caso ela tenha um filho damos um fseek até ele (filho à esquerda) */
					if(NRR[i] != -1){
						fseek(fp, NRR[i]*TAM_REG, SEEK_SET);
						*n_seeks += 1;
						break;
					}
					/* Se não, a chave não existe mais */
					else{
						return -1;
					}
				}
				/* Se for maior chave vem depois */
				else{
					/* Se a próxima chave não existe damos fseek no filho a direita da chave atual*/
					if(no->chaves[i + 1][0] == '*'){
						fseek(fp, NRR[i + 1]*TAM_REG, SEEK_SET);
						*n_seeks += 1;
						break;
					}
					i++;
					/* Caso percorramos todas as chaves do nó */
					if(i == ordem - 1){
						continua = 0;
					}
				}
			}

			/* Procedimentos para liberar os espaços alocados */
			for(j = 0; j < ordem - 1; j++){
				free(no->chaves[j]);
			}

			free(no->chaves);
			free(no->NRR);
			free(no->filhos);
			free(no);
		} /* while(!achou) */
	
	return -1; /* Se a chave não foi encontrada */
}

/* Função responsável por remover um registro do arquivo de dados para então recriar a árvore */
int remove_registo(char* nome_arq, char* chave, int ordem, Barv* arv, FILE* ind){
	FILE* fp = fopen(nome_arq, "r+");

	int NRR[ordem];
	int nseeks;
	/* Buscando a posição da chave a ser encontrada */
	int pos = busca_chave(ind, chave, ordem, &nseeks);

	/* Se ela não for encontrada avisamos ao usuário */
	if(pos == -1){
		printf("Chave não encontrada\n");
		return - 1;
	}
	/* Vamos para a posição encontrada */
	fseek(fp, 54*pos, SEEK_SET);

	fputc('*', fp);

	fclose(fp);

	return 1; /* Sucesso */
}
