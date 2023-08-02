#include "encrypt.h"
#include "helpers.h"
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>

// TODO: debugar em relacao a ausencia de texto

//char somachar(char, char, char); // soma ou diminui caracteres, arg3 pode ser '+' ou '-'
void numtex(int, char *);				// converte numero em string
int combina(char *, char *, char, int); // combina senha e texto, rotorna o quanto falta para completar a senha
void salva(char *);
int formata(char *, int);	  // pula algumas linhas no arquivo
int calcdata(char *, int);	  // imprime a data na posicao indicada
int ajusta(char *, int, int); // ajusta texto dado tamanho da senha
void pergsalva(char *);		  // pergunta e salva


int formata(char *texto, int pos) {

	int i = 0;

	while ((LINHAS - i) > 0) {
		texto[pos + i] = '\n';
		i++;
	}

	return (pos + i);
}

int ajusta(char *texto, int pos, int fix) {

	fix--; // chute
	while (fix > 0) {
		texto[pos++] = ' ';
		fix--;
	}

	return (pos);
}

void escreve(char *senha) {
	char texto[65536];
	int posicao = 0, fix = 0;
	// pega texto
	// combina texto e senha
	// salva
	posicao = formata(texto, posicao); // formata
	posicao = calcdata(texto, posicao); // adiciona data ao inicio da string
	posicao = formata(texto, posicao); // formata

	int tamanhoInput = 0;

	do {
		texto[posicao] = '\n';
		posicao++;
		tamanhoInput = posicao;
		pegastr(&texto[posicao]); // pega texto
		posicao = strlen(texto);  // calcula o tamanho da string pega acima, scanf de merda
		tamanhoInput = posicao - tamanhoInput;
	} while (tamanhoInput > 0);

	posicao = formata(texto, posicao);
	fix = combina(texto, senha, '-', 0); // combina texto e senha
	posicao = ajusta(texto, posicao, fix); // ajusta senha
	pergsalva(texto); // pergunta e salva (ou nao ne)
}


void numtex(int numero, char *texto) {

	int copia = numero, tamanho = 0; // c�pia

	while (copia) { // calcula grandeza do numero
		copia = copia / 10;
		tamanho++;
	}

	texto[tamanho] = '\0'; // define final da string

	while (tamanho) {
		texto[tamanho - 1] = (numero % 10 + '0');
		numero /= 10;
		tamanho--;
	}
}

int calcdata(char *data, int pos) {

	time_t agora; // variavel de tempo
	time(&agora); // funcao nada a ver
	// ctime(&agora));					//funcao que retorna uma string com a data ->  ctime(&agora)

	struct tm *minhahr = localtime(&agora);
	//	printf("ano:%d mes:%d dia%d hora:%d\n\n", (minhahr->tm_year)+1900,minhahr->tm_mon,minhahr->tm_mday,minhahr->tm_hour);

	int dia = minhahr->tm_mday;
	int mes = minhahr->tm_mon;
	int ano = minhahr->tm_year;
	int hora = minhahr->tm_hour;
	int min = minhahr->tm_min;

	// dia

	numtex(dia, &data[pos]);
	if (dia < 10) { // se for menor que 10 adiciona um caracter '0' antes da string
		data[pos + 1] = data[pos];
		data[pos++] = '0';
		pos++;
	}
	else {
		pos += 2;
	}

	data[pos++] = '/'; // espaco entre mes

	// mes
	numtex(mes + 1, &data[pos]);
	if (mes + 1 < 10) {
		data[pos + 1] = data[pos];
		data[pos++] = '0';
		pos++;
	}
	else {
		pos += 2;
	}

	data[pos++] = '/'; // espaco entre mes
	// ano
	numtex(ano + 1900, &data[pos]);
	pos += 4; // copia ano para data

	data[pos++] = ' '; // adiciona espaco entre a data e a hora

	// hora
	if (hora == 0)
		data[pos] = '0'; // horas, minutos podem ser igais a zero
	else
		numtex(hora, &data[pos]);
	if (hora < 10) {
		data[pos + 1] = data[pos];
		data[pos++] = '0';
		pos++;
	}
	else {
		pos += 2;
	}

	data[pos++] = ':'; // simbolo entre hora e minuto

	// min
	if (min == 0)
		data[pos] = '0'; // horas, minutos podem ser igais a zero
	else
		numtex(min, &data[pos]);
	if (min < 10) {
		data[pos + 1] = data[pos];
		data[pos++] = '0';
		pos++;
	}
	else {
		pos += 2;
	}
	return (pos); // posicao final
}

int combina(char *texto, char *senha, char operacao, int posicao) {
	int tamanhosenha = strlen(senha), poscharsen;
	do {
		poscharsen = posicao % tamanhosenha;
		texto[posicao] = somachar(texto[posicao], senha[poscharsen], operacao); // calcula o caractere
		posicao++;
	} while (texto[posicao] != '\0');
	return (tamanhosenha - poscharsen); // retorna quanto falta para completar senha
}

void salva(char *texto) {
	FILE *arq;
	arq = fopen(NOMEARQ, "a");
	int posicao = 0;
	do {
		fwrite(&texto[posicao], sizeof(char), 1, arq); // escreve no arquivo
	} while (texto[++posicao] != '\0');
	fclose(arq);
}

void pergsalva(char *texto) {

	char modo = '\0';

	while (modo == '\0') {
		printf("\nsalvar?(S/N) ");
		modo = pegachar();
		printf("\n");
		if (modo == 'S' || modo == 's') {
			printf("\nSalvando...\n");
			salva(texto);
		}
		else if (!(modo == 'n' || modo == 'N'))
			modo = '\0';
	}
}