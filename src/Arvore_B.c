#include <stdio.h>
#include <stdlib.h>
#include "../include/Arvore_B.h"
#include <string.h>



ArvB* Criar_ArvB(int ordem){
	ArvB *Arvore;

	Arvore = (ArvB*)malloc(sizeof(ArvB));
	Arvore->raiz = NULL;
	Arvore->ordem = ordem;
	return Arvore;
}

int Salvar_No(No *no, int ordem, FILE *arquivo){
	int i, NRR, tamanho, TAM_PAG;

	if(no == NULL){
		return -01;
	}
	/* Chama a função recursivamente para os filhos */
	if(no->n_filhos != 0){
		for(i=0; i<no->n_filhos; i++){
			no->filhos_NRR[i] = Salvar_No(no->filho[i], ordem, arquivo);
		}
	}	
	TAM_PAG = (ordem - 1)*(TAM_CHAVE + TAM_NRR) + ordem*TAM_NRR;
	tamanho = ftell(arquivo);
	NRR = (tamanho - 3) / TAM_PAG;
	/* imprime as chaves e os NRR dos registros correspondentes */
	for(i=0; i<ordem-1; i++){
		if(i<no->n_ind){
			fprintf(arquivo, "%s %03d ", no->indice[i].chave, no->indice[i].reg_NRR);
		}
		else{
			fprintf(arquivo, "############ ");
		}
	}
	/* imprime os NRRs dos filhos */
	for(i=0; i<ordem; i++){
		fprintf(arquivo, "%03d ", no->filhos_NRR[i]);
	}
	fprintf(arquivo, "\n");
	return NRR;

}

void Salvar_ArvB(ArvB* Arvore, char* nome_arq_ind){
	FILE *arquivo;

	arquivo = fopen(nome_arq_ind, "w+");
	fprintf(arquivo, "%03d\n", Arvore->ordem);
	Salvar_No(Arvore->raiz, Arvore->ordem, arquivo);
	fclose(arquivo);

}
//RemoverItem()

void RemoverNoFolha(No* no, int idx){
	/* Move todos os índices após idx par tras*/
	for(int i=idx+1; i<no->n_ind; i++){
		strcpy(no->indice[i-1].chave, no->indice[i].chave);
		no->indice[i-1].reg_NRR = no->indice[i].reg_NRR;
	}
	no->n_ind--;

	return;
}

/*
//idx nesse caso é o índice do n
void RemoverNaoFolha(No* no, int ordem, int idx){
	int minimo;
	Ind pred, suc;

	minimo = (ordem/2 -1);
	if(no->filho[idx]->n_ind >= minimo){
		pred = PegarPred(no->filho[idx]);
		strcpy(no->indice[idx].chave, pred.chave);
		no->indice[idx].reg_NRR = pred.reg_NRR;
	///	RemoverItem(no->filho[idx-1], chave);
	}
	else if(no->filho[idx+1]->n_ind >= minimo){
		suc = PegarSuc(no->filho[idx+1]);
		strcpy(no->indice[idx+1].chave, suc.chave);
		no->indice[idx].reg_NRR = suc.reg_NRR;
	///	RemoverItem(no->filho[idx+1], chave);
	}
	else{
	///	Merge()
	///	RemoverItem(no, chave)
	}
	return;
} */

Ind PegarPred(No* no){
	No* aux;

	/* Percorre a árvore até chegar em um nó folha */
	aux = no;
	while(aux->n_filhos != 0){
		aux = aux->filho[aux->n_filhos-1];
	}

	/* retorna o último índice do nó folha */
	return aux->indice[aux->n_ind-1];
}

Ind PegarSuc(No* no){
	No* aux;

	/* Percorre a árvore até chegar em um nó folha */
	aux = no;
	while(aux->n_filhos != 0){
		aux = aux->filho[0];
	}

	/* retorna o primeiro índice do nó folha */
	return aux->indice[0];	
}
//PreencherNo()

void PedirEmprestadoAnt(No* pai, int idx){
	No *filho1, *filho2;

	filho1 = pai->filho[idx];
	filho2 = pai->filho[idx-1];

	/* Move todos os índices para frente*/
	for(int i=pai->n_ind-1; i>=0; i--){
		strcpy(pai->indice[i+1].chave, pai->indice[i].chave);
		pai->indice[i+1].reg_NRR = pai->indice[i].reg_NRR;
	}

	/* Se não for folha, move para frente todos os ponteiros 
	para os filhos */
	if(pai->n_filhos != 0){
		for(int i=pai->n_filhos-1; i>0; i--){
			pai->filho[i+1] = pai->filho[i];
			pai->filhos_NRR[i+1] = pai->filhos_NRR[i];
		}
	}

	/* Move para o filho1 o índice[idx-1] do no pai */
	strcpy(filho1->indice[0].chave, pai->indice[idx-1].chave);
	filho1->indice[0].reg_NRR = pai->indice[idx-1].reg_NRR;

	/* Move o último filho do filho2 para o primeiro do filho1 */
	if(filho1->n_filhos != 0){
		filho1->filho[0] = filho2->filho[filho2->n_filhos-1];
		filho1->filhos_NRR[0] = filho2->filhos_NRR[filho2->n_filhos-1];	
	}

	/* Move o índicee do filho2 para o pai */
	strcpy(pai->indice[idx-1].chave, filho2->indice[filho2->n_filhos-2].chave);
	pai->indice[idx-1].reg_NRR = filho2->indice[filho2->n_filhos-2].reg_NRR;

	filho1->n_filhos += 1;
	filho2->n_filhos -= 1;

	return;
}

void PedirEmprestadoProx(No* pai, int idx){
	No *filho1, *filho2;

	filho1 = pai->filho[idx];
	filho2 = pai->filho[idx-1];

	strcpy(filho1->indice[filho1->n_ind-1].chave, pai->indice[idx].chave);
	filho1->indice[filho1->n_ind-1].reg_NRR = pai->indice[idx].reg_NRR;

	/* Insere o ultimo filho de filho2 no primeiro de filho1*/
	if(filho1->n_filhos == 0){
		filho1->filho[filho1->n_filhos] = filho2->filho[0];
	}

	strcpy(pai->indice[idx].chave, filho2->indice[0].chave);
	pai->indice[idx].reg_NRR = filho2->indice[0].reg_NRR;

	/* Move todos os índices do filho2 para tras */
	for(int i=1; i<filho2->n_filhos; i++){
		strcpy(filho2->indice[i-1].chave, filho2->indice[i].chave);
		filho2->indice[i-1].reg_NRR = filho2->indice[i].reg_NRR;
	}

	/* Move todos os ponteiros para filhos de filho2 para tras */
	if(filho2->n_filhos != 0){
		for(int i=1; i<filho2->n_filhos; i++){
			filho2->filho[i-1] = filho2->filho[i];
			filho2->filhos_NRR[i-1] = filho2->filhos_NRR[i];
		}
	}

	filho1->n_filhos += 1;
	filho2->n_filhos -=1;

	return;

}

//Merge()

/* Função que retorna o o indice da primeira chave
maior ou igual a chave buscada */
int EncontrarChave(No* no, char* chave){
	Ind aux;
	int idx = 0;

	aux = no->indice[idx];
	while(strcmp(aux.chave, chave) < 0){
		idx++;
	}
	return idx;
}








