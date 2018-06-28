#ifndef ARVORE_B_TEMP_
#define ARVORE_B_TEMP_

#include "Arvore_B.h"

typedef struct reg{
	char nome[41];
	int matricula;
	char curso[3];
	char turma;
}Reg;

/* Aloca o espaço para um nó. */
No* AlocarNo(int ordem);

/* Função que realiza o split de um nó (deve estar cheio). */
void DividirFilho(No* pai, No* no_1, int i, int ordem);

/* Função que insere um indice em um nó não chieo. */
void InserirNaoCheio(No* no, char chave[], int reg_NRR, int ordem);

/* Função que insere um novo item em um nó. */
void InserirItem(ArvB* arv, char chave[], int reg_NRR);

/* Imprimir o nó de uma árvore no terminal. */
int ImprimirNo(No* no, int idx_pai, int idx_atual);

/* Imprimir árvore no terminal. */
void ImprimirArvore(ArvB *arv);

#endif