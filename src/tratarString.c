// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#include "tratarString.h"

// ===============================================================================
// FUNCIONALIDADES PARA TRATAMENTO DE STRING
// ===============================================================================

/*
Implementação de um scan que lê uma string entre aspas, além de tratar os
caracteres indesejados após a leiura.
@param valor: String a ser tratada.
@return: Ponteiro da nova string alocada.
*/
void scan_string_aspas(char *valor_final, char* valor) {
	// Limpa o valor (remove aspas e espaços extras)

	if (valor[0] == '"' && valor[strlen(valor)-1] == '"') {
            strncpy(valor_final, valor + 1, strlen(valor) - 2);
            valor_final[strlen(valor) - 2] = '\0';
        } else {
            strcpy(valor_final, valor);
        }
        char *p_inicio = valor_final;
        while (*p_inicio == ' ' || *p_inicio == '\t') p_inicio++;
        char *p_fim = valor_final + strlen(valor_final) - 1;
        while (p_fim > p_inicio && (*p_fim == ' ' || *p_fim == '\t')) {
            *p_fim = '\0';
            p_fim--;
        }
        if (p_inicio != valor_final) {
            memmove(valor_final, p_inicio, strlen(p_inicio) + 1);
        }
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

	char *inicio = str;
    while (*inicio == ' ' || *inicio == '\n' || *inicio == '\r' || *inicio == '\t') {
        inicio++;
    }
    
    int n = strlen(inicio); // n é o tamanho da str
    // Itera do fim para o começo, removendo caracteres indesejados
    while (n > 0 && (inicio[n-1] == ' ' || inicio[n-1] == '\n' || inicio[n-1] == '\r' || inicio[n-1] == '\t')) {
        n--;
    }
    // Novo espaço para alocar a nova string sem os caracteres indesejados
    // Aloca n+1 bytes para incluir o terminador nulo
    char *new_str = (char*) malloc((n+1) * sizeof(char));
    if (new_str == NULL) return NULL;
    // Copia a parte "limpa" da string original
    strncpy(new_str, inicio, n);
    new_str[n] = '\0'; // Adiciona terminador nulo
    return new_str; // Retorna a nova string limpa
}

