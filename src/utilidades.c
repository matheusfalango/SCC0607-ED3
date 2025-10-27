// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#include "utilidades.h"

// ===============================================================================
// FUNCIONALIDADES COMPLEMENTARES ATRIBUÍDAS
// ===============================================================================

/*
Função para imprimir dados salvos no arquivo em binário no formato exigido
(util para comparar saida no run codes)
@param nomeArquivoBinario: Nome do arquivo binário a ser lido e impresso.
*/
void binarioNaTela(char *nomeArquivoBinario) { 

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	* Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs; // cs é o checksum (soma de bytes)
	unsigned char *mb; // Ponteiro para o buffer de leitura
	size_t fl; // Tamanho do arquivo
	FILE *fs; // Ponteiro para o arquivo
	
	// Tenta abrir o arquivo para leitura binária
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	
	// Determina o tamanho do arquivo
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	
	// Aloca buffer e lê o conteúdo
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	// Calcula o checksum
	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	
	// Imprime o checksum / 100.0
	printf("%lf\n", (cs / (double) 100));
	
	// Libera recursos
	free(mb);
	fclose(fs);
}

/*
Função para ler uma string de entrada do console, tratando strings entre aspas 
e o termo NULO. 
@param str: Ponteiro para a string onde o valor lido será armazenado.
*/
void scan_quote_string(char *str) {
	char R; // Caractere lido

	while((R = getchar()) != EOF && isspace(R)); // Ignora espaços em branco, \r, \n...

	if(R == 'N' || R == 'n') { // Trata o termo "NULO"
		getchar(); getchar(); getchar(); // Ignora o "ULO"
		strcpy(str, ""); // Armazena string vazia
	} else if(R == '"') { // Trata string entre aspas
		int i = 0;
		while((R = getchar()) != EOF && R != '"') {
			str[i++] = R;
		}
		str[i] = '\0'; // Adiciona terminador nulo
	} else if(R != EOF){ // Trata leitura normal (e.g., inteiro, sem aspas)
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // Trata EOF
		strcpy(str, "");
	}
}


// ===============================================================================
// FUNCIONALIDADES COMPLEMENTARES DESENVOLVIDAS
// ===============================================================================

