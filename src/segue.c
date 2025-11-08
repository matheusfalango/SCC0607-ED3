// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#include "segue.h"


// ===============================================================================
// FUNCIONALIDADES - ARQUIVO SEGUE
// ===============================================================================


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
    fwrite(&segue_header->proxRRN, sizeof(int), 1, segue_bin_file);

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
    fseek(segue_bin_file, 0, SEEK_SET);
    segue_header->status = '1'; // Consistente
    segue_header->proxRRN = segue_header->quantidadePessoas; // RRN para calculo do offset disponivel
    fwrite(&segue_header->status, sizeof(char), 1, segue_bin_file);
    fwrite(&segue_header->quantidadePessoas, sizeof(int), 1, segue_bin_file);
    fwrite(&segue_header->proxRRN, sizeof(int), 1, segue_bin_file);
}


/*
Implementação de atualizaCabecPessoa: Atualiza o Cabeçalho do Pessoa
Atualiza o cabeçalho do arquivo de pessoa.
@param segue_bin_file: O endereço do arquivo do pessoa.
@param segue_header: A estrutura do cabeçalho do arquivo do pessoa.
*/
void lerCabecSegue(FILE *segue_bin_file, SegueHeader *segue_header) {
    fseek(segue_bin_file, 0, SEEK_SET);
    fread(&segue_header->status, sizeof(char), 1, segue_bin_file);
    fread(&segue_header->quantidadePessoas, sizeof(int), 1, segue_bin_file);
    fread(&segue_header->proxRRN, sizeof(int), 1, segue_bin_file);
}

/*
Implementação de copiaCabecSegue: Copia o Cabeçalho do Segue Desordenado para o Ordenado
Escreve o cabeçalho do arquivo segue ordenado com os dados do arquivo segue desordenado
@param segue_bin_file: O endereço do arquivo do segue.
@param ordenado_header: A estrutura do cabeçalho do arquivo do segue ordenado.
@param desordenado_header: A estrutura do cabeçalho do arquivo do segue desordenado
*/
void copiaCabecSegue(FILE *segue_bin_file, SegueHeader *ordenado_header, SegueHeader *desordenado_header) {
    // atualiza os valores do cabeçalho no header ordenado
    ordenado_header->status = desordenado_header->status;
    ordenado_header->quantidadePessoas = desordenado_header->quantidadePessoas;
    ordenado_header->proxRRN = desordenado_header->proxRRN;

    // escrita no arquivo no cabec ordenado
    fseek(segue_bin_file, 0, SEEK_SET);
    fwrite(&ordenado_header->status, sizeof(char), 1, segue_bin_file);
    fwrite(&ordenado_header->quantidadePessoas, sizeof(int), 1, segue_bin_file);
    fwrite(&ordenado_header->proxRRN, sizeof(int), 1, segue_bin_file);
}


/*
Implementação de lerSegueEmVetor: Lê um registro do arquivo segue para a memória
Leitura de um registro do arquivo segue para a memória em vetor
@param segue_bin_file: Arquivo de registros Segue.
@param segue_record: Ponteiro para o registro em memória do arquivo segue.
@param qtdPessoas: Quantidade de registros a serem lidos.
*/
void lerSegueEmVetor(FILE *segue_bin_file, SegueRecord *segue_record, int qtdPessoas) {
    // Pula cabeçalho
    fseek(segue_bin_file, SEGUE_HEADER_SIZE, SEEK_SET);
    // Percorre o arquivo armazenando os dados no vetor
    for(int i = 0; i < qtdPessoas; i++) {
        fread(&segue_record[i].removido, sizeof(char), 1, segue_bin_file);
        fread(&segue_record[i].idPessoaQueSegue, sizeof(int), 1, segue_bin_file);
        fread(&segue_record[i].idPessoaQueESeguida, sizeof(int), 1, segue_bin_file);
        fread(&segue_record[i].dataInicioQueSegue, sizeof(char), 10, segue_bin_file);
        fread(&segue_record[i].dataFimQueSegue, sizeof(char), 10, segue_bin_file);
        fread(&segue_record[i].grauAmizade, sizeof(char), 1, segue_bin_file);
    }
}


/*
Implementação de escreveVetorEmSegue: Escreve um vetor de registros no arquivo segue
Escrita de um vetor de registros no arquivo segue
@param segue_bin_file: Arquivo de registros Segue.
@param segue_record: Ponteiro para o registro em memória do arquivo segue.
@param qtdPessoas: Quantidade de registros a serem escritos.
*/
void escreveVetorEmSegue(FILE *segue_bin_file, SegueRecord *segue_record, int qtdPessoas) {
    // Pula cabeçalho
    fseek(segue_bin_file, SEGUE_HEADER_SIZE, SEEK_SET);
    // Escrita no arq ordenado segue
    for (int i = 0; i < qtdPessoas; i++) {
        fwrite(&segue_record[i].removido, sizeof(char), 1, segue_bin_file);
        fwrite(&segue_record[i].idPessoaQueSegue, sizeof(int), 1, segue_bin_file);
        fwrite(&segue_record[i].idPessoaQueESeguida, sizeof(int), 1, segue_bin_file);
        fwrite(&segue_record[i].dataInicioQueSegue, sizeof(char), 10, segue_bin_file);
        fwrite(&segue_record[i].dataFimQueSegue, sizeof(char), 10, segue_bin_file);
        fwrite(&segue_record[i].grauAmizade, sizeof(char), 1, segue_bin_file);
    }
}


/*
Implementação de verificaStatusSegue: Verifica o Status de Consistência do Arquivo
Verifica o status do arquivo de pessoa.
@param segue_bin_file: O endereço do arquivo do pessoa.
@param segue_header: A estrutura do cabeçalho do arquivo do pessoa.
@return: Booleano da consistência do arquivo pessoa.
*/
bool verificaStatusSegue(FILE *segue_bin_file, SegueHeader *segue_header) {
    fseek(segue_bin_file, 0, SEEK_SET);
    fread(&segue_header->status, sizeof(char), 1, segue_bin_file);
    if (segue_header->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(segue_bin_file);
        return 0;
    }

    return 1;
}

