// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#ifndef INDICE_H
#define INDICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ArvAVL.h"

// ===============================================================================
// DEFINIÇÕES - ARQUIVO INDICE
// ===============================================================================

// Definições para o arquivo indexaPessoa.bin - registro de indíces
#define INDEX_HEADER_SIZE 12 // Tamanho fixo do cabeçalho de índice (char + 11 lixo = 1 + 11)

// Estrutura do cabeçalho do arquivo de índice (indexaPessoa.bin)
typedef struct {
    char status; // '0' (inconsistente) ou '1' (consistente)
    char lixo[11]; // 11 bytes de lixo para completar o cabeçalho
} IndexHeader;

// Estrutura do registro de índice (indexaPessoa.bin)
typedef struct {
    int idPessoa; // ID da pessoa (chave)
    long int byteOffset; // Byte offset do registro correspondente no arquivo de dados
} IndexRecord;


// ===============================================================================
// UTILIDADES PARA AS FUNCIONALIDADES - ARQUIVO INDICE
// ===============================================================================

//Implementação da abrirIndice: Abertura de Arquivo de Índice Primário
//Abre o arquivo indice de acordo com o modo de abertura desejado.
FILE *abrirIndice(char *nomeArquivoIndice, char *modo);

//Implementação de initCabecIndice: Inicializar o Cabeçalho do Índice Primário
//Atualiza os valores iniciais para o arquivo de índice primário
void initCabecIndice(FILE *indice_bin_file, IndexHeader *index_header);

//Implementação de statusIndice: Atualiza o Status do Índice Primário
//Atualiza o status do arquivo de índice primário
void statusIndice(FILE *indice_bin_file, IndexHeader *index_header, char status);

//Implementação de escreveIndiceArvore: Carrega o arquivo de índice na arvore AVL em memória
//Lê o arquivo de índice binário e carrega os registros na árvore AVL em memória.
void escreveIndiceArvore(FILE *indice_bin_file, ARV *arvoreIndice);

//Implementação de verificaStatusIndice: Verifica o Status de Consistência do Arquivo
//Verifica o status do arquivo de indice.
bool verificaStatusIndice(FILE *indice_bin_file, IndexHeader *indice_header);

#endif