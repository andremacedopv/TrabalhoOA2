#ifndef REGISTRO_
#define REGISTRO_
#include "Arvore_B.h"
#include "Arvore_B-temp.h"

/* Função que completa um string com espaços. */
void Preenche_espacos(char* string, int tam);

/* Função que insere um novo registro. */
void Inserir_Registro(ArvB* Arvore, char* nome_arq, char* nome_arq_ind);

/* Cria indices a partir de um arquivo de dados no formato de árvore B */
void Criar_Indices(ArvB* Arvore, char* nome_arq, char* nome_arq_ind);

/* Cria uma chave a partir de um registro */
void Criar_chave(char* reg, char* chave);

/* Função provisória */
void Imprimir_Indices(char* nome_arq_ind);

/* Insere um registro no arquivo de dados e no arquivo de índices */
void Inserir_Registro(ArvB* Arvore, char* nome_arq, char* nome_arq_ind);

/* Imprimir arquivo no teminal. */
void ImprimirArquivo(char* nome_arq);

#endif
