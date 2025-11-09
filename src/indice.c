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
Implementação de atualizaCabecIndice: Atualiza o Cabeçalho do Indice
Atualiza o cabeçalho do arquivo de indice.
@param indice_bin_file: O endereço do arquivo do indice.
@param sindice_header: A estrutura do cabeçalho do arquivo do indice.
*/
void lerCabecIndice(FILE *indice_bin_file, IndexHeader *indice_header) {
    fseek(indice_bin_file, 0, SEEK_SET);
    fread(&indice_header->status, sizeof(char), 1, indice_bin_file);
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


/*Implementação de escreveIndiceArvore: Carrega a Árvore AVL com o Conteúdo do Arquivo de Índice
Lê o arquivo de índice binário e carrega os registros na árvore AVL em memória.
@param indice_bin_file: Arquivo binário do índice primário.
@param arvoreIndice: Estrutura para armazenar o conteúdo lido do arquivo índice.
*/
void escreveIndiceArvore(FILE *indice_bin_file, ARV *arvoreIndice) {
    if (indice_bin_file != NULL) {
        IndexHeader index_header;
        fseek(indice_bin_file, 0, SEEK_SET);
        fread(&index_header.status, sizeof(char), 1, indice_bin_file);

        if (index_header.status == '1') { // Índice consistente
            fseek(indice_bin_file, INDEX_HEADER_SIZE, SEEK_SET);

            IndexRecord current_index_record;
            // Lê cada registro de índice e insere na AVL
            while (fread(&current_index_record.idPessoa, sizeof(int), 1, indice_bin_file) == 1) {
                fread(&current_index_record.byteOffset, sizeof(long int), 1, indice_bin_file);
                insereNoAVL(arvoreIndice, current_index_record.idPessoa, current_index_record.byteOffset);
            }
        }
    }
}


/*
Implementação de verificaStatusIndice: Verifica o Status de Consistência do Arquivo
Verifica o status do arquivo de pessoa.
@param indice_bin_file: O endereço do arquivo do pessoa.
@param indice_header: A estrutura do cabeçalho do arquivo do pessoa.
@return: Booleano da consistência do arquivo pessoa.
*/
bool verificaStatusIndice(FILE *indice_bin_file, IndexHeader *indice_header) {
    fseek(indice_bin_file, 0, SEEK_SET);
    fread(&indice_header->status, sizeof(char), 1, indice_bin_file);
    if (indice_header->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(indice_bin_file);
        return 0;
    }

    return 1;
}


/*
Implementação de setProcuradoOffset: Define o Offset Atual no Arquivo de Índice
Define o offset atual no arquivo de índice em memória para um ID específico.
@param indice_em_memoria: Registros de índice em memória.
@param idProcurado: Valor inteiro do id da pessoa procurada.
@return: Valor do byte offset referente ao id da pessoa.
*/
long int setProcuradoOffset(ARV *indice_em_memoria, int idProcurado) {
    NO* noEncontrado = buscarNo(indice_em_memoria, idProcurado);
    if (noEncontrado == NULL) return -1;
    else return noEncontrado->bOffset;
}

