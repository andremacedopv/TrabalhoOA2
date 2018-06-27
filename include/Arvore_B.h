#ifndef ARVORE_B_
#define ARVORE_B_

#include <stdio.h>
#define TAM_REG 54
#define TAM_CHAVE 9

typedef struct ind{
	char chave[TAM_CHAVE];
	int reg_NRR;
}Ind;

typedef struct no{
	int n_ind;
	int n_filhos;
	Ind *indice;
	int *filhos_NRR;
	struct no **filho;
}No;

typedef struct arvb{
	No *raiz;
	int ordem;
}ArvB;



/* Cria uma Árvore B vazia */
ArvB* Criar_ArvB(int ordem);

/* Busca por uma chave em um no e retorna o índice correspondente */
int Busca(No* no, int ordem, char *chave, char *nome_arq);

/* Salva uma árvore B em um arquivo */
int Salvar_ArvB(No *no, int ordem, FILE *arquivo);

#endif