#ifndef __INOUT__H_
#define __INOUT__H_

#include <stdio.h>

/* Estrutura de dados para um registro do arquivo de dados */
typedef struct registro{
	char* nome;
	char* matricula;
	char* curso;
	char* turma;
} Registro;

/* Faz a string ficar maiúscula
   str - string com letras minúsculas */
char *strupr(char *str);

/* Cria a ávore a partir do arquivo de dados 
   arv      - árvore já alocada
   nome_arq - nome do arquivo de dados */
void ler_arquivo_cria_arvore(Barv* arv, char* nome_arq);

/* Salva a árvore-B no arquivo de índices
   arv  - árvore-B
   raiz - ponteiro para a raiz da árvore */
int salvar_arvore(Barv* arv, NoB* raiz, FILE* fp);

/* Preenche com espaços uma string incompleta
   string  - a própria
   tamanho - tamanho da string */
char* PreencheComEspaco(char* string, int tamanho);

/* Insere um novo registro no arquivo de dados
   arv      - a própria árvore
   nome_arq - nome do arquivo de dados */
void insere_novo_registro(Barv* arv, char* nome_arq);

/* Imprime um arquivo no terminal
   fp - arquivo a ser impresso */
void ImprimirArquivo(FILE* fp);

/* Ler um registro no arquivo de índices
   fp    - ponteiro para o arquivo de índices
   NRR   - vetorllll de posições dos filhos do nó lido
   ordem - ordem da árvore a ser lida */
NoB* ler_registro_ind(FILE* fp, int NRR[], int ordem);

/* Função que busca um novo registro na árvore guardada no arquivo de
   índices 
   fp      - ponteiro para o aquivo de índices 
   chave   - chave a ser procurada
   ordem   - ordem da árvore
   n_seeks - número de seeks durante o processo de procura */
int busca_chave(FILE* fp, char* chave, int ordem, int* n_seeks);

/* Função que remove um registro */
int remove_registo(char* nome_arq, char* chave, int ordem, Barv* arv, FILE* ind);

/* Ler um registro no arquivo de dados
   fp  - ponteiro para o arquivo de dados
   reg - ponteiro para um elemento registro
   NRR - posição a ser lida no arquivo */
void ler_registro_dados(FILE* fp, Registro* reg, int NRR);

/* Aloca um elemento de registro */
Registro* aloca_registro(void);

/* Libera um elemento de registro */
void libera_registro(Registro* reg);

#endif
