// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ArvAVL.h"

// ===============================================================================
// FUNCIONALIDADES
// ===============================================================================

// Imprime uma soma de bytes do arquivo binário (checksum/100.0) para comparação/debug.
void binarioNaTela(char *nomeArquivoBinario);

// Lê uma string da entrada padrão, tratando o termo NULO e strings entre aspas.
void scan_quote_string(char *str);

// Funções de utilidade para o projeto

// Wrapper para insereNo da AVL.
void atualizaAVL(ARV* arv, int id, long int bOffset);

// Percorre a AVL em ordem crescente (inorder) e escreve os índices no arquivo binário.
void printCrescIndice(NO* raiz, FILE* indice_bin_file);

// Versão de strtok que não pula delimitadores consecutivos (para parsing de CSV com campos vazios).
char *novo_strtok(char *str, const char *delim, char **saveptr);

// Remove caracteres de espaço, quebra de linha ou tabulação do final de uma string.
// @return: Uma nova string alocada (deve ser liberada).
char *trim(char *str);


#endif