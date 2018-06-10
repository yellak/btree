#include <stdio.h>
#include <stdlib.h>
#include "btree.h"

int main(){
  int ordem = 2;
  Barv* arv = criar_arvore(ordem);

  char c[] = "dbcdefgh";
  char b[] = "cbcdefgh";
  char a[] = "abcdefgh";
  char d[] = "bbcdefgh";

  inserir_btree(arv, c, 0);
  inserir_btree(arv, b, 1);
  inserir_btree(arv, a, 2);
  inserir_btree(arv, d, 3);

  libera_arvore(arv, arv->raiz);
  free(arv);
  
  return 0;
}
