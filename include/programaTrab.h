// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira


#ifndef PROGRAMA_TRAB_H
#define PROGRAMA_TRAB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilidades.h"
#include "ArvAVL.h"

// Definições para o arquivo pessoa.bin - registro de dados
#define PESSOA_HEADER_SIZE 17
#define REMOVIDO_CHAR '1'
#define NAO_REMOVIDO_CHAR '0'
#define LIXO_CHAR '$'

typedef struct {
    char status; // '0' inconsistente, '1' consistente
    int quantidadePessoas;
    int quantidadeRemovidos;
    long long int proxByteOffset;
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

// Definições para o arquivo indexaPessoa.bin - registro de indíces
#define INDEX_HEADER_SIZE 12

typedef struct {
    char status; // '0' inconsistente, '1' consistente
    char lixo[11]; // 11 bytes de lixo
} IndexHeader;

typedef struct {
    int idPessoa;
    long long int byteOffset;
} IndexRecord;

// Funcionalidade 1: Criação de Arquivo de Índice Primário
void criarArquivoIndicePrimario(char *nomeArquivoIndice);

// Funcionalidade 2: Leitura de CSV e Geração de Arquivo de Dados e Índice
void processarCSV(char *arquivoEntradaCSV, char *arquivoSaidaBin, char *arquivoIndicePrimarioBin);

// Funcionalidade 3: Listar todo o registro de dados
void listarRegistros(char *arquivoSaidaBin);

// Funcionalidade 4: Busca por campo de referencia
void buscarRegistros(char *arquivoSaidaBin, char *arquivoIndicePrimarioBin, int qtdBusca);

// Função complementar: printar registros na tela
void printNaTela(PessoaRecord record);

#endif

