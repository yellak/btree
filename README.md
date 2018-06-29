# Trabalho 2 - Organização de arquivos

#### Alunos:
	* Kálley Wilkerson - 170038050
	
	* Felipe Lima Vaz  - 170057852

## Observações

Para a implementação da árvore-B o algoritmo utilizado utilizava
o grau mínimo t para estabelecer alguns dados como a ordem da
árvore (2*t + 1), mas ele foi adaptado para que já recebesse a
ordem direta da árvore o que pode causar alguns incovenientes.

As bibliotecas utilizadas são todas bibliotecas padrões da
linguagem C padrão.

O programa foi criado em sistemas Unix.

O arquivo utilizado para a lista de alunos é o lista.txt e o
arquivo de índices é o arquivo indicelista.bt.

## Compilar o programa

Para isso basta está na pasta btree e digitar no terminal:

> make

E o programa será compilado

## Executar o programa

Para excutar o programa num sistema Unix basta ir no terminal e
digitar:

> ./main

## Módulos do programa

  * btree.c
  
  Contém todas as funções que manipulam a nível de memória a
  árvore-B implementada.
  
  * in-out.c
  
  Contém todas as funções que manipulam a nível de arquivo a
  árvore-B implementada.
  
  * main.c
  
  Implementação principal do programa, onde está implementada
  toda a interface com o usuário.
