#define LINHAS 2
#define NOMEARQ "dados.dy"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
/*
debugar em relacao a ausencia de texto
*/

enum enumeracao{
	NEXT_PAGE,
	PREVIOUS_PAGE
};


char pegachar(); //correção do getchar
int pegastr(char *); //correçao do scanf
void learq(char *);
void escreve(char *);
char somachar(char, char, char); //soma ou diminui caracteres, arg3 pode ser '+' ou '-'
void copiastr(char*, char*);
void numtex(int, char*); //converte numero em string
int combina(char *,char *, char, int); //combina senha e texto, rotorna o quanto falta para completar a senha
void salva(char *);
int formata(char *,int); //pula algumas linhas no arquivo
int calcdata(char*,int); //imprime a data na posicao indicada
int ajusta(char*,int,int); //ajusta texto dado tamanho da senha
void pergsalva(char *); //pergunta e salva

int main() {
	char senha[256], modo = '\0';
	printf("Senha: ");
	pegastr(senha);
	printf("\n\n");
	learq(senha);
	printf("Deseja escrever? (S/N) \n->");
	modo = pegachar();		
	if (modo=='s'|| modo=='S') {system("cls"); printf("-> "); escreve(senha);}
	printf("\n\n");
	return 0;
}



char pegachar() {
	char a;
	do {
		a = getchar();
		fflush(stdin); // limpa buffer
	} while (a == '\n' || a == '\0');
	
	return (a);
}

int pegastr(char *string){
	scanf("%[^\n]", string); // pega string até chegar o \n
	fflush(stdin); // limpa buffer
	return(strlen(string));
}

char* decodeFile(FILE *file, char*senha){
	int i=0, poscharsen=0, fileSize, valid;
	char c;
	char* decoded;
	
	fseek(file, 0L, SEEK_END);
	fileSize = ftell(file) + 1;
	rewind(file);
	decoded = (char *) malloc(sizeof(char)*fileSize + 1);
	int tamanhosenha = strlen(senha);

	for (i=0; i<fileSize; i++){
		poscharsen = i%tamanhosenha;
		valid = fscanf(file, "%c", &c);
		if(valid){
			decoded[i] = (somachar(c,senha[poscharsen],'+'));
		}
		else{
			printf("Erro de leitura\n");
			rewind(file);
			return -1;
		}
	}
	decoded[i]='\0';
	rewind(file);
	return decoded;
}

int findPage(char *decoded, int start, int flag){
	int cursor = 0, prevCursor = 0; 
	decoded += start;
	
	if (flag==NEXT_PAGE) {
		while(decoded[cursor]=='\n' || decoded[cursor] == ' ') cursor++;
	}
	else {
		while(decoded[cursor]=='\n' || decoded[cursor] == ' ') cursor--;
		if(decoded[cursor]=='\0') cursor--; //para entrar no loop
	}	
	while(decoded[cursor]!='\0'){
		if (flag == NEXT_PAGE) {
			cursor++; if (decoded[cursor] == '\0') break;
			if (decoded[cursor] == '\n'){
				cursor++; if (decoded[cursor] == '\0') break;
				while(decoded[cursor] == ' ') cursor++; if (decoded[cursor] == '\0') break;
				if (decoded[cursor] == '\n'){
					cursor++; if (decoded[cursor] == '\0') break;
					while(decoded[cursor] == ' ') cursor++; if (decoded[cursor] == '\0') break;
					if (decoded[cursor] == '\n'){
						cursor++; if (decoded[cursor] == '\0') break;
						while(decoded[cursor] == ' ') cursor++; if (decoded[cursor] == '\0') break;
						if (decoded[cursor] == '\n'){
							break;
						}
					}
				}
			}
			if (decoded[cursor] == '\0') break;
		}
		else{ 		//find previous
			cursor--; if(cursor+start<=0) break;
			prevCursor = cursor;
			while(decoded[cursor] == ' ') cursor--; if(cursor+start<=0) break;
			if (decoded[cursor] == '\n'){
				cursor--; if(cursor+start<=0) break;
				while(decoded[cursor] == ' ') cursor--; if(cursor+start<=0) break;
				if (decoded[cursor] == '\n'){
					cursor--; if(cursor+start<=0) break;
					while(decoded[cursor] == ' ') cursor--; if(cursor+start<=0) break;
					if (decoded[cursor] == '\n'){
						cursor--; if(cursor+start<=0) break;
						while(decoded[cursor] == ' ') cursor--; if(cursor+start<=0) break;
						if (decoded[cursor] == '\n'){
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

void printPage(char *decoded, int cursorBegining, int cursorEnd){
	char header[] = "[D]->proxima, [A]->anterior, [E]->ultima, [W]->escrever no diario";
	int startPos = 0, endPos = 0;
	startPos = cursorBegining; 
	while(decoded[startPos] == ' ' || decoded[startPos] == '\n') startPos++;
	endPos = cursorEnd; 
	while(decoded[endPos] == ' ' || decoded[endPos] == '\n' || decoded[endPos] == '\0') endPos--;
	system("cls");
	printf("%s\n\n%.*s\n", header, endPos - startPos + 1, decoded+startPos);
}

void learq(char *senha)
{
	int tamanhosenha = strlen(senha);
	int decodedLength = 0;
	char c = '\0';
	
	FILE *arq = NULL;
	arq = fopen(NOMEARQ, "r");
	if (arq == NULL){
		printf("arquivo inexistente\n");
		return;
	}	
	
	
	char *decoded = decodeFile(arq, senha); //lembrar de dar free em decoded
	fclose(arq);
	decodedLength = strlen(decoded);
	int cursorBegining = 0;
	int cursorEnd = findPage(decoded,0, NEXT_PAGE);
	
	
	printPage(decoded, cursorBegining, cursorEnd);
	
	while(c != 'w' && c != 'W'){
		c = _getch();
		if(c == 'd' || c == 'D'){ //proxima pagina
			if(cursorEnd < decodedLength){ 
				cursorBegining += findPage(decoded,cursorBegining, NEXT_PAGE);
				cursorEnd += findPage(decoded,cursorEnd, NEXT_PAGE);
				printPage(decoded, cursorBegining, cursorEnd);
			}
		}
		else if(c == 'a' || c == 'A'){ //pagina anterior
			if(cursorBegining > 0){
				cursorBegining += findPage(decoded,cursorBegining, PREVIOUS_PAGE);
				cursorEnd += findPage(decoded,cursorEnd, PREVIOUS_PAGE);
				printPage(decoded, cursorBegining, cursorEnd);
			}	
		}
		else if(c == 'e' || c == 'E'){ //ultima pagina
			while(cursorEnd < decodedLength){
				cursorBegining += findPage(decoded,cursorBegining, NEXT_PAGE);
				cursorEnd += findPage(decoded,cursorEnd, NEXT_PAGE);
			}
			printPage(decoded, cursorBegining, cursorEnd);
		}
	}
	system("cls");
	free(decoded);
}


int formata(char *texto,int pos){

	int i=0;
	
	while ((LINHAS-i)>0){
		texto[pos+i]='\n' ;
		i++;
	}

return(pos+i);
}


int ajusta(char *texto, int pos, int fix){

	fix--; //chute
	while (fix>0){
		texto[pos++]=' ';
		fix--;
	}
	
	return(pos);
}


void escreve(char *senha)
{
	char texto[65536];
	int posicao=0, fix=0;

	// pega texto
	// combina texto e senha
	// salva
	posicao = formata(texto, posicao);//formata

	posicao = calcdata(texto,posicao); //adiciona data ao inicio da string

	posicao = formata(texto, posicao);//formata

	int tamanhoInput = 0;

	do{
		texto[posicao] = '\n';
		posicao++;
		tamanhoInput = posicao;
		pegastr(&texto[posicao]);	// pega texto 
		posicao = strlen(texto); // calcula o tamanho da string pega acima, scanf de merda
		tamanhoInput = posicao - tamanhoInput;
	}while(tamanhoInput>0);
	
	posicao = formata(texto, posicao);

	fix = combina(texto,senha,'-',0);	// combina texto e senha

	posicao = ajusta(texto,posicao,fix);	//ajusta senha

	pergsalva(texto);	// pergunta e salva (ou nao ne)

}


char somachar(char a, char b, char op){
	int resultado = a;
	if (op=='+'){
			if(a>='a' && a<='z'){ //se a for uma letra minuscula
				resultado=a+(b-'a'); // soma à senha
				if (resultado>'z') resultado-='z'-'a'+1; // retira o total de letras caso exceda
		}
			else if (a>='A' && a<='Z'){ //se a for uma letra maiuscula
				resultado=a+(b-'a');
				if (resultado>'Z') resultado-='z'-'a'+1;
		}
}
else if (op=='-'){
		if(a>='a' && a<='z'){ //se a for uma letra minuscula
			resultado=a-(b-'a');
			if (resultado<'a') resultado+='z'-'a'+1;
	}
		else if (a>='A' && a<='Z'){ //se a for uma letra maiuscula
			resultado=a-(b-'a');
			if (resultado<'A') resultado+='z'-'a'+1;
	}
}

	return (resultado);
}

void copiastr(char *destino, char *origem){ /*Minha versão do strcpy*/

	int i=0;
	while(origem[i]!='\0'){
		destino[i]=origem[i];
		i++;
		}
	destino[i++]='\0';
}


void numtex(int numero, char *texto){

	int copia=numero, tamanho=0; //cópia
	
	while(copia){ //calcula grandeza do numero
	copia=copia/10;
	tamanho++;
	}
	
	texto[tamanho]='\0'; //define final da string
	
	while(tamanho){
		texto[tamanho-1]=(numero%10+'0');
		numero/=10;
		tamanho--;
		}

}

int calcdata(char* data, int pos){

	time_t agora;					//variavel de tempo
	time(&agora);					//funcao nada a ver
  //ctime(&agora));					//funcao que retorna uma string com a data ->  ctime(&agora)
	
	
	struct tm *minhahr = localtime(&agora);
//	printf("ano:%d mes:%d dia%d hora:%d\n\n", (minhahr->tm_year)+1900,minhahr->tm_mon,minhahr->tm_mday,minhahr->tm_hour);

	int dia  = minhahr->tm_mday;
	int mes  = minhahr->tm_mon;
	int ano  = minhahr->tm_year;
	int hora = minhahr->tm_hour;
	int min  = minhahr->tm_min;

	//dia

	numtex(dia, &data[pos]); 
	if(dia <10) {			 // se for menor que 10 adiciona um caracter '0' antes da string
		data[pos+1]=data[pos]; 
		data[pos++]='0';
		pos++;
		} 
	else {
		pos+=2;
		}

	data[pos++]='/'; //espaco entre mes

	//mes
	numtex(mes+1, &data[pos]);
	if(mes+1 <10) {
		data[pos+1]=data[pos];
		data[pos++]='0';
		pos++;
		} 
	else {
		pos+=2;
		}

	data[pos++]='/'; //espaco entre mes
	//ano
	numtex(ano+1900, &data[pos]); pos+=4;//copia ano para data

	
	data[pos++]=' '; //adiciona espaco entre a data e a hora

	//hora
	if (hora==0) data[pos]='0'; // horas, minutos podem ser igais a zero
	else numtex(hora, &data[pos]); 
	if(hora <10) {
		data[pos+1]=data[pos];
		data[pos++]='0';
		pos++;
		} 
	else {
		pos+=2;
		}

	data[pos++]=':'; //simbolo entre hora e minuto


	//min
	if (min==0) data[pos]='0'; // horas, minutos podem ser igais a zero
	else numtex(min, &data[pos]); 
	if(min <10) {
		data[pos+1]=data[pos];
		data[pos++]='0';
		pos++;
		} 
	else {
		pos+=2;
		}


// horas, minutos podem ser igais a zero

return(pos); //posicao final
}

int combina(char *texto, char *senha,char operacao, int posicao){

	int tamanhosenha=strlen(senha), poscharsen;



	do{
			poscharsen = posicao%tamanhosenha;
			texto[posicao]= somachar(texto[posicao],senha[poscharsen],operacao); //calcula o caractere
		    posicao++;
	}while(texto[posicao]!='\0');


return(tamanhosenha - poscharsen); //retorna quanto falta para completar senha

}


void salva(char *texto) {

	FILE *arq;
	arq = fopen(NOMEARQ, "a");
	int posicao = 0;

	do{
		fwrite(&texto[posicao],sizeof(char),1,arq);				  // escreve no arquivo
	}while(texto[++posicao]!='\0');


	fclose(arq);

}


void pergsalva(char *texto) {

	char modo = '\0';

	while (modo == '\0')
	{
		printf("\nsalvar?(S/N) ");
		modo = pegachar();
		printf("\n");
		if (modo == 'S'||modo == 's'){
			printf("\nSalvando...\n");
			salva(texto);
			}
		else if (!(modo == 'n' || modo== 'N')) modo = '\0';
	}
}










