#ifndef ARVORE_B_
#define ARVORE_B_

#include <stdio.h>

/* Tamanho ocupado pelos campos nos registros incluindo um espaço vazio */
#define TAM_REG 54       
#define TAM_CHAVE 9
#define TAM_NRR 4

typedef struct ind{
	char chave[TAM_CHAVE];
	int reg_NRR;
}Ind;

typedef struct no{
	int n_ind;
	int n_filhos;
	Ind *indice;
	int *filhos_NRR;
	struct no **filho;
}No;

typedef struct arvb{
	No *raiz;
	int ordem;
}ArvB;

typedef struct reg{
	char nome[41];
	int matricula;
	char curso[3];
	char turma;
}Reg;

/* Cria uma Árvore B vazia */
ArvB* Criar_ArvB(int ordem);

/* Busca por uma chave em um no e retorna o índice correspondente */
int Busca(No* no, int ordem, char *chave, char *nome_arq);

/* Salva recursivamente todos os nós a partir  do nó dado como parâmetro, em um arquivo */
int Salvar_No(No* no, int ordem, FILE *arquivo);

/* Salva uma árvore B em um arquivo */
void Salvar_ArvB(ArvB* Arvore, char* nome_arq_ind);

/* Encontra a posição da chave no nó */
int EncontrarChave(No* no, char* chave);

/* Concatena nós */
void Merge(No* no, int ordem, int idx);

/* Pega um indice emprestado com o irmão da direita */
void PedirEmprestadoProx(No* no, int idx);

/* Pega um indice emprestado com o irmão da esquerda */
void PedirEmprestadoAnt(No* no, int idx);

/* Preenche o nó caso ele não tenha o número mínimo de indices */
void PreencherNo(No* no, int ordem, int idx);

/* Encontra o sucessor de um ínice em um nó folha */
Ind PegarSuc(No* no);

/* Pega o predecessor de um índice em um nó folha */
Ind PegarPred(No* no);

/* Remove o índice de um nó não folha */
void RemoverNaoFolha(No* no, int ordem, int idx);

/* Remove o índice de um nó folha */
void RemoverFolha(No* no, int idx);

/* Remove um índice de um nó ou de seus descendentes */
void RemoverItem(No* no, int ordem, char* chave);

/* Remove um índice de uma Arvore B */
void RemoverArvB(ArvB* Arvore, char* chave);

/* Liberar no da arvore. */
void LiberarNo(No* no);

/* Libear arvore. */
void LiberarArvore(ArvB* arvore);

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

/* Ler um nó no arquivo de dados. */
void LerNo(char* no, int tamanho, FILE* indice);

/* Dividir string em uma chave e um NRR. */
void CopiarString(char* chave, char* NRR, char* origem, int numero);

/* Descobrir posição do próximo seek. */
int ProximoSeek(char* no, int local);

/* Ler um registro do arquivo de dados. */
void LerRegistro(FILE* dados, int NRR, Reg* registro);

/* Buscar um indice no arquivo de indices. */
int BuscarIndice(char *chave, Reg* registro);

/* Buscar um registro no arquivo de dados. */
void Buscar_Registro();

/* Função que encontra o NRR de um registro no arquivo de dados. */
int AcharNRR(char *chave);

#endif