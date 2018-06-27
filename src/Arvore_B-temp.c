#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Arvore_B.h"
#include "Arvore_B-temp.h"

No* AlocarNo(int ordem){
	/* Alocar no e vetores de indices e filhos. */
	No* no = (No*) malloc(sizeof(No));
	no->indice = (Ind*) malloc((ordem-1) * sizeof(Ind));
	no->filho = (No**) malloc(ordem * sizeof(No*));
	no->filhos_NRR = (int*) malloc(ordem * sizeof(int));
	/* Inicializar quantidade de filhos e indices como 0. */
	no->n_ind = 0;
	no->n_filhos = 0;

	return no;
}

void DividirFilho(No* pai, No* no_1, int i, int ordem){
	/* Achar grau mínimo e se a ordem é par. */
	int t = ordem/2;
	/* Criar novo nó que compartilhará chaves de no_1. */
	No* no_2 = AlocarNo(ordem);
	no_2->n_ind = t-1;

	int j;

	/* Copiar as ultimas (ordem/2 - 1) chaves de no_1 para no_2. */
	for(j=0; j < t-1; j++){
		strcpy(no_2->indice[j].chave, no_1->indice[j+t].chave);
		no_2->indice[j].reg_NRR = no_1->indice[j+t].reg_NRR;
	}

	/* Copiar os ultimos t filhos de no_1 para no_2 . */
	if(no_1->n_filhos != 0){
		for(j=0; j<t; j++){
			no_2->filho[j] = no_1->filho[j+t];
		}
		no_2->n_filhos = t;
		no_1->n_filhos = no_1->n_filhos - t;
	}

	/* Reduzir o número de chaves em no_1. */
	no_1->n_ind = t-1;

	/* Criar espaço para o novo filho. */
	for(j = pai->n_ind; j >= i+1; j--){
		pai->filho[j+1] = pai->filho[j];
	}

	/* Ligar novo filho a esse nó. */
	pai->filho[i+1] = no_2;
	pai->n_filhos++;

	/* Mover chave central de no_1 para o pai. */
	for(j = pai->n_ind-1; j >= i; j--){
		strcpy(pai->indice[j+1].chave, pai->indice[j].chave);
		pai->indice[j+1].reg_NRR = pai->indice[j].reg_NRR;
	}

	/* Copiar chave central de no_1. */
	strcpy(pai->indice[i].chave, no_1->indice[t-1].chave);
	pai->indice[i].reg_NRR = no_1->indice[t-1].reg_NRR;
	pai->n_ind++;
}	

void InserirNaoCheio(No* no, char chave[], int reg_NRR, int ordem){
	/* Iniciar indice como o indice do ultimo filho colocado. */
	int i = no->n_ind - 1;

	/* Verificar se é folha. */
	if(no->n_filhos == 0){
		/* Procurar posição a ser inserido o novo nó. */
		while(i >=0 && strcmp(no->indice[i].chave, chave) > 0){
			strcpy(no->indice[i+1].chave, no->indice[i].chave);
			no->indice[i+1].reg_NRR = no->indice[i].reg_NRR;
			i--;
		}

		/* Inserir nó na posição encontrada. */
		strcpy(no->indice[i+1].chave, chave);
		no->indice[i+1].reg_NRR = reg_NRR;
		no->n_ind++;
	}
	/* Se não for folha. */
	else{
		/* Achar filho que vai ter um novo nó. */
		while(i >=0 && strcmp(no->indice[i].chave, chave) > 0){
			i--;
		}
		/* Ver se o filho que vai seceber o novo nó está cheio. */
		if(no->filho[i+1]->n_ind == ordem-1){
			DividirFilho(no, no->filho[i+1], i+1, ordem);

			/* Ver onde será inserifa a nova chave. */
			if(strcmp(no->indice[i+1].chave, chave)<0)
				i++;
		}
		InserirNaoCheio(no->filho[i+1], chave, reg_NRR, ordem);
	}
}

void InserirItem(ArvB* arv, char chave[], int reg_NRR){
	/* Verifica se a raiz já está alocada. */
	if(arv->raiz == NULL){
		/* Alocar raiz. */
		arv->raiz = AlocarNo(arv->ordem);
		/* Inserir chave. */
		strcpy(arv->raiz->indice[0].chave, chave);
		arv->raiz->indice[0].reg_NRR = reg_NRR;
		arv->raiz->n_ind++;
	}
	/* Caso já tenha raiz. */
	else{
		/* Se a raiz estiver cheia. */
		if(arv->raiz->n_ind == arv->ordem-1){
			/* Alocar nova raiz. */
			No* nova_raiz = AlocarNo(arv->ordem);
			/* Fazer nova raiz apontar para a velha. */
			nova_raiz->filho[0] = arv->raiz;
			nova_raiz->n_filhos++;

			/* Dividir velha raiz. */
			DividirFilho(nova_raiz, arv->raiz, 0, arv->ordem);

			/* Adicionar nova chave. */
			int j = 0;
			if(strcmp(nova_raiz->indice[0].chave, chave) < 0)
				j++;
			InserirNaoCheio(nova_raiz->filho[j], chave, reg_NRR, arv->ordem);

			/* Mudar raiz. */
			arv->raiz = nova_raiz;
		}
		/* Se a raiz nao estiver cheia. */
		else
			InserirNaoCheio(arv->raiz, chave, reg_NRR, arv->ordem);
	}
}

int ImprimirNo(No* no, int idx_pai, int idx_atual){
	int i;
	/* Imprimir nó. */
	printf("%02d - Nó filho de %02d: [ ", idx_atual, idx_pai);
	for(i=0; i < no->n_ind; i++){
		printf("%s ", no->indice[i].chave);
	}
	printf("]\n");

	/* Imprimir os filhos. */
	int idx_novo = idx_atual + 1;
	for(i=0; i < no->n_filhos; i++){
		idx_novo = ImprimirNo(no->filho[i], idx_atual, idx_novo);
	}

	return idx_novo;
}

void ImprimirArvore(ArvB *arv){
	int idx_raiz = 0; /* Representa o número do registro no monitor. */
	int i;

	/* Imprimir raiz. */
	No* raiz = arv->raiz;
	printf("%02d - Raiz: [ ", idx_raiz);
	for(i=0; i < raiz->n_ind; i++){
		printf("%s ", raiz->indice[i].chave);
	}
	printf("]\n");

	/* Chamar função para imprimir os filhos. */
	int idx_atual = 1;
	for(i=0; i < raiz->n_filhos; i++){
		idx_atual = ImprimirNo(raiz->filho[i], idx_raiz, idx_atual);
	}
}