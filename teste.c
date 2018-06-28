#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *fp = fopen("lista.txt", "r+"); 
	//fseek(fp, 4 + 56*2, SEEK_SET);
	//fseek(fp, 0, SEEK_END);
	//long int tell = ftell(fp);
	//fseek(fp, tell - 56, SEEK_SET);
	fseek(fp, 54, SEEK_SET);
	printf("%c\n", fgetc(fp));
	fclose(fp);
	return 0;
}