// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

/*
Implementação das funções utilitárias implementadas no trabalho para facilitar
alguns processos, a exemplo da manipulação de string e debug.
*/

#include "programaTrab.h"
#include "utilidades.h"
#include "ArvAVL.h"


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

/*
Função complementar para printar registros na tela no formato exigido.
@param record: Estrutura PessoaRecord contendo os dados do registro a ser impresso.
*/
void printNaTela(PessoaRecord record) {
		printf("Dados da pessoa de codigo %d\n", record.idPessoa);
		
		// Imprime nomePessoa ou '-' se for nulo
		if (record.tamanhoNomePessoa > 0) {
				printf("Nome: %s\n", record.nomePessoa);
		} else {
				printf("Nome: -\n");
		}
		
		// Imprime idadePessoa ou '-' se for -1
		if (record.idadePessoa != -1) {
				printf("Idade: %d\n", record.idadePessoa);
		} else {
				printf("Idade: -\n");
		}
		
		// Imprime nomeUsuario ou '-' se for nulo
		if (record.tamanhoNomeUsuario > 0) {
				printf("Usuario: %s\n", record.nomeUsuario);
		} else {
				printf("Usuario: -\n");
		}
}


/*
Função de wrapper para inserir um nó na Árvore AVL.
@param arv: Ponteiro para a Árvore AVL.
@param id: ID do registro a ser inserido (chave primária).
@param bOffset: Byte Offset do registro no arquivo de dados.
*/
void atualizaAVL(ARV* arv, int id, long int bOffset) {
		insereNo(arv, id, bOffset);
}


/*
Função para percorrer a Árvore AVL em ordem crescente (inorder) e 
escrever os índices no arquivo binário.
@param raiz: Ponteiro para o nó raiz da subárvore atual.
@param indice_bin_file: Ponteiro para o arquivo binário de índice.
*/
void printCrescIndice(NO* raiz, FILE* indice_bin_file) {
		if (raiz == NULL) return;

		// Percorre os nós da esquerda
		printCrescIndice(raiz->esq, indice_bin_file);

		// Escreve o nó atual (idPessoa e byteOffset)
		fwrite(&raiz->id, sizeof(int), 1, indice_bin_file);
		fwrite(&raiz->bOffset, sizeof(long int), 1, indice_bin_file);

		// Percorre os nós da direita
		printCrescIndice(raiz->dir, indice_bin_file);
		
}


/*
Implementação de strtok que não pula delimitadores consecutivos, preservando
campos vazios (necessário para o parsing do CSV).
@param str: String a ser tokenizada. Passar a string na primeira chamada e NULL nas subsequentes.
@param delim: String contendo os delimitadores.
@param saveptr: Ponteiro para o ponteiro que armazena o estado interno da função.
@return: Ponteiro para o próximo token, ou NULL se não houver mais tokens.
*/
char *novo_strtok(char *str, const char *delim, char **saveptr) {
    char *token; // Ponteiro para o token encontrado

		// Se str não for NULL, inicia a tokenização
		if (str != NULL) {
			*saveptr = str;
		}

		// Se o ponteiro de estado (saveptr) for NULL ou apontar para o terminador nulo, retorna NULL
    if (*saveptr == NULL || **saveptr == '\0') {
        return NULL;
    }

		// O token começa na posição atual de *saveptr
		token = *saveptr;
		
		// Encontra a próxima ocorrência de um delimitador
		*saveptr = strpbrk(token, delim);
		
		if (*saveptr != NULL) {
				**saveptr = '\0'; // Substitui o delimitador por '\0' (terminador)
				(*saveptr)++; // Avança para o caractere após o delimitador
		}

		return token; // Retorna o token (substring até o delimitador ou fim)
}


/*
Função auxiliar para remover caracteres especiais (espaço, \n, \r, \t) do 
final de uma string e alocar uma nova string.
@param str: String a ser limpa.
@return: Ponteiro para a nova string alocada (deve ser liberada) ou NULL em caso de falha.
*/
char *trim(char *str) {

		int n = strlen(str); // n é o tamanho da str
		
		// Itera do fim para o começo, removendo caracteres indesejados
    while (n > 0 && (str[n-1] == ' ' || str[n-1] == '\n' || str[n-1] == '\r' || str[n-1] == '\t')) {
				n--;
    }

		// Novo espaço para alocar a nova string sem os caracteres indesejados
		// Aloca n+1 bytes para incluir o terminador nulo
		char *new_str = (char*) malloc((n+1) * sizeof(char));
		if (new_str == NULL) return NULL;

		// Copia a parte "limpa" da string original
		strncpy(new_str, str, n);
		new_str[n] = '\0'; // Adiciona terminador nulo

		return new_str; // Retorna a nova string limpa
}

