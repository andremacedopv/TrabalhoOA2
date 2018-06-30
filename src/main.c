#include <stdio.h>
#include <stdlib.h>
#include "../include/Registros.h"

#define SAIR 0
#define INCLUIR 1
#define REMOVER 2
#define BUSCAR 3
#define IMPRIMIR_ARV 4
#define IMPRIMIR_IND 5
#define IMPRIMIR_DAD 6

int main(){
	/* Prguntar para o usuário qual a ordem a ser utilizada. */
 	int ordem;
    char chave[9];

 	do{
		printf("Digite a ordem da Árvore-B:\n");
    scanf("%d", &ordem);
    if(ordem%2 != 0 || ordem < 3)
      printf("ERRO: Digite uma ordem par maior do que 3.\n");
 	}while(ordem < 4 || ordem%2 != 0);

	/* Ler os dados e criar a árvore e o arquivo de indíce. */
	char nome_arq[] = "lista.txt";
  	char nome_arq_ind[] = "indicelista.bt";
  	ArvB* Arvore = Criar_ArvB(ordem);

  	Criar_Indices(Arvore, nome_arq, nome_arq_ind);

  	printf("Árvore e indices criados. Imprimindo árvore...\n\n");
  	ImprimirArvore(Arvore);
  	printf("\n");

	/* Perguntar o que o usuário deseja fazer. */
  	int continuar = 1;
  	int opcao;
  	do{
    	printf("\n------------------------------------\n");
    	/* Imprimir opções. */
    	printf("Digite opção desejada:\n");
    	printf("(1) - Incluir novo registro;\n");
    	printf("(2) - Remover registro existente;\n");
    	printf("(3) - Buscar Registro;\n");
    	printf("(4) - Imprimir Árvore;\n");
    	printf("(5) - Imprimir Arquivo de Indíces;\n");
    	printf("(6) - Imprimir Arquivo de Dados;\n");
    	printf("(0) - Sair do programa.\n");
    	scanf("%d", &opcao);

	    /* Realizar opção desejada. */
   	 	switch(opcao){
   	 		case INCLUIR:
   	   			Inserir_Registro(Arvore, nome_arq, nome_arq_ind);
   	     	break;

      		case REMOVER:
	      		ImprimirArvore(Arvore);
      			printf("Insira uma chave:\n");
      			scanf("%8s", chave);
      			RemoverItem(Arvore->raiz, Arvore->ordem, chave);
      			Salvar_ArvB(Arvore, nome_arq_ind);
      			Imprimir_Indices(nome_arq_ind);
      			ImprimirArvore(Arvore);
       			//Remover_Registro();
    		break;

    		case BUSCAR:
    			Buscar_Registro();
    		break;

    		case IMPRIMIR_ARV:
    			ImprimirArvore(Arvore);
    		break;

    		case IMPRIMIR_IND:
    			ImprimirArquivo(nome_arq_ind);
    		break;

    		case IMPRIMIR_DAD:
    			ImprimirArquivo(nome_arq);
    		break;

    		case SAIR:
    			/* LIberar arvore. */
          LiberarArvore(Arvore);
        	continuar = 0;
    		break;

    		default:
    			printf("Digite uma opção válida.\n");
    	} /* switch */
	}while(continuar);

	return 0;
}
