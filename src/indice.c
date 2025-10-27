// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#include "indice.h"
#include "def.h"


// ===============================================================================
// UTILIDADES PARA AS FUNCIONALIDADES - ARQUIVO INDICE
// ===============================================================================

/*
Implementação da abrirIndice: Abertura de Arquivo de Índice Primário
Abre o arquivo indice de acordo com o modo de abertura desejado.
@param nomeArquivoIndice: Nome do arquivo de índice a ser criado.
@param modo: Tipo de abertura do arquivo a ser usado.
@return: Endereço de abertura do arquivo.
*/
FILE *abrirIndice(char *nomeArquivoIndice, char *modo) {
    FILE *indice_bin_file = fopen(nomeArquivoIndice, modo);
    if (indice_bin_file == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return NULL;
    }

    return indice_bin_file;
}


/*
Implementação de initCabecIndice: Inicializar o Cabeçalho do Índice Primário
Atualiza os valores iniciais para o arquivo de índice primário
@param indice_bin_file: O endereço do arquivo do índice.
@param index_header: A estrutura do cabeçalho do arquivo do índice.
*/
void initCabecIndice(FILE *indice_bin_file, IndexHeader *index_header) {
    index_header->status = '0'; // Status '0': Inconsistente (em escrita)

    fseek(indice_bin_file, 0 ,SEEK_SET);
    fwrite(&index_header->status, sizeof(char), 1, indice_bin_file);
    
    // Preenche os 11 bytes restantes do cabeçalho com lixo '$'
    for (int i = 0; i < 11; i++) {
        fputc(LIXO_CHAR, indice_bin_file);
    }

    index_header->status = '1'; // Status '1': Consistente (escrita finalizada)
    fseek(indice_bin_file, 0, SEEK_SET);
    fwrite(&index_header->status, sizeof(char), 1, indice_bin_file);
}


/*
Implementação de statusIndice: Atualiza o Status do Índice Primário
Atualiza o status do arquivo de índice primário
@param indice_bin_file: O endereço do arquivo do índice.
@param index_header: A estrutura do cabeçalho do arquivo do índice.
@param status: O char atual para definir o status atual do arquivo do índice.
*/
void statusIndice(FILE *indice_bin_file, IndexHeader *index_header, char status) {
    fseek(indice_bin_file, 0, SEEK_SET);
    fread(&index_header->status, sizeof(char), 1, indice_bin_file); // Lê o status atual
    index_header->status = status;
    fseek(indice_bin_file, 0, SEEK_SET);
    fwrite(&index_header->status, sizeof(char), 1, indice_bin_file);
}