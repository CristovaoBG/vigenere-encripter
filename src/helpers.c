#include "helpers.h"
#include <string.h>

char pegachar() {
	char a;
	do {
		a = getchar();
		fflush(stdin); // limpa buffer
	} while (a == '\n' || a == '\0');
	return (a);
}

int pegastr(char *string) {
	scanf("%[^\n]", string); // pega string ate chegar o \n
	fflush(stdin);			 // limpa buffer
	return (strlen(string));
}


void copiastr(char *destino, char *origem) { /*Minha versao do strcpy credo*/

	int i = 0;
	while (origem[i] != '\0') {
		destino[i] = origem[i];
		i++;
	}
	destino[i++] = '\0';
}

char somachar(char a, char b, char op) {
	int resultado = a;
	if (op == '+') {
		if (a >= 'a' && a <= 'z') {							   // se a for uma letra minuscula
			resultado = a + (b - 'a'); // soma � senha
			if (resultado > 'z')
				resultado -= 'z' - 'a' + 1; // retira o total de letras caso exceda
		}
		else if (a >= 'A' && a <= 'Z') { // se a for uma letra maiuscula
			resultado = a + (b - 'a');
			if (resultado > 'Z')
				resultado -= 'z' - 'a' + 1;
		}
	}
	else if (op == '-') {
		if (a >= 'a' && a <= 'z') { // se a for uma letra minuscula
			resultado = a - (b - 'a');
			if (resultado < 'a')
				resultado += 'z' - 'a' + 1;
		}
		else if (a >= 'A' && a <= 'Z') { // se a for uma letra maiuscula
			resultado = a - (b - 'a');
			if (resultado < 'A')
				resultado += 'z' - 'a' + 1;
		}
	}

	return (resultado);
}