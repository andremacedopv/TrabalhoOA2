#include <stdio.h>
#include <stdlib.h>
#include "Arvore_B.h"
#include <string.h>



ArvB* Cria_ArvB(int ordem){
	ArvB *Arvore;

	Arvore = (ArvB*)malloc(sizeof(ArvB));
	Arvore->raiz = NULL;
	Arvore->ordem = ordem;
	return Arvore;
}

Ind Busca(No* no, char *chave){
	int i = 0;

	while(i < no->n_ind && strcmp(chave, no->indice[i].chave) > 0){
		i++;
	}

	if(strcmp(chave, no->indice[i].chave) == 0){
		return no->indice[i];
	}

	if(no->n_filhos == 0){
		return NULL;
	}

	return Busca(no->filho[i], chave);
}




