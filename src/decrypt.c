#include <conio.h>
#include "decript.h"
#include "defs.h"
#include "string.h"
#include "helpers.h"

int findPage(char *decoded, int start, int flag);
void printPage(char *decoded, int cursorBegining, int cursorEnd);
char *decodeFile(FILE *file, char *senha);

void learq(char *senha) {
	int tamanhosenha = strlen(senha);
	int decodedLength = 0;
	char c = '\0';

	FILE *arq = NULL;
	arq = fopen(NOMEARQ, "r");
	if (arq == NULL) {
		printf("arquivo inexistente\n");
		return;
	}

	//#TODO problemas se a senha contiver espaco (nao deveria conter espaco, mas ne)
	char *decoded = decodeFile(arq, senha);
	fclose(arq);
	decodedLength = strlen(decoded);
	int cursorBegining = 0;
	int cursorEnd = findPage(decoded, 0, NEXT_PAGE);

	printPage(decoded, cursorBegining, cursorEnd);

	while (c != 'w' && c != 'W') {
		c = _getch();
		if (c == 'd' || c == 'D') { // proxima pagina
			if (cursorEnd < decodedLength) {
				cursorBegining += findPage(decoded, cursorBegining, NEXT_PAGE);
				cursorEnd += findPage(decoded, cursorEnd, NEXT_PAGE);
				printPage(decoded, cursorBegining, cursorEnd);
			}
		}
		else if (c == 'a' || c == 'A') { // pagina anterior
			if (cursorBegining > 0) {
				cursorBegining += findPage(decoded, cursorBegining, PREVIOUS_PAGE);
				cursorEnd += findPage(decoded, cursorEnd, PREVIOUS_PAGE);
				printPage(decoded, cursorBegining, cursorEnd);
			}
		}
		else if (c == 'e' || c == 'E') { // ultima pagina
			while (cursorEnd < decodedLength) {
				cursorBegining += findPage(decoded, cursorBegining, NEXT_PAGE);
				cursorEnd += findPage(decoded, cursorEnd, NEXT_PAGE);
			}
			printPage(decoded, cursorBegining, cursorEnd);
		}
	}
	system("cls");
	free(decoded);
}

char *decodeFile(FILE *file, char *senha) {
	int i = 0, poscharsen = 0, fileSize, valid;
	char c;
	char *decoded;

	fseek(file, 0L, SEEK_END);
	fileSize = ftell(file) + 1;
	rewind(file);
	decoded = (char *)malloc(sizeof(char) * fileSize + 1);
	int tamanhosenha = strlen(senha);

	for (i = 0; i < fileSize; i++) {
		poscharsen = i % tamanhosenha;
		valid = fscanf(file, "%c", &c);
		if (valid) {
			decoded[i] = (somachar(c, senha[poscharsen], '+'));
		}
		else {
			printf("Erro de leitura\n");
			rewind(file);
			return '\0';
		}
	}
	decoded[i] = '\0';
	rewind(file);
	return decoded;
}

/* TODO: usar uma estrutra de dados apropriada, usar uma funcao que converte
 o arquivo antigo, porque a solucao agora eh usar essa  gambiarra pra dar 
 estrutura de pagina ao arquivo que eh somente uma string. */
int findPage(char *decoded, int start, int flag) {
	int cursor = 0, prevCursor = 0;
	decoded += start;

	if (flag == NEXT_PAGE) {
		while (decoded[cursor] == '\n' || decoded[cursor] == ' ')
			cursor++;
	}
	else {
		while (decoded[cursor] == '\n' || decoded[cursor] == ' ')
			cursor--;
		if (decoded[cursor] == '\0')
			cursor--; // para entrar no loop
	}
	while (decoded[cursor] != '\0') {
		if (flag == NEXT_PAGE) {
			cursor++;
			if (decoded[cursor] == '\0')
				break;
			if (decoded[cursor] == '\n') {
				cursor++;
				if (decoded[cursor] == '\0')
					break;
				while (decoded[cursor] == ' ')
					cursor++;
				if (decoded[cursor] == '\0')
					break;
				if (decoded[cursor] == '\n') {
					cursor++;
					if (decoded[cursor] == '\0')
						break;
					while (decoded[cursor] == ' ')
						cursor++;
					if (decoded[cursor] == '\0')
						break;
					if (decoded[cursor] == '\n') {
						cursor++;
						if (decoded[cursor] == '\0')
							break;
						while (decoded[cursor] == ' ')
							cursor++;
						if (decoded[cursor] == '\0')
							break;
						if (decoded[cursor] == '\n') {
							break;
						}
					}
				}
			}
			if (decoded[cursor] == '\0')
				break;
		}
		else { // find previous
			cursor--;
			if (cursor + start <= 0)
				break;
			prevCursor = cursor;
			while (decoded[cursor] == ' ')
				cursor--;
			if (cursor + start <= 0)
				break;
			if (decoded[cursor] == '\n') {
				cursor--;
				if (cursor + start <= 0)
					break;
				while (decoded[cursor] == ' ')
					cursor--;
				if (cursor + start <= 0)
					break;
				if (decoded[cursor] == '\n') {
					cursor--;
					if (cursor + start <= 0)
						break;
					while (decoded[cursor] == ' ')
						cursor--;
					if (cursor + start <= 0)
						break;
					if (decoded[cursor] == '\n') {
						cursor--;
						if (cursor + start <= 0)
							break;
						while (decoded[cursor] == ' ')
							cursor--;
						if (cursor + start <= 0)
							break;
						if (decoded[cursor] == '\n') {
							cursor = prevCursor;
							break;
						}
					}
				}
			}
		}
	}
	return cursor;
}

void printPage(char *decoded, int cursorBegining, int cursorEnd) {
	char header[] = "[D]->proxima, [A]->anterior, [E]->ultima, [W]->escrever no diario";
	int startPos = 0, endPos = 0;
	startPos = cursorBegining;
	while (decoded[startPos] == ' ' || decoded[startPos] == '\n')
		startPos++;
	endPos = cursorEnd;
	while (decoded[endPos] == ' ' || decoded[endPos] == '\n' || decoded[endPos] == '\0')
		endPos--;
	system("cls");
	printf("%s\n\n%.*s\n", header, endPos - startPos + 1, decoded + startPos);
}