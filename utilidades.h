#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);
char *novo_strtok(char *str, const char *delim, char **saveptr);
char *trim(char *str);

#endif
