// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#ifndef SEGUE_H
#define SEGUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAO_REMOVIDO_CHAR '0'
#define REMOVIDO_CHAR '1'

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

// Estrutura do registro de dados do segue (segue.bin)
typedef struct {
    char removido; // '0' não removido, '1' removido
    int idPessoaQueSegue; // identifica o idPessoa que esta seguindo
    int idPessoaQueESeguida; // identifica o idPessoa que é seguida por outra pessoa
    char dataInicioQueSegue[10]; // data de inicio que o id segue começou a seguir a id seguida
    char dataFimQueSegue[10]; // data de fim que o id segue terminou de seguir a id seguida
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

#endif