#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Arvore_B.h"
#include "Arvore_B-temp.h"

void LiberarNo(No* no){
	int i;
	/* Liberar filhos. */
	for(i=0; i<no->n_filhos; i++){
		LiberarNo(no->filho[i]);
	}

	/* Libear no. */
	free(no->indice);
	free(no->filho);
	free(no->filhos_NRR);
	free(no);
}

void LiberarArvore(ArvB* arvore){
	/* Libera raiz. */
	LiberarNo(arvore->raiz);

	/* Liberar arvore. */
	free(arvore);
}

No* AlocarNo(int ordem){
	/* Alocar no e vetores de indices e filhos. */
	No* no = (No*) malloc(sizeof(No));
	no->indice = (Ind*) malloc((ordem-1) * sizeof(Ind));
	no->filho = (No**) malloc(ordem * sizeof(No*));
	no->filhos_NRR = (int*) malloc(ordem * sizeof(int));

	/* Iniciar com -1 filhos_NRR. */
	for(int i = 0; i < ordem; i++)
		no->filhos_NRR[i] = -1;

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
		/* Ver se o filho que vai receber o novo nó está cheio. */
		if(no->filho[i+1]->n_ind == ordem-1){
			DividirFilho(no, no->filho[i+1], i+1, ordem);

			/* Ver onde será inserida a nova chave. */
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

void LerNo(char* no, int tamanho, FILE* indice){
	no[tamanho] = '\0';

	/* Ler no. */
	fgets(no, tamanho, indice);
}

void CopiarString(char* chave, char* NRR, char* origem, int numero){
	for(int i = 0; i < 8; i++){
		chave[i] = origem[i + 13*numero];
	}
	for(int i = 0; i < 3; i++){
		NRR[i] = origem[i + 9 + 13*numero];
	}
}

int ProximoSeek(char* no, int local){
	char seek[4];
	seek[3] = '\0';
	for(int i =0; i<3; i++){
		seek[i] = no[local + i];
	}

	return atoi(seek);
}

void LerRegistro(FILE* dados, int NRR, Reg* registro){
	int i;
	fseek(dados, 54*NRR, SEEK_SET);
	for(i=0; i < 40; i++){
		registro->nome[i] = fgetc(dados);
	}
	registro->nome[40] = '\0';

	fgetc(dados);
	char matricula[6];
	for(i=0; i < 5; i++){
		matricula[i] = fgetc(dados);
	}
	matricula[5] = '\0';
	registro->matricula = atoi(matricula);

	fgetc(dados);
	fgetc(dados);
	for(i=0; i < 2; i++){
		registro->curso[i] = fgetc(dados);
	}
	registro->curso[2] = '\0';

	fgetc(dados);
	fgetc(dados);
	registro->turma = fgetc(dados);
}

int BuscarIndice(char *chave, Reg* registro){
	FILE* indice, *dados;
	int i;
	int seeks = 0;
	/* Abrir arquivo de índice. */
	indice = fopen("indicelista.bt", "r+");

	/* Pegar ordem da arvore. */
	char ordem_s[4];
	ordem_s[3] = '\0';
	for(i=0; i < 3; i++)
		ordem_s[i] = fgetc(indice);
	int ordem = atoi(ordem_s);

	/* Descobir tamanho do registro.*/
	int tam_reg = (ordem-1)*13 + ordem*4 + 1;

	/* Descobrir raiz. */
	fseek(indice, 0, SEEK_END);
	long int tell = ftell(indice);
	fseek(indice, tell - tam_reg, SEEK_SET);
	seeks++;

	/* Achar registro. */
	int NRR, next_seek;
	char temp[TAM_CHAVE], NRR_s[4], no[tam_reg+1];
	temp[TAM_CHAVE-1] = '\0';
	NRR_s[3] = '\0';
	int achou = 0;
	int continuar = 1;
	while(continuar){
		LerNo(no, tam_reg, indice);
		for(i=0; i < ordem-1; i++){
			CopiarString(temp, NRR_s, no, i);
			if(strcmp(chave, temp) == 0){
				achou = 1;
				continuar = 0;
				i = ordem;
				NRR = atoi(NRR_s);
			} /* if */
			else if(strcmp(chave, temp) < 0){
				next_seek = ProximoSeek(no, 13*(ordem-1) + i*ordem);
				if(next_seek == -1){
					achou = 0;
					continuar = 0;
				}
				else{
					seeks++;
					fseek(indice, 4 + tam_reg*next_seek, SEEK_SET);
				}
				i = ordem;
			} /* else if */
			else{
				if(temp[1] != '#'){
					if(i == ordem-2){
						next_seek = ProximoSeek(no, 13*(ordem-1) + (i+1)*ordem);
						if(next_seek == -1){
							achou = 0;
							continuar = 0;
						}
						else{
							seeks++;
							fseek(indice, 4 + tam_reg*next_seek, SEEK_SET);
						}
						i = ordem;
					} /* if */
				} /* if */
				else{
					next_seek = ProximoSeek(no, 13*(ordem-1) + i*ordem);
					if(next_seek == -1){
						achou = 0;
						continuar = 0;
					}
					else{
						seeks++;
						fseek(indice, 4 + tam_reg*next_seek, SEEK_SET);
					}
					i = ordem;
				} /* else */
			} /* else */
		} /* for */
	} /* while */

	if(achou){
		dados = fopen("lista.txt", "r+");
		LerRegistro(dados, NRR, registro);
		fclose(dados);
	}
	else{
		registro->nome[1] = '#';
	}

	fclose(indice);

	return seeks;
}

void Buscar_Registro(){
	/* Achar chave a ser procurada. */
	char chave[9];
	printf("Insira a chave do registro a ser pesquisado:\n");
	scanf("%8s", chave);

	/* Achar registro. */
	Reg *registro = (Reg*) malloc(sizeof(Reg));
	int seeks;
	seeks = BuscarIndice(chave, registro);

	/* Imprimir resultado. */
	if(registro->nome[1] == '#'){
		printf("Registro não encontrado na base de dados.\n");
		printf("Seeks necessários: %d seeks.\n", seeks);
	}
	else{
		printf("Registro encontrado:\n");
		printf("Nome: %s\n", registro->nome);
		printf("Matrícula: %d\n", registro->matricula);
		printf("Curso: %s\n", registro->curso);
		printf("Turma: %c\n", registro->turma);
		printf("Seeks necessários: %d\n\n", seeks);
	}

	free(registro);
}

int AcharNRR(char *chave){
	FILE* indice;
	int i;
	/* Abrir arquivo de índice. */
	indice = fopen("indicelista.bt", "r+");

	/* Pegar ordem da arvore. */
	char ordem_s[4];
	ordem_s[3] = '\0';
	for(i=0; i < 3; i++)
		ordem_s[i] = fgetc(indice);
	int ordem = atoi(ordem_s);

	/* Descobir tamanho do registro.*/
	int tam_reg = (ordem-1)*13 + ordem*4 + 1;

	/* Descobrir raiz. */
	fseek(indice, 0, SEEK_END);
	long int tell = ftell(indice);
	fseek(indice, tell - tam_reg, SEEK_SET);

	/* Achar registro. */
	int NRR, next_seek;
	char temp[TAM_CHAVE], NRR_s[4], no[tam_reg+1];
	temp[TAM_CHAVE-1] = '\0';
	NRR_s[3] = '\0';
	int achou = 0;
	int continuar = 1;
	while(continuar){
		LerNo(no, tam_reg, indice);
		for(i=0; i < ordem-1; i++){
			CopiarString(temp, NRR_s, no, i);
			if(strcmp(chave, temp) == 0){
				achou = 1;
				continuar = 0;
				i = ordem;
				NRR = atoi(NRR_s);
			} /* if */
			else if(strcmp(chave, temp) < 0){
				next_seek = ProximoSeek(no, 13*(ordem-1) + i*ordem);
				if(next_seek == -1){
					achou = 0;
					continuar = 0;
				}
				else{
					fseek(indice, 4 + tam_reg*next_seek, SEEK_SET);
				}
				i = ordem;
			} /* else if */
			else{
				if(temp[1] != '#'){
					if(i == ordem-2){
						next_seek = ProximoSeek(no, 13*(ordem-1) + (i+1)*ordem);
						if(next_seek == -1){
							achou = 0;
							continuar = 0;
						}
						else{
							fseek(indice, 4 + tam_reg*next_seek, SEEK_SET);
						}
						i = ordem;
					} /* if */
				} /* if */
				else{
					next_seek = ProximoSeek(no, 13*(ordem-1) + i*ordem);
					if(next_seek == -1){
						achou = 0;
						continuar = 0;
					}
					else{
						fseek(indice, 4 + tam_reg*next_seek, SEEK_SET);
					}
					i = ordem;
				} /* else */
			} /* else */
		} /* for */
	} /* while */

	fclose(indice);

	if(achou){
		return NRR;
	}
	else{
		return -1;
	}
}