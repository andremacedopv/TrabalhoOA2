#include <stdio.h>
#include <stdlib.h>
#include "../include/Registros.h"
#include "../include/Arvore_B.h"

#define SAIR 0
#define INCLUIR 1
#define REMOVER 2
#define BUSCAR 3
#define IMPRIMIR_ARV 4
#define IMPRIMIR_IND 5
#define IMPRIMIR_DAD 6

int main(){

  printf("\n#########################################################\n");
  printf("#        Trabalho 2 - Organização de Arquivos           #\n");
  printf("#########################################################\n\n");

	/* Prguntar para o usuário qual a ordem a ser utilizada. */
 	int ordem;

 	do{
		printf("Digite a ordem da Árvore-B:\n");
    scanf("%d", &ordem);
    if(ordem < 3)
      printf("ERRO: Digite uma ordem par maior do que 3.\n");
    if(ordem%2 != 0)
      printf("ATENÇÂO: ordens ímpares não são garantidas de funcionarem corretamente. De preferência a ordens pares.\n"); 
 	}while(ordem < 4);

	/* Ler os dados e criar a árvore e o arquivo de indíce. */
	char nome_arq[] = "lista.txt";
  	char nome_arq_ind[] = "indicelista.bt";
  	ArvB* Arvore = Criar_ArvB(ordem);

  	Criar_Indices(Arvore, nome_arq, nome_arq_ind);

  	printf("Árvore e indices criados. Imprimindo árvore...\n\n");
  	ImprimirArvore(Arvore);

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
            printf("\n");
            Imprimir_Indices(nome_arq_ind);
            printf("\n");
            ImprimirArvore(Arvore);
   	     	break;

      		case REMOVER:
       			Remover_Registro(Arvore, nome_arq, nome_arq_ind);
            printf("\n");
      			Imprimir_Indices(nome_arq_ind);
      			printf("\n");
            ImprimirArvore(Arvore);
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
         	 	LiberarArvore(Arvore);
        		continuar = 0;
    		break;

    		default:
    			printf("Digite uma opção válida.\n");
    	} /* switch */
	}while(continuar);

	return 0;
}
