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
	int NRR_filhos[2*arv->ordem];

	while((raiz->filhos[i] != NULL) && (i < raiz->n_nos + 1)){
		NRR_filhos[i] = salvar_arvore(arv, raiz->filhos[i], fp);
		i++;
	}
  
	for( ; i < 2*arv->ordem; i++){
		NRR_filhos[i] = -1;
	}

	minha_posicao = ftell(fp)/(14*(2*arv->ordem - 1) + 5*arv->ordem);


	for(i = 0; i < 2*arv->ordem - 1; i++){
		if(i < raiz->n_nos){
			fprintf(fp, "%s %4d ", raiz->chaves[i], raiz->NRR[i]);
		}
		else{
			fprintf(fp, "************* ");
		}
	}

	for(i = 0; i < 2*arv->ordem; i++){
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

	printf("Digite o nome do novo registro:\n");
	scanf("\n%[^\n]",nome);

	printf("Digite a matrícula do novo registro:\n");
	scanf("%s",matricula);	

	printf("Digite o curso do novo registro:\n");
	scanf("%s",curso);

	printf("Digite a turma do novo registro:\n");
	scanf("%s",turma);

	FILE* fp = fopen(nome_arq,"a+");

	nome = PreencheComEspaco(nome, 41);
	matricula = PreencheComEspaco(matricula, 7);
	curso = PreencheComEspaco(curso, 4);
	turma = PreencheComEspaco(turma, 2);

	fseek(fp, 0, SEEK_END);

	NRR = ftell(fp)/(14*(2*arv->ordem - 1) + 5*arv->ordem) - 1;
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
	free(chave);

	fclose(fp);
}