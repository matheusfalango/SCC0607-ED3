// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#ifndef PESSOA_H
#define PESSOA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "manipularAVL.h"

#define NAO_REMOVIDO_CHAR '0'

// ===============================================================================
// DEFINIÇÕES - ARQUIVO PESSOA
// ===============================================================================

// Definições para o arquivo pessoa.bin - registro de dados
#define PESSOA_HEADER_SIZE 17 // Tamanho fixo do cabeçalho de dados (char + int + int + long int = 1 + 4 + 4 + 8)

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


// ===============================================================================
// UTILIDADES PARA AS FUNCIONALIDADES - ARQUIVO PESSOA
// ===============================================================================

//Implementação da abrirPessoa: Abertura de Arquivo de Pessoa
//Abre o arquivo pessoa de acordo com o modo de abertura desejado.
FILE *abrirPessoa(char *arquivoSaidaBin, char *modo);

//Implementação de printNaTela: Impressão da Estrutura Pessoa no Terminal
//Imprime os dados de um registro de pessoa no formato exigido.
void printNaTela(PessoaRecord record);

//Implementação de initCabecPessoa: Inicializar o Cabeçalho do Pessoa
//Atualiza os valores iniciais para o arquivo de pessoas
void initCabecPessoa(FILE *pessoa_bin_file ,PessoaHeader* pessoa_header);

//Implementação de atualizaCabecPessoa: Atualiza o Cabeçalho do Pessoa
//Atualiza o cabeçalho do arquivo de pessoa.
void atualizaCabecPessoa(FILE *pessoa_bin_file, PessoaHeader *pessoa_header);

//Implementação de atualizaCabecPessoa: Atualiza o Cabeçalho do Pessoa
//Atualiza o cabeçalho do arquivo de pessoa.
void lerCabecPessoa(FILE *pessoa_bin_file, PessoaHeader *pessoa_header);

//Implementação de statusPessoa: Atualiza o Status do Arquivo Pessoa
//Atualiza o status do arquivo de pessoa
void statusPessoa(FILE *pessoa_bin_file, PessoaHeader *pessoa_header, char status);

//Implementação de escrevePessoaRecord: Escreve um Registro de Pessoa no Arquivo
//Escreve um registro de pessoa no arquivo binário.
void escrevePessoaRecord(FILE *file, PessoaRecord *record);

//Implementação de copiarPessoaRecord: Copia um Registro de Pessoa
//Copia um registro de pessoa de uma estrutura para outra.
void copiarPessoaRecord(PessoaRecord *destino, PessoaRecord *origem);

//Implementação de verificaStatusPessoa: Verifica o Status de Consistência do Arquivo
//Verifica o status do arquivo de pessoa.
bool verificaStatusPessoa(FILE *pessoa_bin_file, PessoaHeader *pessoa_header);

//Implementação de lerRegistroPessoa: Lê um Registro de Pessoa do Arquivo
//Lê um registro de pessoa do arquivo binário.
PessoaRecord *lerRegistroPessoa(FILE *pessoa_bin_file);

//Implementação de setProcuradoOffset: Define o Offset Atual no Arquivo de Índice
//Define o offset atual no arquivo de índice para um ID específico.
long int setProcuradoOffset(ARV *indice_em_memoria, int idProcurado);

//Implementação de filtroCampoPessoa: Filtra um Registro de Pessoa com Base em um Campo e Valor
//Filtra um registro de pessoa com base em um campo específico e um valor fornecido.
int filtroCampoPessoa(PessoaRecord *pessoa_record, char *campo, char *valor_final);


#endif