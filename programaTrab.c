#include "programaTrab.h"
#include "utilidades.h"
#include <string.h>

// Função auxiliar para remover espaços em branco do final de uma string
void trim(char *str) {
    int n = strlen(str);
    while (n > 0 && (str[n-1] == ' ' || str[n-1] == '\n' || str[n-1] == '\r')) {
        n--;
    }
    str[n] = '\0';
}

// Implementação da Funcionalidade 1: Criação de Arquivo de Índice Primário
void criarArquivoIndicePrimario(char *nomeArquivoIndice) {
    FILE *f_indice = fopen(nomeArquivoIndice, "wb");
    if (f_indice == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    IndexHeader header;
    header.status = '0'; // Inconsistente durante a escrita

    fwrite(&header.status, sizeof(char), 1, f_indice);
    // Preencher os 11 bytes restantes com lixo '$'
    for (int i = 0; i < 11; i++) {
        fputc(LIXO_CHAR, f_indice);
    }

    header.status = '1'; // Consistente após a escrita
    fseek(f_indice, 0, SEEK_SET);
    fwrite(&header.status, sizeof(char), 1, f_indice);

    fclose(f_indice);
    binarioNaTela(nomeArquivoIndice);
}

// Implementação da Funcionalidade 2: Leitura de CSV e Geração de Arquivo de Dados e Índice
void processarCSV(char *arquivoEntradaCSV, char *arquivoSaidaBin, char *arquivoIndicePrimarioBin) {
    FILE *csv_file = fopen(arquivoEntradaCSV, "r");
    if (csv_file == NULL) {
        printf("Falha no processamento do arquivo: Não foi possível abrir o CSV de entrada.\n");
        return;
    }

    FILE *pessoa_bin_file = fopen(arquivoSaidaBin, "wb");
    if (pessoa_bin_file == NULL) {
        printf("Falha no processamento do arquivo: Não foi possível criar o arquivo binário de pessoas.\n");
        fclose(csv_file);
        return;
    }

    FILE *indice_bin_file = fopen(arquivoIndicePrimarioBin, "r+b"); // r+b para ler e escrever
    if (indice_bin_file == NULL) {
        printf("Falha no processamento do arquivo: Não foi possível abrir/modificar o arquivo de índice primário.\n");
        fclose(csv_file);
        fclose(pessoa_bin_file);
        return;
    }

    // Inicializar cabeçalho do arquivo pessoa.bin
    PessoaHeader pessoa_header;
    pessoa_header.status = '0'; // Inconsistente
    pessoa_header.quantidadePessoas = 0;
    pessoa_header.quantidadeRemovidos = 0;
    pessoa_header.proxByteOffset = PESSOA_HEADER_SIZE; // Começa após o cabeçalho
    fwrite(&pessoa_header.status, sizeof(char), 1, pessoa_bin_file);
    fwrite(&pessoa_header.quantidadePessoas, sizeof(int), 1, pessoa_bin_file);
    fwrite(&pessoa_header.quantidadeRemovidos, sizeof(int), 1, pessoa_bin_file);
    fwrite(&pessoa_header.proxByteOffset, sizeof(long long), 1, pessoa_bin_file);

    // Atualizar status do cabeçalho do arquivo de índice para '0'
    IndexHeader index_header;
    fseek(indice_bin_file, 0, SEEK_SET);
    fread(&index_header.status, sizeof(char), 1, indice_bin_file);
    index_header.status = '0';
    fseek(indice_bin_file, 0, SEEK_SET);
    fwrite(&index_header.status, sizeof(char), 1, indice_bin_file);

    char line[1024];
    // Ignorar a primeira linha (cabeçalho do CSV)
    fgets(line, sizeof(line), csv_file);

    while (fgets(line, sizeof(line), csv_file) != NULL) {
        PessoaRecord record;
        record.removido = NAO_REMOVIDO_CHAR;
        record.nomePessoa = NULL;
        record.nomeUsuario = NULL;

        char *token;
        char *rest = line;

        // idPessoa
        token = my_strtok(rest, ",", &rest);
        record.idPessoa = (token != NULL && strcmp(token, "NULO") != 0) ? atoi(token) : -1;

        // idadePessoa
        token = my_strtok(rest, ",", &rest);
        record.idadePessoa = (token != NULL && strcmp(token, "NULO") != 0) ? atoi(token) : -1;

        // nomePessoa
        char nomePessoa_buffer[256];
        token = my_strtok(rest, ",", &rest);
        if (token != NULL && strcmp(token, "NULO") != 0) {
            strcpy(nomePessoa_buffer, token);
            trim(nomePessoa_buffer);
            record.nomePessoa = strdup(nomePessoa_buffer);
            record.tamanhoNomePessoa = strlen(record.nomePessoa);
        } else {
            record.nomePessoa = NULL;
            record.tamanhoNomePessoa = 0;
        }

        // nomeUsuario
        char nomeUsuario_buffer[256];
        token = my_strtok(rest, ",", &rest);
        if (token != NULL && strcmp(token, "NULO") != 0) {
            strcpy(nomeUsuario_buffer, token);
            trim(nomeUsuario_buffer);
            record.nomeUsuario = strdup(nomeUsuario_buffer);
            record.tamanhoNomeUsuario = strlen(record.nomeUsuario);
        } else {
            record.nomeUsuario = NULL;
            record.tamanhoNomeUsuario = 0;
        }

        // Calcular tamanho do registro
        // removido (1) + tamanhoRegistro (4) + idPessoa (4) + idadePessoa (4) + tamanhoNomePessoa (4) + tamanhoNomeUsuario (4)
        record.tamanhoRegistro = 1 + 4 + 4 + 4 + 4 + 4;
        if (record.nomePessoa != NULL) record.tamanhoRegistro += record.tamanhoNomePessoa;
        if (record.nomeUsuario != NULL) record.tamanhoRegistro += record.tamanhoNomeUsuario;

        // Escrever registro no arquivo pessoa.bin
        long long current_byte_offset = ftell(pessoa_bin_file);
        fwrite(&record.removido, sizeof(char), 1, pessoa_bin_file);
        fwrite(&record.tamanhoRegistro, sizeof(int), 1, pessoa_bin_file);
        fwrite(&record.idPessoa, sizeof(int), 1, pessoa_bin_file);
        fwrite(&record.idadePessoa, sizeof(int), 1, pessoa_bin_file);
        fwrite(&record.tamanhoNomePessoa, sizeof(int), 1, pessoa_bin_file);
        if (record.nomePessoa != NULL) fwrite(record.nomePessoa, sizeof(char), record.tamanhoNomePessoa, pessoa_bin_file);
        fwrite(&record.tamanhoNomeUsuario, sizeof(int), 1, pessoa_bin_file);
        if (record.nomeUsuario != NULL) fwrite(record.nomeUsuario, sizeof(char), record.tamanhoNomeUsuario, pessoa_bin_file);

        // Atualizar cabeçalho do arquivo pessoa.bin
        pessoa_header.quantidadePessoas++;
        // pessoa_header.proxByteOffset += record.tamanhoRegistro; // Isso será feito no final

        // Inserir no índice
        IndexRecord index_record;
        index_record.idPessoa = record.idPessoa;
        index_record.byteOffset = current_byte_offset;
        fseek(indice_bin_file, 0, SEEK_END);
        fwrite(&index_record.idPessoa, sizeof(int), 1, indice_bin_file);
        fwrite(&index_record.byteOffset, sizeof(long long), 1, indice_bin_file);

        if (record.nomePessoa != NULL) free(record.nomePessoa);
        if (record.nomeUsuario != NULL) free(record.nomeUsuario);
    }

    // Atualizar cabeçalho final do arquivo pessoa.bin
    fseek(pessoa_bin_file, 0, SEEK_SET);
    pessoa_header.status = '1'; // Consistente
    fwrite(&pessoa_header.status, sizeof(char), 1, pessoa_bin_file);
    fwrite(&pessoa_header.quantidadePessoas, sizeof(int), 1, pessoa_bin_file);
    fwrite(&pessoa_header.quantidadeRemovidos, sizeof(int), 1, pessoa_bin_file);
    pessoa_header.proxByteOffset = ftell(pessoa_bin_file); // Atualiza o proxByteOffset com o tamanho total do arquivo
    fwrite(&pessoa_header.proxByteOffset, sizeof(long long), 1, pessoa_bin_file);

    // Atualizar cabeçalho final do arquivo de índice
    fseek(indice_bin_file, 0, SEEK_SET);
    index_header.status = '1'; // Consistente
    fwrite(&index_header.status, sizeof(char), 1, indice_bin_file);

    fclose(csv_file);
    fclose(pessoa_bin_file);
    fclose(indice_bin_file);

    binarioNaTela(arquivoSaidaBin);
    binarioNaTela(arquivoIndicePrimarioBin);
}

// Implementação da Funcionalidade 3: Busca por idPessoa (com índice)
void buscarPessoaComIndice(char *arquivoSaidaBin, char *arquivoIndicePrimarioBin, int idPessoa) {
    FILE *pessoa_bin_file = fopen(arquivoSaidaBin, "rb");
    if (pessoa_bin_file == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    FILE *indice_bin_file = fopen(arquivoIndicePrimarioBin, "rb");
    if (indice_bin_file == NULL) {
        printf("Falha no processamento do arquivo.\n");
        fclose(pessoa_bin_file);
        return;
    }

    // Verificar status do cabeçalho do índice
    IndexHeader index_header;
    fread(&index_header.status, sizeof(char), 1, indice_bin_file);
    if (index_header.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(pessoa_bin_file);
        fclose(indice_bin_file);
        return;
    }

    long long found_byte_offset = -1;
    // Pular os 11 bytes de lixo do cabeçalho do índice
    fseek(indice_bin_file, INDEX_HEADER_SIZE, SEEK_SET);

    IndexRecord current_index_record;
    while (fread(&current_index_record.idPessoa, sizeof(int), 1, indice_bin_file) == 1) {
        fread(&current_index_record.byteOffset, sizeof(long long), 1, indice_bin_file);
        if (current_index_record.idPessoa == idPessoa) {
            found_byte_offset = current_index_record.byteOffset;
            break;
        }
    }

    if (found_byte_offset == -1) {
        printf("Registro inexistente.\n");
    } else {
        // Encontrou no índice, agora buscar no arquivo de dados
        fseek(pessoa_bin_file, found_byte_offset, SEEK_SET);

        PessoaRecord record;
        fread(&record.removido, sizeof(char), 1, pessoa_bin_file);
        fread(&record.tamanhoRegistro, sizeof(int), 1, pessoa_bin_file);
        fread(&record.idPessoa, sizeof(int), 1, pessoa_bin_file);
        fread(&record.idadePessoa, sizeof(int), 1, pessoa_bin_file);
        fread(&record.tamanhoNomePessoa, sizeof(int), 1, pessoa_bin_file);

        record.nomePessoa = (char *) malloc(record.tamanhoNomePessoa + 1);
        fread(record.nomePessoa, sizeof(char), record.tamanhoNomePessoa, pessoa_bin_file);
        record.nomePessoa[record.tamanhoNomePessoa] = '\0';

        fread(&record.tamanhoNomeUsuario, sizeof(int), 1, pessoa_bin_file);
        record.nomeUsuario = (char *) malloc(record.tamanhoNomeUsuario + 1);
        fread(record.nomeUsuario, sizeof(char), record.tamanhoNomeUsuario, pessoa_bin_file);
        record.nomeUsuario[record.tamanhoNomeUsuario] = '\0';

        if (record.removido == NAO_REMOVIDO_CHAR) {
            printf("Registro encontrado!\n");
            printf("ID: %d\n", record.idPessoa);
            if (record.idadePessoa != -1) {
                printf("Idade: %d\n", record.idadePessoa);
            } else {
                printf("Idade: -\n");
            }
            if (record.tamanhoNomePessoa > 0) {
                printf("Nome: %s\n", record.nomePessoa);
            } else {
                printf("Nome: -\n");
            }
            if (record.tamanhoNomeUsuario > 0) {
                printf("Usuario: %s\n", record.nomeUsuario);
            } else {
                printf("Usuario: -\n");
            }
        } else {
            printf("Registro inexistente.\n"); // Logicamente removido
        }

        free(record.nomePessoa);
        free(record.nomeUsuario);
    }

    fclose(pessoa_bin_file);
    fclose(indice_bin_file);
}

// Implementação da Funcionalidade 4: Busca por idPessoa (sem índice)
void buscarPessoaSemIndice(char *arquivoSaidaBin, int idPessoa) {
    FILE *pessoa_bin_file = fopen(arquivoSaidaBin, "rb");
    if (pessoa_bin_file == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    PessoaHeader pessoa_header;
    fread(&pessoa_header.status, sizeof(char), 1, pessoa_bin_file);
    if (pessoa_header.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(pessoa_bin_file);
        return;
    }
    fread(&pessoa_header.quantidadePessoas, sizeof(int), 1, pessoa_bin_file);
    fread(&pessoa_header.quantidadeRemovidos, sizeof(int), 1, pessoa_bin_file);
    fread(&pessoa_header.proxByteOffset, sizeof(long long), 1, pessoa_bin_file);

    // Pular o cabeçalho para começar a ler os registros de dados
    fseek(pessoa_bin_file, PESSOA_HEADER_SIZE, SEEK_SET);

    int found = 0;
    while (ftell(pessoa_bin_file) < pessoa_header.proxByteOffset) {
        PessoaRecord record;
        record.nomePessoa = NULL;
        record.nomeUsuario = NULL;

        fread(&record.removido, sizeof(char), 1, pessoa_bin_file);
        fread(&record.tamanhoRegistro, sizeof(int), 1, pessoa_bin_file);
        fread(&record.idPessoa, sizeof(int), 1, pessoa_bin_file);
        fread(&record.idadePessoa, sizeof(int), 1, pessoa_bin_file);
        fread(&record.tamanhoNomePessoa, sizeof(int), 1, pessoa_bin_file);

        record.nomePessoa = (char *) malloc(record.tamanhoNomePessoa + 1);
        fread(record.nomePessoa, sizeof(char), record.tamanhoNomePessoa, pessoa_bin_file);
        record.nomePessoa[record.tamanhoNomePessoa] = '\0';

        fread(&record.tamanhoNomeUsuario, sizeof(int), 1, pessoa_bin_file);
        record.nomeUsuario = (char *) malloc(record.tamanhoNomeUsuario + 1);
        fread(record.nomeUsuario, sizeof(char), record.tamanhoNomeUsuario, pessoa_bin_file);
        record.nomeUsuario[record.tamanhoNomeUsuario] = '\0';

        if (record.removido == NAO_REMOVIDO_CHAR && record.idPessoa == idPessoa) {
            printf("Registro encontrado!\n");
            printf("ID: %d\n", record.idPessoa);
            if (record.idadePessoa != -1) {
                printf("Idade: %d\n", record.idadePessoa);
            } else {
                printf("Idade: -\n");
            }
            if (record.tamanhoNomePessoa > 0) {
                printf("Nome: %s\n", record.nomePessoa);
            } else {
                printf("Nome: -\n");
            }
            if (record.tamanhoNomeUsuario > 0) {
                printf("Usuario: %s\n", record.nomeUsuario);
            } else {
                printf("Usuario: -\n");
            }
            found = 1;
            free(record.nomePessoa);
            free(record.nomeUsuario);
            break;
        }

        free(record.nomePessoa);
        free(record.nomeUsuario);
    }

    if (!found) {
        printf("Registro inexistente.\n");
    }

    fclose(pessoa_bin_file);
}

// Implementação da Funcionalidade 5: Busca por nomePessoa (sem índice)
void buscarPessoaPorNome(char *arquivoSaidaBin, char *nomePessoa) {
    FILE *pessoa_bin_file = fopen(arquivoSaidaBin, "rb");
    if (pessoa_bin_file == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    PessoaHeader pessoa_header;
    fread(&pessoa_header.status, sizeof(char), 1, pessoa_bin_file);
    if (pessoa_header.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(pessoa_bin_file);
        return;
    }
    fread(&pessoa_header.quantidadePessoas, sizeof(int), 1, pessoa_bin_file);
    fread(&pessoa_header.quantidadeRemovidos, sizeof(int), 1, pessoa_bin_file);
    fread(&pessoa_header.proxByteOffset, sizeof(long long), 1, pessoa_bin_file);

    // Pular o cabeçalho para começar a ler os registros de dados
    fseek(pessoa_bin_file, PESSOA_HEADER_SIZE, SEEK_SET);

    int found_any = 0;
    while (ftell(pessoa_bin_file) < pessoa_header.proxByteOffset) {
        PessoaRecord record;
        record.nomePessoa = NULL;
        record.nomeUsuario = NULL;

        fread(&record.removido, sizeof(char), 1, pessoa_bin_file);
        fread(&record.tamanhoRegistro, sizeof(int), 1, pessoa_bin_file);
        fread(&record.idPessoa, sizeof(int), 1, pessoa_bin_file);
        fread(&record.idadePessoa, sizeof(int), 1, pessoa_bin_file);
        fread(&record.tamanhoNomePessoa, sizeof(int), 1, pessoa_bin_file);

        record.nomePessoa = (char *) malloc(record.tamanhoNomePessoa + 1);
        fread(record.nomePessoa, sizeof(char), record.tamanhoNomePessoa, pessoa_bin_file);
        record.nomePessoa[record.tamanhoNomePessoa] = '\0';

        fread(&record.tamanhoNomeUsuario, sizeof(int), 1, pessoa_bin_file);
        record.nomeUsuario = (char *) malloc(record.tamanhoNomeUsuario + 1);
        fread(record.nomeUsuario, sizeof(char), record.tamanhoNomeUsuario, pessoa_bin_file);
        record.nomeUsuario[record.tamanhoNomeUsuario] = '\0';

        if (record.removido == NAO_REMOVIDO_CHAR && record.tamanhoNomePessoa > 0 && strcmp(record.nomePessoa, nomePessoa) == 0) {
            if (!found_any) {
                printf("Registro encontrado!\n");
                found_any = 1;
            }
            printf("ID: %d\n", record.idPessoa);
            if (record.idadePessoa != -1) {
                printf("Idade: %d\n", record.idadePessoa);
            } else {
                printf("Idade: -\n");
            }
            printf("Nome: %s\n", record.nomePessoa);
            if (record.tamanhoNomeUsuario > 0) {
                printf("Usuario: %s\n", record.nomeUsuario);
            } else {
                printf("Usuario: -\n");
            }
            printf("\n"); // Linha em branco entre registros
        }

        free(record.nomePessoa);
        free(record.nomeUsuario);
    }

    if (!found_any) {
        printf("Registro inexistente.\n");
    }

    fclose(pessoa_bin_file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: ./programaTrab <funcionalidade> [argumentos...]\n");
        return 1;
    }

    int funcionalidade = atoi(argv[1]);

    switch (funcionalidade) {
        case 1:
            if (argc != 3) {
                printf("Uso: ./programaTrab 1 <arquivoIndicePrimario.bin>\n");
                return 1;
            }
            criarArquivoIndicePrimario(argv[2]);
            break;
        case 2:
            if (argc != 5) {
                printf("Uso: ./programaTrab 2 <arquivoEntrada.csv> <arquivoSaida.bin> <arquivoIndicePrimario.bin>\n");
                return 1;
            }
            processarCSV(argv[2], argv[3], argv[4]);
            break;
        case 3:
            if (argc != 5) {
                printf("Uso: ./programaTrab 3 <arquivoSaida.bin> <arquivoIndicePrimario.bin> <idPessoa>\n");
                return 1;
            }
            buscarPessoaComIndice(argv[2], argv[3], atoi(argv[4]));
            break;
        case 4:
            if (argc != 4) {
                printf("Uso: ./programaTrab 4 <arquivoSaida.bin> <idPessoa>\n");
                return 1;
            }
            buscarPessoaSemIndice(argv[2], atoi(argv[3]));
            break;
        case 5:
            if (argc != 4) {
                printf("Uso: ./programaTrab 5 <arquivoSaida.bin> <nomePessoa>\n");
                return 1;
            }
            buscarPessoaPorNome(argv[2], argv[3]);
            break;
        default:
            printf("Funcionalidade inválida.\n");
            return 1;
    }

    return 0;
}

