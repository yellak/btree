#include <stdio.h>
#include "btree.h"
#include "in-out.h"

int main(){
  int ordem = 2;
  Barv* arv = criar_arvore(ordem);
  char* nome_arq = "lista.txt";

  le_arquivo_cria_arvore(arv, nome_arq);
  /*char c[] = "dbcdefgh";
  char b[] = "cbcdefgh";
  char a[] = "abcdefgh";
  char d[] = "bbcdefgh";*/

  /*inserir_btree(arv, c, 0);
  inserir_btree(arv, b, 1);
  inserir_btree(arv, a, 2);
  inserir_btree(arv, d, 3);*/
  return 0;
}
