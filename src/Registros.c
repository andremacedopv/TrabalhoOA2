#include "../include/Registros.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void Preenche_espacos(char* string, int tam){
	int i, j;

	for(i=0; i<tam; i++){
		if(string[i] == '\0'){
			for(j = i; j < tam -1; j++){
				string[j] = ' ';
			}
			string[j] = '\0';
			break;				
		}
	}	
}

void Inserir_Registro(ArvB* Arvore, char* nome_arq, char* nome_arq_ind){
	FILE* arquivo;
	char reg[TAM_REG+1], matricula[8], nome[42], curso[5], turma[2];
	char chave[TAM_CHAVE];
	int tamanho, NRR;

	printf("Insira a matricula:\n");
	scanf("\n%5s", matricula);
	printf("\n");
	printf("Insira o nome:\n");
	scanf("\n%40[^\n]", nome);
	printf("\n");
	printf("Insira a curso:\n");
	scanf("%3s", curso);
	printf("\n");
	printf("Insira a turma:\n");
	scanf("%1s", turma);
	printf("\n");

	//preenche os vetores com espaço
	Preenche_espacos(nome, 42);   
	Preenche_espacos(matricula, 8);
	Preenche_espacos(curso, 5);
	Preenche_espacos(turma, 2);

	/* concatena os campos do registro */
	strcpy(reg, nome);
	strncat(reg, matricula, 8);
	strncat(reg, curso, 5);
	strncat(reg, turma, 2);

	arquivo = fopen(nome_arq, "r+");
	fseek(arquivo, 0, SEEK_END);
	tamanho = ftell(arquivo);
	NRR = tamanho / TAM_REG;
	fprintf(arquivo, "%s\n", reg);
	fclose(arquivo);

	Criar_chave(reg, chave);
	InserirItem(Arvore, chave, NRR);
	Salvar_ArvB(Arvore, nome_arq_ind);
}

/*
void Remover_Registro(ArvB* Arvore, char* chave, char* nome_arq, char* nome_arq_ind){

	printf("")

}*/

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

	/* Salva a ordem da Árvore B */
	Salvar_ArvB(Arvore, nome_arq_ind);

}

void Imprimir_Indices(char* nome_arq_ind){
	FILE *arquivo;
	int NRR = 0, ordem, TAM_PAG;
	arquivo = fopen(nome_arq_ind, "r+");
	fscanf(arquivo,"%d", &ordem);

	/* Calcula o tamanho do registro correspondente a uma página da árvore b */
	TAM_PAG = (ordem - 1)*(TAM_CHAVE + TAM_NRR) + ordem*TAM_NRR;
	char pag[TAM_PAG+2];

	fgetc(arquivo);
	while(fgets(pag, TAM_PAG+2, arquivo)!= NULL){
		printf("%03d %s", NRR, pag);
		NRR ++;
	}
	fclose(arquivo);
}