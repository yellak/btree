#ifndef __INOUT__H_
#define __INOUT__H_

#include <stdio.h>

char *strupr(char *str);

void ler_arquivo_cria_arvore(Barv* arv, char* nome_arq);

int salvar_arvore(Barv* arv, NoB* raiz, FILE* fp);

char* PreencheComEspaco(char* string, int tamanho);

void insere_novo_registro(Barv* arv, char* nome_arq);

void ImprimirArquivo(FILE* fp);

NoB* ler_registro_ind(FILE* fp, int NRR[], int ordem);

int busca_chave(FILE* fp, char* chave, int ordem, int* n_seeks);

void remove_registo(char* nome_arq, char* chave, int ordem, Barv* arv, FILE* ind);

#endif
