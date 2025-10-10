// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

/*
Implementação das funções implementadas no trabalho para facilitar
alguns processos, a exemplo da manipulação de string
*/

#include "utilidades.h"
#include "ArvAVL.h"


// Funções disponibilizadas para o projeto
/*
Função para imprimir dados salvos no arquivo em binário
(util para comparar saida no run codes)
*/
void binarioNaTela(char *nomeArquivoBinario) { 

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

void scan_quote_string(char *str) {
	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '"') {
		int i = 0;
		while((R = getchar()) != EOF && R != '"') {
			str[i++] = R;
		}
		str[i] = '\0';
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}


// Funções desenvolvidas para o projeto
void atualizaAVL(ARV* arv, int id, long long bOffset) {
		insereNo(arv, id, bOffset);
}


void printCrescIndice(NO* raiz, FILE* indice_bin_file) {
		if (raiz != NULL) {
				printCrescIndice(raiz->esq, indice_bin_file);
				fwrite(&raiz->id, sizeof(int), 1, indice_bin_file);
				fwrite(&raiz->bOffset, sizeof(long long), 1, indice_bin_file);
				printCrescIndice(raiz->dir, indice_bin_file);
		}
}


char *novo_strtok(char *str, const char *delim, char **saveptr) {
    char *token;

		if (str != NULL) {
			*saveptr = str;
		}

		// Se chegar no fim da string, retorna null
    if (*saveptr == NULL || **saveptr == '\0') {
        return NULL;
    }

		// Não pular delimitadores consecutivos para preservar campos vazios
		token = *saveptr;
		*saveptr = strpbrk(token, delim);
		
		if (*saveptr != NULL) {
				**saveptr = '\0';
				(*saveptr)++;
		}

		return token;
}


// Função auxiliar para remover caracteres especiais do final de uma string
char *trim(char *str) {
    long int n = strlen(str); // n é o tamanho da str
    while (n > 0 && (str[n-1] == ' ' || str[n-1] == '\n' || str[n-1] == '\r')) {
				n--;
    }

		//Novo espaço para alocar a nova string sem os caracteres indesejados
		// Alocar n+1 bytes para incluir o terminador nulo
		char *new_str = (char*) malloc((n + 1) * sizeof(char));
		if (new_str == NULL) return NULL;

		strncpy(new_str, str, n);
		new_str[n] = '\0';

		return new_str;
}