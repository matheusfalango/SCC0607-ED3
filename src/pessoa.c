// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#include "pessoa.h"


// ===============================================================================
// UTILIDADES PARA AS FUNCIONALIDADES - ARQUIVO PESSOA
// ===============================================================================

/*
//Implementação da abrirPessoa: Abertura de Arquivo de Pessoa
//Abre o arquivo pessoa de acordo com o modo de abertura desejado.
@param arquivoSaidaBin: Nome do arquivo de pessoa a ser criado.
@param modo: Tipo de abertura do arquivo a ser usado.
@return: Endereço de abertura do arquivo.
*/
FILE *abrirPessoa(char *arquivoSaidaBin, char *modo) {
    FILE *pessoa_bin_file = fopen(arquivoSaidaBin, modo);
    if (pessoa_bin_file == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return NULL;
    }

    return pessoa_bin_file;
}


/*
Função complementar para printar registros na tela no formato exigido.
@param pessoa_record: Estrutura PessoaRecord contendo os dados do registro a ser impresso.
*/
void printNaTela(PessoaRecord pessoa_record) {
		printf("Dados da pessoa de codigo %d\n", pessoa_record.idPessoa);
		
		// Imprime nomePessoa ou '-' se for nulo
		if (pessoa_record.tamanhoNomePessoa > 0) {
				printf("Nome: %s\n", pessoa_record.nomePessoa);
		} else {
				printf("Nome: -\n");
		}
		
		// Imprime idadePessoa ou '-' se for -1
		if (pessoa_record.idadePessoa != -1) {
				printf("Idade: %d\n", pessoa_record.idadePessoa);
		} else {
				printf("Idade: -\n");
		}
		
		// Imprime nomeUsuario ou '-' se for nulo
		if (pessoa_record.tamanhoNomeUsuario > 0) {
				printf("Usuario: %s\n", pessoa_record.nomeUsuario);
		} else {
				printf("Usuario: -\n");
		}
}


/*
//Implementação de initCabecPessoa: Inicializar o Cabeçalho do Pessoa
//Atualiza os valores iniciais para o arquivo de pessoas
@param pessoa_header: A estrutura do cabeçalho do arquivo do pessoa.
*/
void initCabecPessoa(FILE *pessoa_bin_file, PessoaHeader *pessoa_header) {
    // Variáveis de inicialização
    pessoa_header->status = '0'; 
    pessoa_header->quantidadePessoas = 0;
    pessoa_header->quantidadeRemovidos = 0;
    pessoa_header->proxByteOffset = PESSOA_HEADER_SIZE; 

    // Escreve cabeçalho inicial no arquivo de dados
    fwrite(&pessoa_header->status, sizeof(char), 1, pessoa_bin_file);
    fwrite(&pessoa_header->quantidadePessoas, sizeof(int), 1, pessoa_bin_file);
    fwrite(&pessoa_header->quantidadeRemovidos, sizeof(int), 1, pessoa_bin_file);
    fwrite(&pessoa_header->proxByteOffset, sizeof(long int), 1, pessoa_bin_file);
}


/*
Implementação de atualizaCabecPessoa: Atualiza o Cabeçalho do Pessoa
Atualiza o cabeçalho do arquivo de pessoa.
@param pessoa_bin_file: O endereço do arquivo do pessoa.
@param pessoa_header: A estrutura do cabeçalho do arquivo do pessoa.
*/
void atualizaCabecPessoa(FILE *pessoa_bin_file, PessoaHeader *pessoa_header) {
    fseek(pessoa_bin_file, 0, SEEK_END);
    pessoa_header->proxByteOffset = ftell(pessoa_bin_file); // Tamanho total do arquivo
    fseek(pessoa_bin_file, 0, SEEK_SET);
    pessoa_header->status = '1'; // Consistente
    fwrite(&pessoa_header->status, sizeof(char), 1, pessoa_bin_file);
    fwrite(&pessoa_header->quantidadePessoas, sizeof(int), 1, pessoa_bin_file);
    fwrite(&pessoa_header->quantidadeRemovidos, sizeof(int), 1, pessoa_bin_file);
    fwrite(&pessoa_header->proxByteOffset, sizeof(long int), 1, pessoa_bin_file);
}


/*
Implementação de atualizaCabecPessoa: Atualiza o Cabeçalho do Pessoa
Atualiza o cabeçalho do arquivo de pessoa.
@param pessoa_bin_file: O endereço do arquivo do pessoa.
@param pessoa_header: A estrutura do cabeçalho do arquivo do pessoa.
*/
void lerCabecPessoa(FILE *pessoa_bin_file, PessoaHeader *pessoa_header) {
    fseek(pessoa_bin_file, 0, SEEK_SET);
    fread(&pessoa_header->status, sizeof(char), 1, pessoa_bin_file);
    fread(&pessoa_header->quantidadePessoas, sizeof(int), 1, pessoa_bin_file);
    fread(&pessoa_header->quantidadeRemovidos, sizeof(int), 1, pessoa_bin_file);
    fread(&pessoa_header->proxByteOffset, sizeof(long int), 1, pessoa_bin_file);
}


/*
Implementação de statusPessoa: Atualiza o Status do Arquivo Pessoa
Atualiza o status do arquivo de pessoa
@param pessoa_bin_file: O endereço do arquivo do pessoa.
@param pessoa_header: A estrutura do cabeçalho do arquivo do pessoa.
@param status: O char atual para definir o status atual do arquivo do pessoa.
*/
void statusPessoa(FILE *pessoa_bin_file, PessoaHeader *pessoa_header, char status) {
    fseek(pessoa_bin_file, 0, SEEK_SET);
    fread(&pessoa_header->status, sizeof(char), 1, pessoa_bin_file); // Lê o status atual
    pessoa_header->status = status;
    fseek(pessoa_bin_file, 0, SEEK_SET);
    fwrite(&pessoa_header->status, sizeof(char), 1, pessoa_bin_file);
}

/*
Implementação de escrevePessoaRecord: Escreve um Registro de Pessoa no Arquivo
Escreve um registro de pessoa no arquivo binário.
@param file: Ponteiro do arquivo de dados.
@param record: Ponteiro do registro de dados a ser escrito.
*/
void escrevePessoaRecord(FILE *file, PessoaRecord *record) {
    fwrite(&record->removido, sizeof(char), 1, file);
    fwrite(&record->tamanhoRegistro, sizeof(int), 1, file);
    fwrite(&record->idPessoa, sizeof(int), 1, file);
    fwrite(&record->idadePessoa, sizeof(int), 1, file);
    fwrite(&record->tamanhoNomePessoa, sizeof(int), 1, file);
    if(record->tamanhoNomePessoa > 0) fwrite(record->nomePessoa, record->tamanhoNomePessoa, 1, file);
    fwrite(&record->tamanhoNomeUsuario, sizeof(int), 1, file);
    if(record->tamanhoNomeUsuario > 0) fwrite(record->nomeUsuario, record->tamanhoNomeUsuario, 1, file);
}


/*
Implementação de copiarPessoaRecord: Copia um Registro de Pessoa
Copia um registro de pessoa de uma estrutura para outra.
@param origem: Ponteiro do registro de dados de origem.
@param destino: Ponteiro do registro de dados de destino.
*/
void copiarPessoaRecord(PessoaRecord *destino, PessoaRecord *origem) {
    destino->removido = origem->removido;
    destino->tamanhoRegistro = origem->tamanhoRegistro;
    destino->idPessoa = origem->idPessoa;
    destino->idadePessoa = origem->idadePessoa;

    destino->tamanhoNomePessoa = origem->tamanhoNomePessoa;
    if (origem->tamanhoNomePessoa > 0) {
        destino->nomePessoa = (char *) malloc(origem->tamanhoNomePessoa + 1);
        strcpy(destino->nomePessoa, origem->nomePessoa);
    } else {
        destino->nomePessoa = NULL;
    }

    destino->tamanhoNomeUsuario = origem->tamanhoNomeUsuario;
    if (origem->tamanhoNomeUsuario > 0) {
        destino->nomeUsuario = (char *) malloc(origem->tamanhoNomeUsuario + 1);
        strcpy(destino->nomeUsuario, origem->nomeUsuario);
    } else {
        destino->nomeUsuario = NULL;
    }
}


/*
Implementação de verificaStatusPessoa: Verifica o Status de Consistência do Arquivo
Verifica o status do arquivo de pessoa.
@param pessoa_bin_file: O endereço do arquivo do pessoa.
@param pessoa_header: A estrutura do cabeçalho do arquivo do pessoa.
@return: Booleano da consistência do arquivo pessoa.
*/
bool verificaStatusPessoa(FILE *pessoa_bin_file, PessoaHeader *pessoa_header) {
    fseek(pessoa_bin_file, 0, SEEK_SET);
    fread(&pessoa_header->status, sizeof(char), 1, pessoa_bin_file);
    if (pessoa_header->status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(pessoa_bin_file);
        return 0;
    }

    return 1;
}


/*
Implementação de lerRegistroPessoa: Lê um Registro de Pessoa do Arquivo
Lê um registro de pessoa do arquivo binário.
@param: Ponteiro do arquivo de dados.
@return: Endereço do registro de dados lido.
*/
PessoaRecord *lerRegistroPessoa(FILE *pessoa_bin_file) {
    long int atual_byte_offset = ftell(pessoa_bin_file);

    PessoaRecord *pessoa_record = (PessoaRecord *) malloc(sizeof(PessoaRecord));
    if (pessoa_record == NULL) return NULL;
    pessoa_record->nomePessoa = NULL;
    pessoa_record->nomeUsuario = NULL;
    
    // Tenta ler o campo 'removido'
    if(fread(&pessoa_record->removido, sizeof(char), 1, pessoa_bin_file) != 1) return NULL;

    if (pessoa_record->removido == NAO_REMOVIDO_CHAR ) {
        // Registro não removido: lê e processa
        fread(&pessoa_record->tamanhoRegistro, sizeof(int), 1, pessoa_bin_file);
        fread(&pessoa_record->idPessoa, sizeof(int), 1, pessoa_bin_file);
        fread(&pessoa_record->idadePessoa, sizeof(int), 1, pessoa_bin_file);
        
        // Leitura de nomePessoa (tamanho + dado)
        fread(&pessoa_record->tamanhoNomePessoa, sizeof(int), 1, pessoa_bin_file);
        if (pessoa_record->tamanhoNomePessoa > 0) {
            pessoa_record->nomePessoa = (char *) malloc(pessoa_record->tamanhoNomePessoa + 1);
            fread(pessoa_record->nomePessoa, sizeof(char), pessoa_record->tamanhoNomePessoa, pessoa_bin_file);
            pessoa_record->nomePessoa[pessoa_record->tamanhoNomePessoa] = '\0';
        }

        // Leitura de nomeUsuario (tamanho + dado)
        fread(&pessoa_record->tamanhoNomeUsuario, sizeof(int), 1, pessoa_bin_file);
        if (pessoa_record->tamanhoNomeUsuario > 0) {
            pessoa_record->nomeUsuario = (char *) malloc(pessoa_record->tamanhoNomeUsuario + 1);
            fread(pessoa_record->nomeUsuario, sizeof(char), pessoa_record->tamanhoNomeUsuario, pessoa_bin_file);
            pessoa_record->nomeUsuario[pessoa_record->tamanhoNomeUsuario] = '\0';
        }
    } else {
        // Registro removido, pula
        int tamanhoRegistro;
        fread(&tamanhoRegistro, sizeof(int), 1, pessoa_bin_file);
        // Avança o ponteiro de arquivo
        fseek(pessoa_bin_file, atual_byte_offset + tamanhoRegistro + sizeof(char) + sizeof(int), SEEK_SET);
    }

    return pessoa_record;
}


/*
Implementação de filtroCampoPessoa: Filtra um Registro de Pessoa com Base em um Campo e Valor
Filtra um registro de pessoa com base em um campo específico e um valor fornecido.
@param pessoa_record:
@param campo:
@param valor_final:
@return:
*/
int filtroCampoPessoa(PessoaRecord *pessoa_record, char *campo, char *valor_final) {
    // Compara o campo
    int match = 0;
    if (strcmp(campo, "idadePessoa") == 0) {
        int idadeProcurada = (strcmp(valor_final, "NULO") != 0) ? atoi(valor_final) : -1;
        if (pessoa_record->idadePessoa == idadeProcurada) {
            match = 1;
        }
    } else if (strcmp(campo, "nomePessoa") == 0) {
        if (pessoa_record->nomePessoa != NULL && strcmp(pessoa_record->nomePessoa, valor_final) == 0) {
            match = 1;
        } else if (pessoa_record->tamanhoNomePessoa == 0 && strcmp(valor_final, "NULO") == 0) {
            match = 1;
        }
    } else if (strcmp(campo, "nomeUsuario") == 0) {
        if (pessoa_record->nomeUsuario != NULL && strcmp(pessoa_record->nomeUsuario, valor_final) == 0) {
            match = 1;
        } else if (pessoa_record->tamanhoNomeUsuario == 0 && strcmp(valor_final, "NULO") == 0) {
            match = 1;
        }
    }

    return match;
}

