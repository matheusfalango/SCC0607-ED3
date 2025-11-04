// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#include "segue.h"


/*
Implementação da abrirSegue: Abertura de Arquivo de Segue
Abre o arquivo segue de acordo com o modo de abertura desejado.
*/
FILE *abrirSegue(char *nomeArquivoSegue, char *modo) {
    FILE *segue_bin_file = fopen(nomeArquivoSegue, modo);
    if (segue_bin_file == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return NULL;
    }

    return segue_bin_file;
}


/*
Implementação de initCabecSegue: Inicializar o Cabeçalho do Segue
Atualiza os valores iniciais para o arquivo segue
*/
void initCabecSegue(FILE *segue_bin_file, SegueHeader *segue_header) {
    segue_header->status = '0'; // Status '0': Inconsistente (em escrita)
    segue_header->quantidadePessoas = 0;
    segue_header->proxRRN = 0;

    fseek(segue_bin_file, 0 ,SEEK_SET);
    fwrite(&segue_header->status, sizeof(char), 1, segue_bin_file);
    fwrite(&segue_header->quantidadePessoas, sizeof(int), 1, segue_bin_file);
    fwrite(&segue_header->proxRRN, sizeof(long int), 1, segue_bin_file);

    segue_header->status = '1'; // Status '1': Consistente (escrita finalizada)
    fseek(segue_bin_file, 0, SEEK_SET);
    fwrite(&segue_header->status, sizeof(char), 1, segue_bin_file);
}


/*
Implementação de statusSegue: Atualiza o Status do Segue
Atualiza o status do arquivo de Segue
@param segue_bin_file: O endereço do arquivo do índice.
@param segue_header: A estrutura do cabeçalho do arquivo do índice.
@param status: O char atual para definir o status atual do arquivo do índice.
*/
void statusSegue(FILE *segue_bin_file, SegueHeader *segue_header, char status) {
    fseek(segue_bin_file, 0, SEEK_SET);
    fread(&segue_header->status, sizeof(char), 1, segue_bin_file); // Lê o status atual
    segue_header->status = status;
    fseek(segue_bin_file, 0, SEEK_SET);
    fwrite(&segue_header->status, sizeof(char), 1, segue_bin_file);
}


/*Implementação de escreverSegue: Escreve no Arquivo um Registro do Arquivo Segue.
Insere um registro no arquivo segue.
@param segue_bin_file: Arquivo de registros Segue.
@param record: Ponteiro para o registro em memória do arquivo segue. 
*/
void escreveSegueRecord(FILE *segue_bin_file, SegueRecord *record) {
    record->removido = NAO_REMOVIDO_CHAR;
    fwrite(&record->removido, sizeof(char), 1, segue_bin_file);
    fwrite(&record->idPessoaQueSegue, sizeof(int), 1, segue_bin_file);
    fwrite(&record->idPessoaQueESeguida, sizeof(int), 1, segue_bin_file);
    fwrite(&record->dataInicioQueSegue, sizeof(char), 10, segue_bin_file);
    fwrite(&record->dataFimQueSegue, sizeof(char), 10, segue_bin_file);
    fwrite(&record->grauAmizade, sizeof(char), 1, segue_bin_file);
}


/*Implementação de atualizaCabecSegue: Atualiza o Cabeçalho do Segue
Atualiza o cabeçalho do arquivo de segue.
@param segue_bin_file: O endereço do arquivo do segue.
@param segue_header: A estrutura do cabeçalho do arquivo do segue.
*/
void atualizaCabecSegue(FILE *segue_bin_file, SegueHeader *segue_header) {
    fseek(segue_bin_file, 0, SEEK_END);
    segue_header->proxRRN = ftell(segue_bin_file); // Tamanho total do arquivo
    fseek(segue_bin_file, 0, SEEK_SET);
    segue_header->status = '1'; // Consistente
    fwrite(&segue_header->status, sizeof(char), 1, segue_bin_file);
    fwrite(&segue_header->quantidadePessoas, sizeof(int), 1, segue_bin_file);
    fwrite(&segue_header->proxRRN, sizeof(long int), 1, segue_bin_file);
}