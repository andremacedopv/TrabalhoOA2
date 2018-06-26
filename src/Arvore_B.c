#include <stdio.h>
#include <stdlib.h>
#include "../include/Arvore_B.h"
#include <string.h>



ArvB* Cria_ArvB(int ordem){
	ArvB *Arvore;

	Arvore = (ArvB*)malloc(sizeof(ArvB));
	Arvore->raiz = NULL;
	Arvore->ordem = ordem;
	return Arvore;
}

No* Le_No(int NRR, int ordem, char *nome_arq){
	FILE* arquivo;
	char chave[TAM_CHAVE];
	char c;
	int n_ind, n_filhos, reg_NRR, i;

	arquivo = fopen(nome_arq, "r+");
	fseek(arquivo, NRR*TAM_PAG, SEEK_SET);
	for (i=0; i<ordem - 1; i++){
			c = getc(arquivo);
		if(c != ' '){
			fgets(chave, TAM_CHAVE, arquivo);
			fscanf(arquivo, "%d", &reg_NRR);
		}	
	}
	for(i=0; i<ordem)

	//InsereNo();
}

int Busca(No* no, char *chave, char *nome_arq){
	int i = 0;
	No* no_filho;

	while(i < no->n_ind && strcmp(chave, no->indice[i].chave) > 0){
		i++;
	}

	if(strcmp(chave, no->indice[i].chave) == 0){
		return no->indice[i].reg_NRR;
	}

	if(no->n_filhos == 0){
		return -1;
	}
	no_filho = Le_No(no->filho[i]->pag_NRR, nome_arq);

	return Busca(no_filho, chave, nome_arq);
}




