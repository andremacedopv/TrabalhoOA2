# Trabalho 2 - Organização de Arquivos
#### Implementação de um B-Tree e buscas em arquivos

## Integrantes
	André Macedo Prado Valle - 17/0005950
	Danilo Inácio dos Santos Silva - 17/0031861


## Observações

	O programa foi projetado utilizando o algoritmo proposto por cormen, que trabalha com o
	grau mínimo da árvore ao invés da ordem dela. Por conta disso, nosso programa funciona
	melhor para ordens pares, com funcionamento esperado não garantido para ordens impares.
	Além disso, é exigido que a ordem da árvore, passada pelo usuário, seja maior ou igual a
	quatro.

	O programa foi projetado, também, para funcionar com arquivos de nome "lista.txt". Renomeie
	os possíveis arquivos de dados para esse nome para o correto funcionamento.

	Os arquvios de índice gerados terão sempre o nome "indicelista.bt".
	
## Compilação

	Para compilar o programa, vá até a pasta raiz do projeto, onde se encontra o arquivo makefile,
	abra o terminal e execute:
	>> make

## Execução

	Para executar o programa, abra o terminal na pasta onde
	está o programa e execute o comando:
	>> ./main

## Arquivo produzidos

   	-> indicelista.bt
	
	Arquivo com os indíces e seus respectivos NRRs em formato de B-Tree.
	Na primeira linha se encontra a ordem (m) da árvore B e na última a raiz dessa árvore.
	Cada linha, com exceção da primeira, contém um nó da árvore, contendo primeiramente
	as (m-1) chaves do nó, juntos com os respectivos NRRs do arquivo de dados, seguidos, no
	fim, pelas NRRs dos filhos desse nó no arquivo de índices.


## Módulos

	-> Arvore_B.c

	Contém as funções de criação e manipulação da B-Tree.

	-> Resgistros.c

	Contém as funções de manipulação da lista de dados.

	-> main.c

	Contém os procedimentos principais do software.


## Repositório

	O repositório oficial deste programa pode ser acessado
	pelo link abaixo:
	https://github.com/andremacedopv/TrabalhoOA2
