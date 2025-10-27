// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utilidades.h"
#include "tratarString.h"
#include "manipularAVL.h"
#include "ArvAVL.h"
#include "def.h"


// ===============================================================================
// FUNCIONALIDADES COMPLEMENTARES - ATRIBUÍDAS
// ===============================================================================

// Imprime uma soma de bytes do arquivo binário (checksum/100.0) para comparação/debug.
void binarioNaTela(char *nomeArquivoBinario);

// Lê uma string da entrada padrão, tratando o termo NULO e strings entre aspas.
void scan_quote_string(char *str);


// ===============================================================================
// FUNCIONALIDADES COMPLEMENTARES - DESENVOLVIDAS
// ===============================================================================




#endif