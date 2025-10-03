/*
Implementação das funções implementadas no trabalho para facilitar
alguns processos, a exemplo da manipulação de string
*/

#include "utilidades.h"

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


char *my_strtok(char *str, const char *delim, char **saveptr) {
    char *token;
    if (str != NULL) {
        *saveptr = str;
    }
    if (*saveptr == NULL || **saveptr == '\0') {
        return NULL;
    }

    // Pular delimitadores
    *saveptr += strcspn(*saveptr, delim);
    if (**saveptr == '\0') {
        return NULL;
    }
		if (strcmp(*saveptr, "") == 0) {
				token = "";
				*saveptr = strpbrk(token, delim);
		}

    token = *saveptr;
    *saveptr = strpbrk(token, delim);

		// Adiciona um '\0' no final das strings lidas
    /*if (*saveptr != NULL) {
        **saveptr = '\0';
        (*saveptr)++;
    }*/

    return token;
}


// Função auxiliar para remover espaços em branco do final de uma string
void trim(char *str) {
    int n = strlen(str);
    while (n > 0 && (str[n-1] == ' ' || str[n-1] == '\n' || str[n-1] == '\r' || str[n-1] == '\0')) {
        n--;
    }
    //str[n] = '\0';
}