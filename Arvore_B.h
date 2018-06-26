

typedef struct ind{
	char *chave;
	int reg_NRR;
}Ind;

typedef struct no{
	int pag_NRR;
	int n_ind;
	int n_filhos;
	Ind *indice;
	struct no **filho;
}No;

typedef struct arvb{
	No *raiz;
	int ordem;
}ArvB;



/* Cria uma Árvore B vazia */
ArvB* Cria_ArvB(int ordem);

/* Busca por uma chave em um no e retorna o índice correspondente */
Ind Busca(No* no, char *chave);

