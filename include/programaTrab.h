// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira


#ifndef PROGRAMA_TRAB_H
#define PROGRAMA_TRAB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilidades.h"
#include "ArvAVL.h"


// ===============================================================================
// DEFINIÇÕES
// ===============================================================================

// Definições para o arquivo pessoa.bin - registro de dados
#define PESSOA_HEADER_SIZE 17 // Tamanho fixo do cabeçalho de dados (char + int + int + long int = 1 + 4 + 4 + 8)
#define REMOVIDO_CHAR '1' // Caractere que indica que o registro foi removido
#define NAO_REMOVIDO_CHAR '0' // Caractere que indica que o registro NÃO foi removido
#define LIXO_CHAR '$' // Caractere de preenchimento de lixo

// Estrutura do cabeçalho do arquivo de dados (pessoa.bin)
typedef struct {
    char status; // '0' (inconsistente) ou '1' (consistente)
    int quantidadePessoas; // Número total de registros não removidos
    int quantidadeRemovidos; // Número total de registros removidos
    long int proxByteOffset; // Byte offset do próximo registro a ser inserido (ou tamanho total do arquivo)
} PessoaHeader;

// Estrutura do registro de dados (pessoa.bin)
typedef struct {
    char removido; // '0' não removido, '1' removido
    int tamanhoRegistro; // Tamanho total do registro em bytes (exclui 'removido' e 'tamanhoRegistro')
    int idPessoa; // ID da pessoa (chave primária)
    int idadePessoa; // Idade da pessoa
    int tamanhoNomePessoa; // Tamanho em bytes da string nomePessoa
    char *nomePessoa; // String nomePessoa (alocada dinamicamente)
    int tamanhoNomeUsuario; // Tamanho em bytes da string nomeUsuario
    char *nomeUsuario; // String nomeUsuario (alocada dinamicamente)
} PessoaRecord;

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
// FUNCIONALIDADES
// ===============================================================================

// Funcionalidade 1: Criação de Arquivo de Índice Primário
// Cria o arquivo de índice binário apenas com o cabeçalho inicializado.
void criarArquivoIndicePrimario(char *nomeArquivoIndice);

// Funcionalidade 2: Leitura de CSV e Geração de Arquivo de Dados e Índice
// Lê o CSV, gera o arquivo de dados binário e o arquivo de índice primário (usando AVL em memória).
void processarCSV(char *arquivoEntradaCSV, char *arquivoSaidaBin, char *arquivoIndicePrimarioBin);

// Funcionalidade 3: Listar todo o registro de dados
// Faz uma varredura sequencial no arquivo de dados e lista os registros não removidos.
void listarRegistros(char *arquivoSaidaBin);

// Funcionalidade 4: Busca por campo de referencia
// Realiza buscas indexadas (por idPessoa) ou sequenciais (outros campos) no arquivo de dados.
void buscarRegistros(char *arquivoSaidaBin, char *arquivoIndicePrimarioBin, int qtdBusca);

// Função complementar: printar registros na tela
// Imprime os dados de um registro de pessoa no formato exigido.
void printNaTela(PessoaRecord record);

#endif