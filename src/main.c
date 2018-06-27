#include <stdio.h>
#include <stdlib.h>
#include "../include/Registros.h"


int main (){
	char nome_arq[] = "lista.txt";
	char nome_arq_ind[] = "indicelista.bt";
	ArvB* Arvore;
	int ordem = 4;

	Arvore = Criar_ArvB(ordem);

	Criar_Indices(Arvore, nome_arq, nome_arq_ind);
	Imprimir_Indices(nome_arq_ind);
	ImprimirArvore(Arvore);

	return 0;
}