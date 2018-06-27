#include "../include/Registros.h"
#include <stdio.h>
#include <stdlib.h>


void Criar_chave(char* reg, char* chave){
	int i, j;
	/*copia as três primeiras letras do nome para chave */
	for(i=0; i<3; i++){
		/* Transforma a letra em maiúscula se ela for minúscula */
		if(reg[i]>='a' && reg[i]<='z'){
			reg[i] = reg[i] - 32;
		}
		chave[i] = reg[i];
	}
	/* copia a matrícula para chave */
	for(j=0; j<5; j++){
		chave[j+3] = reg[41+j];
	}
	chave[8]='\0';

}

void Criar_Indices(ArvB* Arvore, char* nome_arq, char* nome_arq_ind){
	FILE *arquivo;
	char reg[TAM_REG], chave[TAM_CHAVE];
	int NRR = 0;

	arquivo = fopen(nome_arq, "r+");
	while(!feof(arquivo)){
		char *temp = fgets(reg, 54, arquivo);
		if(temp == NULL)
			break;
		if(reg[0]!='#'){
			Criar_chave(reg, chave);
			InserirItem(Arvore, chave, NRR);
		}
		fgetc(arquivo);
		NRR++;
	}
	fclose(arquivo);
	arquivo = fopen(nome_arq_ind, "w+");
	Salvar_ArvB(Arvore->raiz, Arvore->ordem, arquivo);
	fclose(arquivo);

}

void Imprimir_Indices(char* nome_arq_ind){
	FILE *arquivo;
	char pag[200];
	int NRR = 0;

	arquivo = fopen(nome_arq_ind, "r+");
	while(!feof(arquivo)){
		fscanf(arquivo,"%200[^\n]", pag);
		printf("%3d %s\n", NRR, pag);
		fgetc(arquivo);
		NRR ++;
	}
	fclose(arquivo);
}