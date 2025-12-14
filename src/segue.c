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
Implementação de atualizaCabecSegue: Atualiza o Cabeçalho do Segue
Atualiza o cabeçalho do arquivo de segue.
@param segue_bin_file: O endereço do arquivo do segue.
@param segue_header: A estrutura do cabeçalho do arquivo do segue.
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
Implementação de buscarPessoaEmSegue: Busca registros de Pessoa relacionados no arquivo Segue
Buscar registros de Segue relacionados no arquivo Pessoa e imprime os dados da pessoa e do segue relacionado ao idPessoa
@param segue_bin_file: Arquivo de registros Segue.
@param lista_registros: Ponteiro para a lista de registros encontrados.
@param segue_record: Ponteiro para o registro em memória do arquivo segue.
@param qtdPessoasSegue: Quantidade de registros do arquivo segue.
*/
void buscarPessoaEmSegue(Lista *lista_registros, SegueRecord *segue_record, int qtdPessoasSegue) {
    if(lista_registros == NULL || segue_record == NULL) return;

    No* atual = lista_registros->inicio;
    if(atual == NULL) return;
    
    while(atual != NULL) {
        PessoaRecord *atualRecord = atual->record;
        if(atualRecord == NULL) return;

        int idPessoa = atualRecord->idPessoa; // define a variável idPessoa do registro atual em análise
        int primeiroRegistro;
        int qtdRegistrosEncontrados = buscaBinariaIdPessoaQueSegue(segue_record, qtdPessoasSegue, idPessoa, &primeiroRegistro);

        if (qtdRegistrosEncontrados > 0) {
            printNaTelaPessoa(*atualRecord); // printa na tela o registro pessoa
            printf("\n");
            for(int i = primeiroRegistro; i < primeiroRegistro + qtdRegistrosEncontrados - 1; i++) {
                printNaTelaSegue(segue_record[i]); // printa na tela o registro que pessoa segue
            }
        printf("\n");
        }

        atual = atual->prox;
    }
}


/*
Implementação de printNaTelaSegue: Impressão da Estrutura Segue no Terminal
Escreve os dados de um registro de segue no formato exigido no terminal.
@param record: Estrutura SegueRecord contendo os dados do registro a ser impresso.
*/
void printNaTelaSegue(SegueRecord segue_record) {
    // Imprime o id da pessoa que segue
    printf("Segue a pessoa de codigo: %d\n", segue_record.idPessoaQueESeguida);
		
    // Imprime o motivo de seguir uma pessoa ou '-' se for nulo
    if (segue_record.grauAmizade == '0') {
        printf("Justificativa para seguir: celebridade\n");
    } else if (segue_record.grauAmizade == '1') {
        printf("Justificativa para seguir: amiga de minha amiga\n");
    } else if (segue_record.grauAmizade == '2') {
        printf("Justificativa para seguir: minha amiga\n");
    } else {
        printf("Justificativa para seguir: -\n");
    }
    
    // Imprime a data que começou a seguir a pessoa
    if(segue_record.dataInicioQueSegue[0] != '$' && segue_record.dataInicioQueSegue[0] != '\0') {
        printf("Começou a seguir em: %s\n", segue_record.dataInicioQueSegue);
    } else {
        printf("Começou a seguir em: -\n");
    }

    // Imprime a data que parou de seguir a pessoa
    if(segue_record.dataFimQueSegue[0] != '$' && segue_record.dataFimQueSegue[0] != '\0') {
        printf("Parou de seguir em: %s\n", segue_record.dataFimQueSegue);
    } else {
        printf("Parou de seguir em: -\n");
    }

    printf("\n");

}


/*
Implementação de lerRegistroSegue: Lê um registro do arquivo segue após o campo removido
Leitura de um registro do arquivo segue para a memória
@param segue_bin_file: Arquivo de registros Segue.
@param segue_record: Ponteiro para o registro em memória do arquivo segue.
*/
void lerRegistroSegue(FILE *segue_bin_file, SegueRecord *segue_record) {
    
    fread(&segue_record->idPessoaQueSegue, sizeof(int), 1, segue_bin_file);
    fread(&segue_record->idPessoaQueESeguida, sizeof(int), 1, segue_bin_file);
    fread(segue_record->dataInicioQueSegue, sizeof(char), 10, segue_bin_file);
    segue_record->dataInicioQueSegue[10] = '\0';
    fread(segue_record->dataFimQueSegue, sizeof(char), 10, segue_bin_file);
    segue_record->dataFimQueSegue[10] = '\0';
    fread(&segue_record->grauAmizade, sizeof(char), 1, segue_bin_file);
}


/*Implementação de buscaBinariaIdPessoaQueSegue: Busca Binária por idPessoaQueSegue no Arquivo Segue
Busca binária por idPessoaQueSegue no arquivo segue ordenado 
e retorna a quantidade de registros encontrados com o idPessoaQueSegue igual ao idPessoaProcurado
@param segue_record: A estrutura do registro do arquivo do segue.
@param qtdPessoasSegue: A quantidade de registros do arquivo do segue.
@param idPessoaProcurado: O idPessoa que está sendo procurado.
@param primeiraPosicao: Ponteiro para armazenar a posição do primeiro registro encontrado.
@return: Inteiro indicando a quantidade de registros encontrados com o idPessoaQueSegue igual ao idPessoaProcurado.
*/
int buscaBinariaIdPessoaQueSegue(SegueRecord *segue_record, int qtdPessoasSegue, int idPessoaProcurado, int *primeiraPosicao) {
    int esquerda = 0; // primeiro indice de registro no vetor
    int direita = qtdPessoasSegue - 1; // ultimo indice registro no vetor
    int encontrado = -1; // armazena o indice da primeira aparição do id buscado

    //busca binaria para encontrar as ocorrências do idPessoaProcurado
    while(esquerda <= direita) {
        int meio = (esquerda + direita)/2;

        if(segue_record[meio].idPessoaQueSegue == idPessoaProcurado) {
            encontrado = meio;
            break;
        } 
        else if(segue_record[meio].idPessoaQueSegue < idPessoaProcurado) esquerda = meio + 1;
        else if(segue_record[meio].idPessoaQueSegue > idPessoaProcurado) direita = meio - 1;
    }

    if(encontrado == -1) {
        // nao encontrou nenhum registro com o id procurado
        *primeiraPosicao = -1;
        return 0;
    }

    //encontra indice do primeiro e ultimo registro com o id procurado
    int primeiroIndice = encontrado;
    while(primeiroIndice > 0 && segue_record[primeiroIndice-1].idPessoaQueSegue == segue_record[encontrado].idPessoaQueSegue) {
        primeiroIndice--;
    }

    int ultimoIndice = encontrado;
    while(segue_record[ultimoIndice].idPessoaQueSegue == segue_record[encontrado].idPessoaQueSegue) {
        ultimoIndice++;
    }

    *primeiraPosicao = primeiroIndice;
    return (ultimoIndice - primeiroIndice + 1);
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

