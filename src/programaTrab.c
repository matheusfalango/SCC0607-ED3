// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#include "programaTrab.h"
#include "utilidades.h"
#include "ArvAVL.h"


// Programa principal

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

    // Inicializar cabeçalho do arquivo pessoa.bin - registro de dados
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

    // Criar árvore AVL para o índice
    ARV* arvoreIndice = criarAVL();

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
        token = novo_strtok(rest, ",", &rest);
        record.idPessoa = (token != NULL) ? atoi(token) : -1;
        
        // nomePessoa
        char nomePessoa_buffer[256];
        token = novo_strtok(rest, ",", &rest);
        if (token != NULL && strcmp(token, "") != 0) {
            strcpy(nomePessoa_buffer, token);
            record.nomePessoa = trim(nomePessoa_buffer);
            record.tamanhoNomePessoa = strlen(record.nomePessoa);
        } else {
            record.nomePessoa = NULL;
            record.tamanhoNomePessoa = 0;
        }

        // idadePessoa
        token = novo_strtok(rest, ",", &rest);
        record.idadePessoa = (token != NULL) ? atoi(token) : -1;

        // nomeUsuario
        char nomeUsuario_buffer[256];
        token = novo_strtok(rest, ",", &rest);
        if (token != NULL && strcmp(token, "") != 0) {
            strcpy(nomeUsuario_buffer, token);
            record.nomeUsuario = trim(nomeUsuario_buffer);
            record.tamanhoNomeUsuario = strlen(record.nomeUsuario);
        } else {
            record.nomeUsuario = NULL;
            record.tamanhoNomeUsuario = 0;
        }
        // Ao colocar nulo no ponteiro da string, nao aloca no disco a string
        // logo somente é guardado o tamanho da string

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
        if (record.nomePessoa != NULL && strcmp(record.nomePessoa, "") != 0) fwrite(record.nomePessoa, sizeof(char), record.tamanhoNomePessoa, pessoa_bin_file);
        fwrite(&record.tamanhoNomeUsuario, sizeof(int), 1, pessoa_bin_file);
        if (record.nomeUsuario != NULL && strcmp(record.nomeUsuario, "") != 0) fwrite(record.nomeUsuario, sizeof(char), record.tamanhoNomeUsuario, pessoa_bin_file);

        // Atualizar cabeçalho do arquivo pessoa.bin
        pessoa_header.quantidadePessoas++;
        // pessoa_header.proxByteOffset += record.tamanhoRegistro; // Isso será feito no final

        // Inserir na arvore AVL para o índice
        IndexRecord index_record;
        index_record.idPessoa = record.idPessoa;
        index_record.byteOffset = current_byte_offset;
        atualizaAVL(arvoreIndice, index_record.idPessoa, index_record.byteOffset);

        if (record.nomePessoa != NULL) free(record.nomePessoa);
        if (record.nomeUsuario != NULL) free(record.nomeUsuario);
    }

    // Escrever a árvore AVL no arquivo de índice
    printCrescIndice(arvoreIndice->raiz, indice_bin_file);

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

    liberarAVL(arvoreIndice->raiz);
    free(arvoreIndice);
}


// Implementação da Funcionalidade 3: Busca por idPessoa (com índice)
void listarRegistros(char *arquivoSaidaBin, char *arquivoIndicePrimarioBin) {
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

    long long atual_byte_offset = -1;
    // Pular os 11 bytes de lixo do cabeçalho do índice
    fseek(indice_bin_file, INDEX_HEADER_SIZE, SEEK_SET);

    IndexRecord current_index_record;
    while (fread(&current_index_record.idPessoa, sizeof(int), 1, indice_bin_file) == 1) {
        fread(&current_index_record.byteOffset, sizeof(long long), 1, indice_bin_file);
        
        atual_byte_offset = current_index_record.byteOffset;
        if(atual_byte_offset != -1) {
            // buscar no arquivo de dados
            fseek(pessoa_bin_file, atual_byte_offset, SEEK_SET);

            PessoaRecord record;
            fread(&record.removido, sizeof(char), 1, pessoa_bin_file);

            if (record.removido == NAO_REMOVIDO_CHAR ) {
                fread(&record.tamanhoRegistro, sizeof(int), 1, pessoa_bin_file);
                fread(&record.idPessoa, sizeof(int), 1, pessoa_bin_file);
                fread(&record.idadePessoa, sizeof(int), 1, pessoa_bin_file);
                
                fread(&record.tamanhoNomePessoa, sizeof(int), 1, pessoa_bin_file);
                if (record.tamanhoNomePessoa > 0) {
                    record.nomePessoa = (char *) malloc(record.tamanhoNomePessoa);
                    fread(record.nomePessoa, sizeof(char), record.tamanhoNomePessoa, pessoa_bin_file);
                }   // Leitura somente se existir o nome da pessoa

                fread(&record.tamanhoNomeUsuario, sizeof(int), 1, pessoa_bin_file);
                if (record.tamanhoNomeUsuario > 0) {
                    record.nomeUsuario = (char *) malloc(record.tamanhoNomeUsuario);
                    fread(record.nomeUsuario, sizeof(char), record.tamanhoNomeUsuario, pessoa_bin_file);
                }   // Leitura somente se existir o nome de usuario
                
                // Print na tela
                printf("Dados da pessoa de codigo %d\n", record.idPessoa);
                if (record.tamanhoNomePessoa > 0) {
                    printf("Nome: %s\n", record.nomePessoa);
                } else {
                    printf("Nome: -\n");
                }
                if (record.idadePessoa != -1) {
                    printf("Idade: %d\n", record.idadePessoa);
                } else {
                    printf("Idade: -\n");
                }
                if (record.tamanhoNomeUsuario > 0) {
                    printf("Usuario: %s\n", record.nomeUsuario);
                } else {
                    printf("Usuario: -\n");
                }
            }

            free(record.nomePessoa);
            free(record.nomeUsuario);
        }
    }

    fclose(pessoa_bin_file);
    fclose(indice_bin_file);
}


/*Implementação 4: Busca de uma quantidade de campos 
seguindo o campo referência dado*/ 
void buscarRegistros(char *arquivoSaidaBin, char *arquivoIndicePrimarioBin, int qtdBusca) {
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

    while (qtdBusca > 0) {

        // implementação para ler campo e valor do campo
        // depois disso, faz a busca no arquivo de dados
        // id ou idade ou nome ou usuario

        qtdBusca--;
    }
}


// Programa main com argumentos lidos após chamada de execução
int main() {

    int funcionalidade;

    // ler a linha dos argumentos pós make run ./trabalhoPrat
    char line[1024];
    fgets(line, sizeof(line), stdin);

    // remove '\n' se existir
    line[strcspn(line, "\n")] = '\0';

    char *argv[4] = {NULL}; // maximo de 3 argumentos + 1 funcionalidade
    char *rest = strtok(line, " ");
    int index = 0;

    while (rest != NULL && index < 4) {
        argv[index] = rest;
        index++;
        rest = strtok(NULL, " ");
    }

    if (argv[0] ==  NULL) {
        printf("Funcionalidade inválida!\n");
        return EXIT_FAILURE;
    }

    funcionalidade = atoi(argv[0]);
    switch (funcionalidade) {
        case 1:
        criarArquivoIndicePrimario(argv[1]);
        break;

        case 2:
        processarCSV(argv[1], argv[2], argv[3]);
        break;

        case 3:
        listarRegistros(argv[1], argv[2]);
        break;

        case 4:
        buscarRegistros(argv[1], argv[2], atoi(argv[3]));
        break;

        default:
        printf("Funcionalidade inválida");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}