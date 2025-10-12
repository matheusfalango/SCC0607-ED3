// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ArvAVL.h"

void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);

void atualizaAVL(ARV* arv, int id, long int bOffset);
void printCrescIndice(NO* raiz, FILE* indice_bin_file);

char *novo_strtok(char *str, const char *delim, char **saveptr);
char *trim(char *str);


#endif
