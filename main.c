#include <stdio.h>
#include <stdlib.h>	
#include "btree.h"
#include "in-out.h"

int main(){
  /* Ordem é a quatidade mínima de filhos */
  int ordem = 2;
  Barv* arv = criar_arvore(ordem);
  char* nome_arq = "lista.txt";
  FILE* fp = fopen("indicelista.bt", "w");

  char c1[] = "aaaaaaaa";
  char c2[] = "bbbbbbbb";
  char c3[] = "cccccccc";
  char c4[] = "dddddddd";
  char c5[] = "eeeeeeee";

  inserir_btree(arv, c5, 1);
  inserir_btree(arv, c4, 2);
  inserir_btree(arv, c3, 3);
  inserir_btree(arv, c2, 4);
  inserir_btree(arv, c1, 5);
  
  //ler_arquivo_cria_arvore(arv, nome_arq);
  salvar_arvore(arv, arv->raiz, fp);
  libera_arvore(arv, arv->raiz);
  free(arv);
  fclose(fp);
  return 0;
}
