#ifndef ARVORE_B_TEMP_
#define ARVORE_B_TEMP_

#include "Arvore_B.h"

/* Aloca o espaço para um nó. */
No* AlocarNo(int ordem);

/* Função que realiza o split de um nó (deve estar cheio). */
void DividirFilho(No* pai, No* no_1, int i, int ordem);

/* Função que insere um indice em um nó não chieo. */
void InserirNaoCheio(No* no, char chave[], int reg_NRR, int ordem);

/* Função que insere um novo item em um nó. */
void InserirItem(ArvB* arv, char chave[], int reg_NRR);

#endif