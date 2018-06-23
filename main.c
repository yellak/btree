#include <stdio.h>
#include <stdlib.h>	
#include "btree.h"
#include "in-out.h"

int main(){
  int ordem = 2;
  Barv* arv = criar_arvore(ordem);
  char* nome_arq = "lista.txt";

  le_arquivo_cria_arvore(arv, nome_arq);
  libera_arvore(arv, arv->raiz);
  free(arv);
  return 0;
}
