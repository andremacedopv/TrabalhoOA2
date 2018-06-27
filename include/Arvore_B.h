#ifndef ARVORE_B_
#define ARVORE_B_

#define TAM_REG
#define TAM_PAG
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
ArvB* Cria_ArvB(int ordem);

/* Busca por uma chave em um no e retorna o índice correspondente */
int Busca(No* no, char *chave, char *nome_arq);

#endif