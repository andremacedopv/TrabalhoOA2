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


int Salvar_ArvB(No *no, int ordem, FILE *arquivo){
	int i;

	static int n = -1;
	/* Chama a função recursivamente para os filhos */
	if(no->n_filhos != 0){
		for(i=0; i<no->n_filhos; i++){
			no->filhos_NRR[i] = Salvar_ArvB(no->filho[i], ordem, arquivo);
		}
	}	
	/* imprime as chaves e os NRR dos registros correspondentes */
	for(i=0; i<ordem-1; i++){
		if(i<no->n_ind){
			fprintf(arquivo, "%s %3d ", no->indice[i].chave, no->indice[i].reg_NRR);
		}
		else{
			fprintf(arquivo, "############ ");
		}
	}
	/* imprime os NRRs dos filhos */
	for(i=0; i<ordem; i++){
		fprintf(arquivo, "%3d ", no->filhos_NRR[i]);
	}
	fprintf(arquivo, "\n");
	n++;
	return n;
}

void Ler_No(int NRR, int ordem, char *nome_arq){ //incompleta
	FILE* arquivo;
	char chave[TAM_CHAVE];
	int reg_NRR, i, filhos_NRR[ordem], n_filhos;
	Ind indice[ordem-1];

	arquivo = fopen(nome_arq, "r+");
	fseek(arquivo, NRR, SEEK_SET);
	/* Le as chaves e os NRRs dos registros correspondentes */
	for (i=0; i<ordem - 1; i++){

		if(1){
			fgets(chave, TAM_CHAVE, arquivo);
			fscanf(arquivo, "%d", &reg_NRR);
			strcpy(indice[i].chave, chave);
			indice[i].reg_NRR = reg_NRR;
		}	
	}
//	/* Le os NRRs dos filhos */
	n_filhos = 0;
	for(i=0; i<ordem; i++){
		fscanf(arquivo, "%d", &filhos_NRR[i]);
		if(filhos_NRR[i] != -1){
			n_filhos ++;
		}
	}
	fclose(arquivo);
}


int Busca(No* no, int ordem, char *chave, char *nome_arq){ //incompleta
	int i = 0;
//	No* no_filho;

	while(i < no->n_ind && strcmp(chave, no->indice[i].chave) > 0){
		i++;
	}

	if(strcmp(chave, no->indice[i].chave) == 0){
		return no->indice[i].reg_NRR;
	}

	if(no->n_filhos == 0){
		return -1;
	}
	//Ler_No(no->filhos_NRR[i], ordem, nome_arq);

	//return Busca(no_filho, ordem, chave, nome_arq);
	return 1;
}




