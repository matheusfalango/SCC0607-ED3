// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#ifndef SEGUE_H
#define SEGUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "constantes.h"
#include "lista.h"

// ===============================================================================
// DEFINIÇÕES - ARQUIVO SEGUE
// ===============================================================================

#define SEGUE_HEADER_SIZE 9 // Tamanho fixo do cabeçalho de segue (status + quantidadePessoas + proxRRN)

// Estrutura do cabeçalho do arquivo segue (segue.bin)
typedef struct {
    char status; // '0' (inconsistente) ou '1' (consistente)
    int quantidadePessoas; // Número total de pessoas que seguem outras pessoas presentes no arquivo
    int proxRRN; // Armazena o valor do proximo byte offset disponivel
} SegueHeader;

#define SEGUE_RECORD_SIZE 32 // Tamanho fixo do registro de segue (campos fixos)

// Estrutura do registro de dados do segue (segue.bin)
typedef struct {
    char removido; // '0' não removido, '1' removido
    int idPessoaQueSegue; // identifica o idPessoa que esta seguindo
    int idPessoaQueESeguida; // identifica o idPessoa que é seguida por outra pessoa
    char dataInicioQueSegue[11]; // data de inicio que o id segue começou a seguir a id seguida
    char dataFimQueSegue[11]; // data de fim que o id segue terminou de seguir a id seguida
    char grauAmizade; // segue uma pessoa porque é; '0' celebridade, '1' amiga da minha amiga, '2' amiga
} SegueRecord;


// ===============================================================================
// UTILIDADES PARA AS FUNCIONALIDADES - ARQUIVO SEGUE
// ===============================================================================

//Implementação da abrirSegue: Abertura de Arquivo de Segue
//Abre o arquivo segue de acordo com o modo de abertura desejado.
FILE *abrirSegue(char *nomeArquivoSegue, char *modo);

//Implementação de initCabecSegue: Inicializar o Cabeçalho do Segue
//Atualiza os valores iniciais para o arquivo segue
void initCabecSegue(FILE *segue_bin_file, SegueHeader *segue_header);

//Implementação de statusSegue: Atualiza o Status do Segue
//Atualiza o status do arquivo de Segue
void statusSegue(FILE *segue_bin_file, SegueHeader *segue_header, char status);

//Implementação de escreverSegue: Escreve no Arquivo um Registro do Arquivo Segue.
//Insere um registro no arquivo segue.
void escreveSegueRecord(FILE *segue_bin_file, SegueRecord *record);

//Implementação de atualizaCabecSegue: Atualiza o Cabeçalho do Segue
//Atualiza o cabeçalho do arquivo de segue.
void atualizaCabecSegue(FILE *segue_bin_file, SegueHeader *segue_header);

//Implementação de atualizaCabecPessoa: Atualiza o Cabeçalho do Pessoa
//Atualiza o cabeçalho do arquivo de pessoa.
void lerCabecSegue(FILE *segue_bin_file, SegueHeader *segue_header);

//Implementação de copiaCabecSegue: Copia o Cabeçalho do Segue Desordenado para o Ordenado
//Escreve o cabeçalho do arquivo segue ordenado com os dados do arquivo segue desordenado
void copiaCabecSegue(FILE *segue_bin_file, SegueHeader *ordenado_header, SegueHeader *desordenado_header);

//Implementação de lerSegueEmVetor: Lê um registro do arquivo segue para a memória
//Leitura de um registro do arquivo segue para a memória em vetor
void lerSegueEmVetor(FILE *segue_bin_file, SegueRecord *segue_record, int qtdPessoas);

//Implementação de escreveVetorEmSegue: Escreve um vetor de registros no arquivo segue
//Escrita de um vetor de registros no arquivo segue
void escreveVetorEmSegue(FILE *segue_bin_file, SegueRecord *segue_record, int qtdPessoas);

//Implementação de buscarPessoaEmSegue: Busca registros de Pessoa relacionados no arquivo Segue em memória
//Buscar registros de Segue, em memoria no vetor, relacionados no arquivo Pessoa, em memória na lista,
//e imprime os dados da pessoa e do segue relacionado ao idPessoa
void buscarPessoaEmSegue(Lista *lista_registros, SegueRecord *segue_record, int qtdPessoasSegue);

//Implementação de printNaTelaSegue: Impressão da Estrutura Segue no Terminal
//Escreve os dados de um registro de segue no formato exigido no terminal.
void printNaTelaSegue(SegueRecord segue_record);

//Implementação de lerRegistroSegue: Lê um registro do arquivo segue
//Leitura de um registro do arquivo segue para a memória
void lerRegistroSegue(FILE *segue_bin_file, SegueRecord *segue_record);

//Implementação de buscaBinariaIdPessoaQueSegue: Busca Binária por idPessoaQueSegue no Arquivo Segue em memória no vetor ordenado
//Busca binária por idPessoaQueSegue no arquivo segue ordenado em memoria no vetor
//e retorna a quantidade de registros encontrados com o idPessoaQueSegue igual ao idPessoaProcurado
int buscaBinariaIdPessoaQueSegue(SegueRecord *segue_header, int qtdPessoasSegue, int idPessoaProcurado, int *primeiraPosicao);

//Implementação de verificaStatusSegue: Verifica o Status de Consistência do Arquivo
//Verifica o status do arquivo de segue.
bool verificaStatusSegue(FILE *segue_bin_file, SegueHeader *segue_header);

#endif