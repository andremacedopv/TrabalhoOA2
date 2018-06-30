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

void RemoverArvB(ArvB* Arvore, char* chave){

	RemoverItem(Arvore->raiz, Arvore->ordem, chave);
	if(Arvore->raiz->n_ind == 0){
		Arvore->raiz = Arvore->raiz->filho[0];
	}
}

void RemoverItem(No* no, int ordem, char* chave){
	int idx = EncontrarChave(no, chave);
	int minimo =(ordem/2 - 1);
	int flag;

	if(idx < no->n_ind && strcmp(no->indice[idx].chave, chave) == 0){
		if(no->n_filhos == 0){
			RemoverFolha(no, idx);
		}
		else{
			RemoverNaoFolha(no, ordem, idx);
		}
	}
	else{
		/* Se o nó for folha a chave não está no arvore*/
		if(no->n_filhos == 0){
			return;
		}

		flag = ( (idx==no->n_ind)? 1:0);

		/* Preenche se for o filho em que a chave deveria estar e ele tiver
		n_ind < minimo */

		if (flag && idx > no->n_ind){
			RemoverItem(no->filho[idx-1], ordem, chave);
			
		}
		else{
			RemoverItem(no->filho[idx], ordem, chave);
		}
		if(no->filho[idx]->n_ind < minimo && no->filho[idx]->n_ind > 0){
			PreencherNo(no, ordem, idx);
		}
	}
	return;
}


void RemoverFolha(No* no, int idx){
	/* Move todos os índices após idx par tras*/
	for(int i=idx+1; i<no->n_ind; i++){
		strcpy(no->indice[i-1].chave, no->indice[i].chave);
		no->indice[i-1].reg_NRR = no->indice[i].reg_NRR;
	}
	no->n_ind--;

	return;
}

void RemoverNaoFolha(No* no, int ordem, int idx){
	int minimo;
	Ind pred, suc;

	minimo = (ordem/2 - 1);
	if(no->filho[idx+1]->n_ind > minimo){
		suc = PegarSuc(no->filho[idx+1]);
		strcpy(no->indice[idx].chave, suc.chave);
		no->indice[idx].reg_NRR = suc.reg_NRR;
		RemoverItem(no->filho[idx+1], ordem, suc.chave);
	}
	else if(no->filho[idx]->n_ind > minimo){
		pred = PegarPred(no->filho[idx]);
		strcpy(no->indice[idx].chave, pred.chave);
		no->indice[idx].reg_NRR = pred.reg_NRR;
		RemoverItem(no->filho[idx], ordem,  pred.chave);
	}
	else{
		Merge(no, ordem, idx);
		RemoverItem(no, ordem, no->indice[idx].chave);
	}
	return;
} 

Ind PegarPred(No* no){
	No* aux;

	/* Percorre a árvore até chegar em um nó folha */
	aux = no;
	while(aux->n_filhos != 0){
		aux = aux->filho[aux->n_ind];
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
void PreencherNo(No* no, int ordem, int idx){
	int minimo;

	minimo = (ordem/2 - 1);

	/* Se o filho anterior tiver mais que o minimo-1 indices pega emprestado com ele */
	if(idx!=0 && no->filho[idx - 1]->n_ind > minimo){
		PedirEmprestadoAnt(no, idx);
	}
	/* Se o proximo filho tiver mais que o minimo-1 indices pega emprestado com ele */
	else if(idx != no->n_ind && no->filho[idx+1]->n_ind > minimo){
		PedirEmprestadoProx(no, idx);
	}
	/* Merge com o seu irmão */
	else{
		/* Se não for o ultimo filho, mescla com o proximo irmão*/
		if(idx != no->n_ind){
			Merge(no, ordem, idx);
		}
		/* Se for, mescla com o anterior */
		else{	
			Merge(no, ordem, idx-1);
		}
	}

	return;
}

void PedirEmprestadoAnt(No* no, int idx){
	No *filho1, *filho2;

	filho1 = no->filho[idx];
	filho2 = no->filho[idx-1];

	/* Move todos os índices para frente*/
	for(int i=filho1->n_ind-1; i>=0; i--){
		strcpy(filho1->indice[i+1].chave, filho1->indice[i].chave);
		filho1->indice[i+1].reg_NRR = filho1->indice[i].reg_NRR;
	}

	/* Se não for folha, move para frente todos os ponteiros 
	para os filhos */
	if(filho1->n_filhos != 0){                           
		for(int i=filho1->n_filhos-2; i>=0; i--){        
			filho1->filho[i+1] = filho1->filho[i];
			filho1->filhos_NRR[i+1] = filho1->filhos_NRR[i];
		}
	}

	/* Move para o filho1 o índice[idx-1] do no */
	strcpy(filho1->indice[0].chave, no->indice[idx-1].chave);
	filho1->indice[0].reg_NRR = no->indice[idx-1].reg_NRR;

	/* Move o último filho do filho2 para o primeiro do filho1 */
	if(filho1->n_filhos != 0){
		filho1->filho[0] = filho2->filho[filho2->n_filhos-1];
		filho1->filhos_NRR[0] = filho2->filhos_NRR[filho2->n_filhos-1];	
		filho2->filhos_NRR[filho2->n_filhos-1] = -01;
		filho1->n_filhos ++;
		filho2->n_filhos --;
	}

	/* Move o índice do filho2 para o no */
	strcpy(no->indice[idx-1].chave, filho2->indice[filho2->n_ind-1].chave);
	no->indice[idx-1].reg_NRR = filho2->indice[filho2->n_ind-1].reg_NRR;

	filho1->n_ind += 1;
	filho2->n_ind -= 1;

	return;
}

void PedirEmprestadoProx(No* no, int idx){
	No *filho1, *filho2;

	filho1 = no->filho[idx];
	filho2 = no->filho[idx+1];

	/* Copia indice[idx] do no para o ultimo indice do filho1 */
	strcpy(filho1->indice[filho1->n_ind].chave, no->indice[idx].chave);
	filho1->indice[filho1->n_ind].reg_NRR = no->indice[idx].reg_NRR;

	/* Insere o ultimo filho de filho2 no primeiro de filho1*/
	if(filho1->n_filhos != 0){
		filho1->filho[filho1->n_filhos] = filho2->filho[0];
		filho1->filhos_NRR[filho1->n_filhos] = filho2->filhos_NRR[0];
	}

	/* Copia o ultimo filho de filho2 para o primeiro de filho1 */
	strcpy(no->indice[idx].chave, filho2->indice[0].chave);
	no->indice[idx].reg_NRR = filho2->indice[0].reg_NRR;

	/* Move todos os índices do filho2 para tras */
	for(int i=1; i<filho2->n_ind; i++){
		strcpy(filho2->indice[i-1].chave, filho2->indice[i].chave);
		filho2->indice[i-1].reg_NRR = filho2->indice[i].reg_NRR;
	}

	/* Move todos os ponteiros para filhos de filho2 para tras */
	if(filho2->n_filhos != 0){
		for(int i=1; i<filho2->n_filhos; i++){
			filho2->filho[i-1] = filho2->filho[i];
			filho2->filhos_NRR[i-1] = filho2->filhos_NRR[i];
		}
		filho2->filhos_NRR[filho2->n_filhos-1] = -01;
		filho1->n_filhos ++;
		filho2->n_filhos --;
	}

	filho1->n_ind += 1;
	filho2->n_ind -=1;

	return;

}

void Merge(No* no, int ordem, int idx){
	No *filho1 = no->filho[idx];
	No *filho2 = no->filho[idx+1];

	int minimo = (ordem / 2 - 1);
	/* Pega o índice do nó atual e insere na posição minimo-1 do filho1*/
	strcpy(filho1->indice[minimo-1].chave, no->indice[idx].chave);
	filho1->indice[minimo-1].reg_NRR = no->indice[idx].reg_NRR;

	/* Copia as chaves do filho2 para o filho1 */
	for(int i=0; i<filho2->n_ind; i++){
		strcpy(filho1->indice[i+minimo].chave, filho2->indice[i].chave);
		filho1->indice[i+minimo].reg_NRR = filho2->indice[i].reg_NRR;
	}
	/* Copia os ponteiros para os filhos de filho2 para filho1 */
	if(filho1->n_filhos!=0){
		for(int i=0; i<filho2->n_filhos; i++){
			filho1->filho[i+minimo] = filho2->filho[i];
			filho1->filhos_NRR[i+minimo] = filho2->filhos_NRR[i];
		}
	}

	/* Move os indices do no atual a partir de idx+1
	 um passo para tras */
	for(int i=idx+1; i<no->n_ind; i++){
		strcpy(no->indice[i-1].chave, no->indice[i].chave);
		no->indice[i-1].reg_NRR = no->indice[i].reg_NRR;
	}
	/* Move os ponteiros para os filhos do nó atual a partir de idx+2
	um passo para tras */
	for(int i=idx+2; i<no->n_filhos; i++){
		no->filho[i-1] = no->filho[i];
	}

	/* Atualiza o número de chaves do nó atual */
	filho1->n_ind += filho2->n_ind+1;
	filho1->n_filhos += filho2->n_filhos;
	no->n_ind --;
	no->n_filhos --;
	no->filhos_NRR[idx+1] = -01;
	no->filhos_NRR[idx+1] = -01;

	/* Desaloca filho2 */
//	LiberarNo(filho2);

	return;
}

/* Função que retorna o o indice da primeira chave
maior ou igual a chave buscada */
int EncontrarChave(No* no, char* chave){
	int idx = 0;

	while(strcmp(no->indice[idx].chave, chave) < 0 && idx < no->n_ind){
		idx++;
	}
	return idx;
}

void LiberarNo(No* no){
	if(no != NULL){
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