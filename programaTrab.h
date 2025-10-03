#ifndef PROGRAMA_TRAB_H
#define PROGRAMA_TRAB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilidades.h"

// Definições para o arquivo pessoa.bin
#define PESSOA_HEADER_SIZE 17
#define REMOVIDO_CHAR '1'
#define NAO_REMOVIDO_CHAR '0'
#define LIXO_CHAR '$'

typedef struct {
    char status; // '0' inconsistente, '1' consistente
    int quantidadePessoas;
    int quantidadeRemovidos;
    long long proxByteOffset;
} PessoaHeader;

typedef struct {
    char removido; // '0' não removido, '1' removido
    int tamanhoRegistro;
    int idPessoa;
    int tamanhoNomePessoa;
    char *nomePessoa;
    int idadePessoa;
    int tamanhoNomeUsuario;
    char *nomeUsuario;
} PessoaRecord;

// Definições para o arquivo indexaPessoa.bin
#define INDEX_HEADER_SIZE 12

typedef struct {
    char status; // '0' inconsistente, '1' consistente
    char lixo[11]; // 11 bytes de lixo
} IndexHeader;

typedef struct {
    int idPessoa;
    long long byteOffset;
} IndexRecord;

// Protótipos das funções
void binarioNaTela(char *nomeArquivoBinario);
void trim(char *str);

// Funcionalidade 1: Criação de Arquivo de Índice Primário
void criarArquivoIndicePrimario(char *nomeArquivoIndice);

// Funcionalidade 2: Leitura de CSV e Geração de Arquivo de Dados e Índice
void processarCSV(char *arquivoEntradaCSV, char *arquivoSaidaBin, char *arquivoIndicePrimarioBin);

// Funcionalidade 3: Busca por idPessoa (com índice)
void buscarPessoaComIndice(char *arquivoSaidaBin, char *arquivoIndicePrimarioBin, int idPessoa);

// Funcionalidade 4: Busca por idPessoa (sem índice)
void buscarPessoaSemIndice(char *arquivoSaidaBin, int idPessoa);

// Funcionalidade 5: Busca por nomePessoa (sem índice)
void buscarPessoaPorNome(char *arquivoSaidaBin, char *nomePessoa);

#endif

