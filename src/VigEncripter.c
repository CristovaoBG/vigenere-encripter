#include "decript.h"
#include "helpers.h"
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>


//TODO: ver warnings
int main() {
	char senha[256], modo = '\0';
	printf("Senha: ");
	pegastr(senha);
	printf("\n\n");
	learq(senha);
	printf("Deseja escrever? (S/N) \n->");
	modo = pegachar();
	if (modo == 's' || modo == 'S') {
		system("cls");
		printf("-> ");
		escreve(senha);
	}
	printf("\n\n");
	return 0;
}

