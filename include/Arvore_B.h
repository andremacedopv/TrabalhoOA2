#ifndef ARVORE_B_
#define ARVORE_B_

#include <stdio.h>

/* Tamanho ocupado pelos campos nos registros incluindo um espaço vazio */
#define TAM_REG 54       
#define TAM_CHAVE 9
#define TAM_NRR 4

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

/* Salva recursivamente todos os nós a partir  do nó dado como parâmetro, em um arquivo */
int Salvar_No(No* no, int ordem, FILE *arquivo);

/* Salva uma árvore B em um arquivo */
void Salvar_ArvB(ArvB* Arvore, char* nome_arq_ind);

#endif