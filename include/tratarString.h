// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#ifndef TRATAR_STRING_H
#define TRATAR_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ===============================================================================
// FUNCIONALIDADES PARA TRATAMENTO DE STRINGS
// ===============================================================================

// Versão do scan_quote_string 
// @return: Nova string alocada após tratamento dos caracteres indesejados e aspas
void scan_string_aspas(char *valor_final, char *valor);

// Versão de strtok que não pula delimitadores consecutivos (para parsing de CSV com campos vazios).
// @return: Uma nova string alocada que foi lida antes do delimitador
char *novo_strtok(char *str, const char *delim, char **saveptr);

// Remove caracteres de espaço, quebra de linha ou tabulação do final de uma string.
// @return: Uma nova string alocada (deve ser liberada).
char *trim(char *str);


#endif