#include "btree.h"
#include "in-out.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//const int T_CHAVE = 9;

char *strupr(char *str){
	unsigned char *p = (unsigned char *)str;

	while (*p) {
		*p = toupper(*p);
		p++;
	}

	return str;
}

NoB* ler_registro_ind(FILE* fp, int NRR[], int ordem){
	int TAM_REG = 14*(ordem - 1) + 5*ordem;
	int i, j;
	char chave_aux[14];

	NoB* no = aloca_no(ordem);

	for(i = 0; i < ordem - 1; i++){
		fscanf(fp, "%s", chave_aux);
		if(chave_aux[0] != '*'){
			for(j = 0; j < 9; j++){
				no->chaves[i][j] = chave_aux[j];
			}
			no->chaves[i][8] = '\0';
			fscanf(fp, "%d ", &no->NRR[i]);
		}
		else{
			strcpy(no->chaves[i], "********");
			no->NRR[i] = -1;
		}
	}

	for(i = 0; i < ordem; i++){
		fscanf(fp, "%d", &NRR[i]);
	}

	return no;
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

	minha_posicao = ftell(fp)/(14*(arv->ordem - 1) + 5*arv->ordem);


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

void ImprimirArquivo(FILE* fp){
	int c;
	rewind(fp);
	while( (c = getc(fp)) != EOF){
		putchar(c);
	}
}

//Função responsavel por receber uma string e preencher o resto do seu espaço com espaço(" ") para facilitar a criação de chaves
char* PreencheComEspaco(char* string, int tamanho){
	size_t prevlen = strlen(string);
	memset(string + prevlen,' ',tamanho - 1 - prevlen);
	string[tamanho-1] = '\0';
	return string;
}

void insere_novo_registro(Barv* arv, char* nome_arq){
	char* nome = (char*)malloc(41*sizeof(char));
	char* matricula = (char*)malloc(7*sizeof(char));
	char* curso = (char*)malloc(4*sizeof(char));
	char* turma = (char*)malloc(2*sizeof(char));
	char* chave = (char*)malloc(9*sizeof(char));
	int NRR;

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

	nome = PreencheComEspaco(nome, 41);
	matricula = PreencheComEspaco(matricula, 7);
	curso = PreencheComEspaco(curso, 4);
	turma = PreencheComEspaco(turma, 2);

	fseek(fp, 0, SEEK_END);

	NRR = ftell(fp)/(14*(arv->ordem - 1) + 5*arv->ordem) - 1;
	fprintf(fp, "%s %s %s %s\n",nome, matricula, curso,turma);

	for(int i = 0; i < 3; i++){
		chave[i] = nome[i];
	}

	chave = strupr(chave);

	for(int i = 0; i < 5; i++){
		chave[i+3] = matricula[i];
	}

	chave[8] = '\0';

	inserir_btree(arv, chave, NRR);

	free(chave);
	free(nome);
	free(matricula);
	free(curso);
	free(turma);

	fclose(fp);
}

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
				/* Se for maior que zero a primeira chave é depois da ordem alfabetica */
				if(strcmp(no->chaves[i], chave) == 0){
					return no->NRR[i];
				}
				else if(strcmp(chave, no->chaves[i]) < 0){

					if(NRR[i] != -1){
						fseek(fp, NRR[i]*TAM_REG, SEEK_SET);
						*n_seeks += 1;
						break;
					}

					else{
						return -1;
					}
				}
				else{
					i++;
					if(i == ordem - 1){
						continua = 0;
					}
				}
			}

			for(j = 0; j < ordem - 1; j++){
				free(no->chaves[j]);
			}

			free(no->chaves);
			free(no->NRR);
			free(no->filhos);
			free(no);
		} /* while(!achou) */
	
	return -1;
}

void remove_registo(char* nome_arq, char* chave, int ordem, Barv* arv, FILE* ind){
	FILE* fp = fopen(nome_arq, "r");

	int NRR[ordem];
	int nseeks;
	int pos = busca_chave(ind, chave, ordem, &nseeks);

	fseek(fp, ((14*(arv->ordem - 1) + 5*arv->ordem) - 1)*pos, SEEK_SET);

	fputc('*', fp);

	libera_arvore(arv, arv->raiz);
	fclose(fp);

	arv = criar_arvore(ordem);
	ler_arquivo_cria_arvore(arv, nome_arq);

}
