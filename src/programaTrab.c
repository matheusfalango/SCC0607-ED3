// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#include "programaTrab.h"


// ===============================================================================
// FUNCIONALIDADES PRINCIPAIS
// ===============================================================================

/*
Implementação da Funcionalidade 1: Criação de Arquivo de Índice Primário
Cria um arquivo binário de índice com um cabeçalho inicializado.
@param nomeArquivoIndice: Nome do arquivo de índice a ser criado.
*/
void criarArquivoIndicePrimario(char *nomeArquivoIndice) {
    FILE *indice_bin_file = abrirIndice(nomeArquivoIndice, "wb");
    if (indice_bin_file == NULL) return;

    IndexHeader index_header;
    initCabecIndice(indice_bin_file, &index_header); // Função para inicializar cabeçalho no arquivo

    fclose(indice_bin_file);
    binarioNaTela(nomeArquivoIndice); // Função de debug
}


/*
Implementação da Funcionalidade 2: Leitura de CSV e Geração de Arquivo de Dados e Índice
Lê um arquivo CSV, gera um arquivo binário de dados e um arquivo binário de índice primário 
(utilizando uma Árvore AVL em memória).
@param arquivoEntradaCSV: Nome do arquivo CSV de entrada.
@param arquivoSaidaBin: Nome do arquivo binário de dados (pessoa.bin).
@param arquivoIndicePrimarioBin: Nome do arquivo binário de índice (indexaPessoa.bin).
*/
void processarCSV(char *arquivoEntradaCSV, char *arquivoSaidaBin, char *arquivoIndicePrimarioBin) {
    
    // Abertura dos arquivos
    FILE *csv_file = fopen(arquivoEntradaCSV, "r");
    if (csv_file == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    FILE *pessoa_bin_file = abrirPessoa(arquivoSaidaBin ,"wb"); // criar e escrever no novo arquivo
    if(pessoa_bin_file == NULL) {fclose(csv_file); return;}

    FILE *indice_bin_file = abrirIndice(arquivoIndicePrimarioBin, "r+b"); // r+b para ler e escrever
    if (indice_bin_file == NULL) {fclose(csv_file); fclose(pessoa_bin_file); return;}

    // 1. Inicializar cabeçalho do arquivo pessoa.bin (dados)
    PessoaHeader pessoa_header;
    initCabecPessoa(pessoa_bin_file, &pessoa_header);

    // 2. Atualizar status do cabeçalho do arquivo de índice para '0' (inconsistente)
    IndexHeader index_header;
    statusIndice(indice_bin_file, &index_header, '0');    

    // 3. Criar árvore AVL para o índice em memória
    ARV* arvoreIndice = criarAVL();

    char line[1024];
    // Ignorar a primeira linha (cabeçalho do CSV)
    fgets(line, sizeof(line), csv_file);

    // 4. Processamento de cada linha do CSV
    while (fgets(line, sizeof(line), csv_file) != NULL) {
        PessoaRecord record;
        record.removido = NAO_REMOVIDO_CHAR;
        record.nomePessoa = NULL;
        record.nomeUsuario = NULL;

        char *token;
        char *rest = line;

        // Extrai campos usando novo_strtok para preservar campos vazios
        
        // idPessoa (int)
        token = novo_strtok(rest, ",", &rest);
        record.idPessoa = (token != NULL && token[0] != '\0') ? atoi(token) : -1;
        
        // nomePessoa (string variável)
        char nomePessoa_buffer[256];
        token = novo_strtok(NULL, ",", &rest);
        if (token != NULL && token[0] != '\0' && strcmp(token, "") != 0) {
            strcpy(nomePessoa_buffer, token);
            record.nomePessoa = trim(nomePessoa_buffer); // Remove espaços/quebras de linha
            record.tamanhoNomePessoa = strlen(record.nomePessoa);
        } else {
            record.nomePessoa = NULL;
            record.tamanhoNomePessoa = 0;
        }

        // idadePessoa (int)
        token = novo_strtok(NULL, ",", &rest);
        record.idadePessoa = (token != NULL && token[0] != '\0') ? atoi(token) : -1;

        // nomeUsuario (string variável)
        char nomeUsuario_buffer[256];
        token = novo_strtok(NULL, ",", &rest);
        if (token != NULL && token[0] != '\0' && strcmp(token, "") != 0) {
            strcpy(nomeUsuario_buffer, token);
            record.nomeUsuario = trim(nomeUsuario_buffer); // Remove espaços/quebras de linha
            record.tamanhoNomeUsuario = strlen(record.nomeUsuario);
        } else {
            record.nomeUsuario = NULL;
            record.tamanhoNomeUsuario = 0;
        }

        // Calcular tamanho do registro (fixo + variável)
        // 4 * sizeof(int) = idPessoa + idadePessoa + tamanhoNomePessoa + tamanhoNomeUsuario
        record.tamanhoRegistro = 4*sizeof(int);
        if (record.nomePessoa != NULL && record.tamanhoNomePessoa != 0) record.tamanhoRegistro += record.tamanhoNomePessoa;
        if (record.nomeUsuario != NULL && record.tamanhoNomeUsuario != 0) record.tamanhoRegistro += record.tamanhoNomeUsuario;

        // 5. Escrever registro no arquivo pessoa.bin
        long int atual_byte_offset = ftell(pessoa_bin_file); // Byte offset de início do registro
        fseek(pessoa_bin_file, 0, SEEK_END);
        
        // Cabeçalho do registro (removido + tamanhoRegistro)
        fwrite(&record.removido, sizeof(char), 1, pessoa_bin_file);
        fwrite(&record.tamanhoRegistro, sizeof(int), 1, pessoa_bin_file);
        
        // Campos fixos
        fwrite(&record.idPessoa, sizeof(int), 1, pessoa_bin_file);
        fwrite(&record.idadePessoa, sizeof(int), 1, pessoa_bin_file);
        
        // Campos variáveis (tamanho + dado)
        fwrite(&record.tamanhoNomePessoa, sizeof(int), 1, pessoa_bin_file);
        if (record.nomePessoa != NULL && record.nomePessoa[0] != '\0') fwrite(record.nomePessoa, sizeof(char), record.tamanhoNomePessoa, pessoa_bin_file);
        
        fwrite(&record.tamanhoNomeUsuario, sizeof(int), 1, pessoa_bin_file);
        if (record.nomeUsuario != NULL && record.nomeUsuario[0] != '\0') fwrite(record.nomeUsuario, sizeof(char), record.tamanhoNomeUsuario, pessoa_bin_file);

        // 6. Atualizar contagem do cabeçalho do arquivo pessoa.bin
        pessoa_header.quantidadePessoas++;

        // 7. Inserir na arvore AVL para o índice
        IndexRecord index_record;
        index_record.idPessoa = record.idPessoa;
        index_record.byteOffset = atual_byte_offset;
        insereNoAVL(arvoreIndice, index_record.idPessoa, index_record.byteOffset);

        // 8. Liberar memória alocada
        if (record.nomePessoa != NULL) free(record.nomePessoa);
        if (record.nomeUsuario != NULL) free(record.nomeUsuario);
    }

    // 9. Escrever a árvore AVL no arquivo índice (em ordem crescente)
    fseek(indice_bin_file, INDEX_HEADER_SIZE, SEEK_SET); // Posiciona após o cabeçalho
    printCrescIndice(arvoreIndice->raiz, indice_bin_file);

    // 10. Atualizar cabeçalho final do arquivo pessoa.bin (dados)
    atualizaCabecPessoa(pessoa_bin_file, &pessoa_header);

    // 11. Atualizar cabeçalho final do arquivo de índice
    statusIndice(indice_bin_file, &index_header, '1');

    // 12. Fechar arquivos e liberar memória
    fclose(csv_file);
    fclose(pessoa_bin_file);
    fclose(indice_bin_file);

    binarioNaTela(arquivoSaidaBin);
    binarioNaTela(arquivoIndicePrimarioBin);

    liberarAVL(arvoreIndice->raiz);
    free(arvoreIndice);
}

/*
Implementação da Funcionalidade 3: Listar todos os registros
Realiza uma varredura sequencial no arquivo de dados e imprime na tela apenas
os registros NÃO REMOVIDOS.
@param arquivoSaidaBin: Nome do arquivo binário de dados (pessoa.bin).
*/
void listarRegistros(char *arquivoSaidaBin) {
    FILE *pessoa_bin_file = abrirPessoa(arquivoSaidaBin ,"rb"); // ler no novo arquivo
    if(pessoa_bin_file == NULL) return;

    // Ler cabeçalho
    PessoaHeader pessoa_header;
    lerCabecPessoa(pessoa_bin_file, &pessoa_header);

    // Verificar consistência
    if(verificaStatusPessoa(&pessoa_header) == 0) return;

    // Pula o cabeçalho
    fseek(pessoa_bin_file, PESSOA_HEADER_SIZE, SEEK_SET);

    // Busca sequencial dos registros
    Lista *lista_registros = buscaPessoa(pessoa_bin_file, NULL, NULL, NULL);
    
    //Imprimir lista de registros
    imprimirLista(lista_registros);

    //Liberar memória
    liberarLista(lista_registros);

    fclose(pessoa_bin_file);
}


/*
Implementação da Funcionalidade 4: Busca por campo de referência
Realiza 'qtdBusca' buscas no arquivo de dados, utilizando o índice primário (AVL em memória)
para buscas por 'idPessoa' e busca sequencial para outros campos.
@param arquivoSaidaBin: Nome do arquivo binário de dados (pessoa.bin).
@param arquivoIndicePrimarioBin: Nome do arquivo binário de índice (indexaPessoa.bin).
@param qtdBusca: Número de buscas a serem realizadas.
*/
void buscarRegistros(char *arquivoSaidaBin, char *arquivoIndicePrimarioBin, int qtdBusca) {
    FILE *pessoa_bin_file = abrirPessoa(arquivoSaidaBin ,"rb"); // ler no novo arquivo
    if(pessoa_bin_file == NULL) return;

    // 1. Verifica consistência do arquivo de dados
    PessoaHeader pessoa_header;
    if(verificaStatusPessoa(&pessoa_header) == 0) return;

    // 2. Carrega o índice para a Árvore AVL em memória
    ARV* arvoreIndice = criarAVL();
    FILE *indice_bin_file = abrirIndice(arquivoIndicePrimarioBin, "rb");
    if (indice_bin_file == NULL) return;

    // Ler cabeçalho
    IndexHeader indice_header;
    if(verificaStatusIndice(&indice_header) == 0) return; // Verificar consistência

    escreveIndiceArvore(indice_bin_file, arvoreIndice); // Processamento de leitura e inserção
    fclose(indice_bin_file);

    // 3. Processa as buscas
    int numBusca = 1;
    while (numBusca <= qtdBusca) {
        char linha[256];
        
        // Lê a linha de comando de busca
        memset(linha, 0, sizeof(linha));
        if (fgets(linha, sizeof(linha), stdin) == NULL) break;
        linha[strcspn(linha, "\r\n")] = '\0'; // Remove \n e \r
        if (strlen(linha) == 0) continue; // Pula linhas vazias

        // Prepara a linha para tokenização (ex: "1 campo=valor")
        char linha_copia[256];
        strcpy(linha_copia, linha);

        char *token = strtok(linha_copia, " ");
        if (token == NULL) { 
            printf("Registro inexistente.\n\n");
            numBusca++;
            continue;
        }

        token = strtok(NULL, " "); // Pula o número da busca
        if (token == NULL) {
            printf("Registro inexistente.\n\n");
            numBusca++;
            continue;
        }

        // Processa campo=valor
        char campo[50];
        char valor[200];

        char *igual_pos = strchr(token, '=');
        if (igual_pos == NULL) {
            printf("Registro inexistente.\n\n");
            numBusca++;
            continue;
        }

        // Extrai 'campo'
        int campo_len = igual_pos - token;
        strncpy(campo, token, campo_len);
        campo[campo_len] = '\0';

        // Extrai 'valor'
        strcpy(valor, igual_pos + 1);

        // Trata valores com espaços (se houver mais tokens)
        char *resto = strtok(NULL, "");
        if (resto != NULL && strlen(resto) > 0) {
            strcat(valor, " ");
            strcat(valor, resto);
        }

        // Limpa o valor (remove aspas e espaços extras)
        char valor_final[200];
        scan_string_aspas(valor_final, valor);

        // 4. Executa a busca
        fseek(pessoa_bin_file, PESSOA_HEADER_SIZE, SEEK_SET); // pula cabeçalho
        Lista *lista_registros = buscaPessoa(pessoa_bin_file, arvoreIndice, campo, valor_final);
        imprimirLista(lista_registros); // imprimir registros encontrados

        // se a lista não tiver nenhum registro encontrado
        if (lista_registros->tamanho == 0) {
            printf("Registro inexistente.\n\n");
        }

        liberarLista(lista_registros); // liberar memória
        numBusca++; // atualiza contagem de buscas
    }

    // 5. Libera memória
    liberarAVL(arvoreIndice->raiz);
    free(arvoreIndice);
    fclose(pessoa_bin_file);
}

/*
Funcionalidade 5: Remoção Lógica de um Registro de Dados do Arquivo pessoa.bin
Seguindo a busca de registro de dados geral, encontra-se o registro e remove-o logicamente.
@param arquivoEntradaBin: Nome do arquivo binário de dados (pessoa.bin).
@param arquivoIndicePrimarioBin: Nome do arquivo binário de índice (indexaPessoa.bin).
@param qtdDelete: Número de registros a serem deletados.
*/
void deletarRegistros(char *arquivoEntradaBin, char *arquivoIndicePrimarioBin, int qtdDelete) {
    FILE *pessoa_bin_file = abrirPessoa(arquivoEntradaBin ,"r+b"); // ler e escrever no novo arquivo
    if(pessoa_bin_file == NULL) return;
    
    // 1. Verifica consistência do arquivo de dados
    PessoaHeader pessoa_header;
    lerCabecPessoa(pessoa_bin_file, &pessoa_header);
    if(verificaStatusPessoa(&pessoa_header) == 0) return;
    statusPessoa(pessoa_bin_file, &pessoa_header, '0'); // inconsistente para deletar registros logicamente

    // 2. Carrega o índice para a Árvore AVL em memória
    ARV* arvoreIndice = criarAVL();
    FILE *indice_bin_file = abrirIndice(arquivoIndicePrimarioBin, "rb"); // leitura
    if (indice_bin_file == NULL) return;
    
    escreveIndiceArvore(indice_bin_file, arvoreIndice); // Processamento de leitura e inserção
    fclose(indice_bin_file);

    // 3. Processa as buscas para deletar logicamente
    int numDeletados = 1;
    while (numDeletados <= qtdDelete) {
        char linha[256];
        
        // Lê a linha de comando de busca
        memset(linha, 0, sizeof(linha));
        if (fgets(linha, sizeof(linha), stdin) == NULL) break;
        linha[strcspn(linha, "\r\n")] = '\0'; // Remove \n e \r
        if (strlen(linha) == 0) continue; // Pula linhas vazias

        // Prepara a linha para tokenização (ex: "1 campo=valor")
        char linha_copia[256];
        strcpy(linha_copia, linha);

        char *token = strtok(linha_copia, " ");
        if (token == NULL) { 
            printf("Registro inexistente.\n\n");
            numDeletados++;
            continue;
        }

        token = strtok(NULL, " "); // Pula o número da busca
        if (token == NULL) {
            printf("Registro inexistente.\n\n");
            numDeletados++;
            continue;
        }

        // Processa campo=valor
        char campo[50];
        char valor[200];

        char *igual_pos = strchr(token, '=');
        if (igual_pos == NULL) {
            printf("Registro inexistente.\n\n");
            numDeletados++;
            continue;
        }

        // Extrai 'campo'
        int campo_len = igual_pos - token;
        strncpy(campo, token, campo_len);
        campo[campo_len] = '\0';

        // Extrai 'valor'
        strcpy(valor, igual_pos + 1);

        // Trata valores com espaços (se houver mais tokens)
        char *resto = strtok(NULL, "");
        if (resto != NULL && strlen(resto) > 0) {
            strcat(valor, " ");
            strcat(valor, resto);
        }

        // Limpa o valor (remove aspas e espaços extras)
        char *valor_final = valor;
        if(strcmp(campo,"nomeUsuario") == 0 || strcmp(campo,"nomePessoa") == 0 || strcmp(valor, "NULO")) {
            if(strcmp(valor, "NULO") != 0) scan_string_aspas(valor_final, valor);
            valor_final = trim(valor_final);
        }

        // 4. Executa a busca dos registros
        fseek(pessoa_bin_file, PESSOA_HEADER_SIZE, SEEK_SET); // pula cabeçalho
        Lista *lista_registros = buscaPessoa(pessoa_bin_file, arvoreIndice, campo, valor_final);
        
        if(lista_registros->tamanho > 0) {
            // 5. Deleta os registros da lista
            fseek(pessoa_bin_file, PESSOA_HEADER_SIZE, SEEK_SET);
            deletarPessoaDaLista(pessoa_bin_file, arvoreIndice, lista_registros);
            pessoa_header.quantidadeRemovidos += lista_registros->tamanho; // atualiza tamanho da qtd de removidos
            pessoa_header.quantidadePessoas = pessoa_header.quantidadePessoas - lista_registros->tamanho;
        }

        liberarLista(lista_registros); // liberar memória
        numDeletados++; // atualiza contagem de buscas
    }

    // 6. Abertura do arquivo índice para reescrita
    indice_bin_file = abrirIndice(arquivoIndicePrimarioBin, "wb"); // escrita em um novo arquivo limpo
    if (indice_bin_file == NULL) return;

    IndexHeader index_header;
    initCabecIndice(indice_bin_file, &index_header); // Função para inicializar cabeçalho no arquivo
    if(verificaStatusIndice(&index_header) == 0) return; // verifica consistẽncia
    statusIndice(indice_bin_file, &index_header, '0'); // Inconsistente durante escrita

    // 7. Escrever a árvore AVL no arquivo índice (em ordem crescente)
    fseek(indice_bin_file, INDEX_HEADER_SIZE, SEEK_SET); // Posiciona após o cabeçalho
    printCrescIndice(arvoreIndice->raiz, indice_bin_file);
    statusIndice(indice_bin_file, &index_header, '1');
    fclose(indice_bin_file);

    // 8. Atualizar cabeçalho da qtd de removidos no arquivo pessoa
    atualizaCabecPessoa(pessoa_bin_file, &pessoa_header);

    // 9. Libera memória
    liberarAVL(arvoreIndice->raiz);
    free(arvoreIndice);

    // 10. Fechar arquivo
    fclose(pessoa_bin_file);

    // 11. Debug
    binarioNaTela(arquivoEntradaBin);
    binarioNaTela(arquivoIndicePrimarioBin);
}


/*
Funcionalidade 6: Inserção de Novos Registros de dados no arquivo pessoa.bin
Insere o novo registro ao final do arquivo pessoa.bin sem reaproveitamento de espaço dos registros removidos
@param arquivoEntradaBin: Nome do arquivo binário de dados (pessoa.bin).
@param arquivoIndicePrimarioBin: Nome do arquivo binário de índice (indexaPessoa.bin).
@param qtdInseridos: Número de registros a serem inseridos.
*/
void inserirRegistros(char *arquivoEntradaBin, char *arquivoIndicePrimarioBin, int qtdInseridos) {
    // Abertura dos arquivos
    FILE *pessoa_bin_file = abrirPessoa(arquivoEntradaBin ,"r+b"); // criar e escrever no novo arquivo
    if(pessoa_bin_file == NULL) return;

    // 1. Verifica consistência do arquivo de dados pessoa e atualiza consistência
    PessoaHeader pessoa_header;
    lerCabecPessoa(pessoa_bin_file, &pessoa_header);
    if(verificaStatusPessoa(&pessoa_header) == 0) return;
    statusPessoa(pessoa_bin_file, &pessoa_header, '0');
    fseek(pessoa_bin_file, 0, SEEK_END);

    // 2. Carrega o índice para a Árvore AVL em memória
    ARV* arvoreIndice = criarAVL();
    FILE *indice_bin_file = abrirIndice(arquivoIndicePrimarioBin, "rb"); // leitura
    if (indice_bin_file == NULL) return;

    escreveIndiceArvore(indice_bin_file, arvoreIndice); // Processamento de leitura e inserção
    fclose(indice_bin_file);

    // 3. Processa as buscas para inserir os registros
    int numInseridos = 1;
    while(numInseridos <= qtdInseridos) {
        char linha[256];
        
        // Lê a linha de comando de busca
        memset(linha, 0, sizeof(linha));
        if (fgets(linha, sizeof(linha), stdin) == NULL) break;
        linha[strcspn(linha, "\r\n")] = '\0'; // Remove \n e \r
        if (strlen(linha) == 0) continue; // Pula linhas vazias

        // Prepara a linha para tokenização (ex: "1 idPessoa, "nomePessoa", idadePessoa, "nomeUsuario")
        char linha_copia[256];
        strcpy(linha_copia, linha);

        char *token = strtok(linha_copia, " "); // pula contador
        if (token == NULL) { 
            printf("Registro inexistente.\n\n");
            numInseridos++;
            continue;
        }

        char *rest = strtok(NULL, ""); // pula contador
        if (rest == NULL) { 
            printf("Registro inexistente.\n\n");
            numInseridos++;
            continue;
        }

        // Trata idPessoa, "nomePessoa", idadePessoa, "nomeUsuario"
        PessoaRecord record;
        record.removido = NAO_REMOVIDO_CHAR;
        record.nomePessoa = NULL;
        record.nomeUsuario = NULL;

        // Extrai campos usando novo_strtok para preservar campos vazios
        
        // idPessoa (int)
        token = novo_strtok(rest, ",", &rest);
        if(token != NULL) {
            token = trim(token);
            record.idPessoa = (strcmp(token, "NULO") != 0) ? atoi(token) : -1;
        }
        
        // nomePessoa (string variável)
        char nomePessoa_buffer[256], tempPessoa[256];
        token = novo_strtok(NULL, ",", &rest);
        if(token != NULL) { 
            token = trim(token);
            if (strcmp(token, "NULO") != 0) {
                scan_string_aspas(tempPessoa, token);
                scan_string_aspas(nomePessoa_buffer, tempPessoa);
                record.nomePessoa = trim(nomePessoa_buffer); // Remove espaços/quebras de linha
                record.tamanhoNomePessoa = strlen(record.nomePessoa);
            } else {
                record.nomePessoa = NULL;
                record.tamanhoNomePessoa = 0;
            }
        }

        // idadePessoa (int)
        token = novo_strtok(NULL, ",", &rest);
        if(token != NULL) {
            token = trim(token);
            record.idadePessoa = (strcmp(token, "NULO") != 0) ? atoi(token) : -1;
        }

        // nomeUsuario (string variável)
        char nomeUsuario_buffer[256], tempUsuario[256];
        token = novo_strtok(NULL, ",", &rest);
        if(token != NULL) { 
            token = trim(token);
            if (strcmp(token, "NULO") != 0) {
                scan_string_aspas(tempUsuario, token);
                scan_string_aspas(nomeUsuario_buffer, tempUsuario);
                record.nomeUsuario = trim(nomeUsuario_buffer); // Remove espaços/quebras de linha
                record.tamanhoNomeUsuario = strlen(record.nomeUsuario);
            } else {
                record.nomeUsuario = NULL;
                record.tamanhoNomeUsuario = 0;
            }
        }
        
        // Calcular tamanho do registro (fixo + variável)
        // 4 * sizeof(int) = idPessoa + idadePessoa + tamanhoNomePessoa + tamanhoNomeUsuario
        record.tamanhoRegistro = 4*sizeof(int);
        if (record.nomePessoa != NULL && record.tamanhoNomePessoa != 0) record.tamanhoRegistro += record.tamanhoNomePessoa;
        if (record.nomeUsuario != NULL && record.tamanhoNomeUsuario != 0) record.tamanhoRegistro += record.tamanhoNomeUsuario;
        
        // 5. Escrever registro no arquivo pessoa.bin
        long int atual_byte_offset = ftell(pessoa_bin_file); // Byte offset de início do registro
        escrevePessoaRecord(pessoa_bin_file, &record);
        
        // 6. Atualizar contagem do cabeçalho do arquivo pessoa.bin
        pessoa_header.quantidadePessoas++;
        
        // 7. Inserir na arvore AVL para o índice
        IndexRecord index_record;
        index_record.idPessoa = record.idPessoa;
        index_record.byteOffset = atual_byte_offset;
        insereNoAVL(arvoreIndice, index_record.idPessoa, index_record.byteOffset);
        
        // 8. Liberar memória alocada
        if (record.nomePessoa != NULL) free(record.nomePessoa);
        if (record.nomeUsuario != NULL) free(record.nomeUsuario);

        numInseridos++;
    }

    // 9. Atualizar o cabeçalho do arquivo pessoa
    statusPessoa(pessoa_bin_file, &pessoa_header, '1');
    atualizaCabecPessoa(pessoa_bin_file, &pessoa_header);
    fclose(pessoa_bin_file);

    // 10. Reescrever o arquivo de índice
    indice_bin_file = abrirIndice(arquivoIndicePrimarioBin, "r+b"); // leitura e escrita
    if (indice_bin_file == NULL) return;

    IndexHeader indice_header;
    if(verificaStatusIndice(&indice_header) == 0) return;

    fseek(indice_bin_file, INDEX_HEADER_SIZE, SEEK_SET);
    printCrescIndice(arvoreIndice->raiz, indice_bin_file);
    statusIndice(indice_bin_file, &indice_header, '1');
    fclose(indice_bin_file);

    // 11. Liberar memória
    liberarAVL(arvoreIndice->raiz);
    free(arvoreIndice);

    binarioNaTela(arquivoEntradaBin);
    binarioNaTela(arquivoIndicePrimarioBin);
}

/*
Funcionalidade 7: Atualização de Registros existentes do arquivo pessoa.bin com busca do campo a ser alterado
Busca do registro com referência ao campo, tratando o reaproveitamento de espaço da modificação dos registros
@param arquivoEntradaBin: Nome do arquivo binário de dados (pessoa.bin).
@param arquivoIndicePrimarioBin: Nome do arquivo binário de índice (indexaPessoa.bin).
@param qtdBusca: Número de buscas a serem realizadas.
*/
void atualizarRegistros(char *arquivoEntradaBin, char *arquivoIndicePrimarioBin, int qtdBusca) {
    FILE *pessoa_bin_file = abrirPessoa(arquivoEntradaBin ,"r+b"); // ler no novo arquivo
    if(pessoa_bin_file == NULL) return;

    // 1. Verifica consistência do arquivo de dados
    PessoaHeader pessoa_header;
    lerCabecPessoa(pessoa_bin_file, &pessoa_header);
    if(verificaStatusPessoa(&pessoa_header) == 0) return;
    statusPessoa(pessoa_bin_file, &pessoa_header, '0'); // garantir que durante escrita, o arq esteja inconsistente

    // 2. Carrega o índice para a Árvore AVL em memória
    ARV* arvoreIndice = criarAVL();
    FILE *indice_bin_file = abrirIndice(arquivoIndicePrimarioBin, "rb");
    if (indice_bin_file == NULL) return;

    escreveIndiceArvore(indice_bin_file, arvoreIndice); // Processamento de leitura e inserção
    fclose(indice_bin_file);

    // 3. Processa as buscas
    int numBusca = 1;
    while (numBusca <= qtdBusca) {
        char linha[256];
        
        // Lê a linha de comando de busca
        memset(linha, 0, sizeof(linha));
        if (fgets(linha, sizeof(linha), stdin) == NULL) break;
        linha[strcspn(linha, "\r\n")] = '\0'; // Remove \n e \r
        if (strlen(linha) == 0) continue; // Pula linhas vazias

        // Prepara a linha para tokenização (ex: "1 campo1=valor1 campo2=valor2")
        char linha_copia[256];
        strcpy(linha_copia, linha);

        char *token = strtok(linha_copia, " "); // Pula o número da busca
        if (token == NULL) {
            printf("Registro inexistente.\n\n");
            numBusca++;
            continue;
        }

        // token campo1=valor1
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("Registro inexistente.\n\n");
            numBusca++;
            continue;
        }

        // Processa campo1=valor1
        char campo1[50];
        char valor1[200];
        char *igual_pos = strchr(token, '=');
        if (igual_pos == NULL) {
            printf("Registro inexistente.\n\n");
            numBusca++;
            continue;
        }

        // Extrai 'campo1'
        int campo_len = igual_pos - token;
        strncpy(campo1, token, campo_len);
        campo1[campo_len] = '\0';

        // Extrai 'valor1'
        char *inicio_valor1 = igual_pos + 1;
        strcpy(valor1, inicio_valor1);

        // Pega o resto da linha para campo2=valor2
        char *resto = strtok(NULL, "");
        if (resto == NULL) {
            printf("Registro inexistente.\n\n");
            numBusca++;
            continue;
        }

        // Processa campo2=valor2
        char campo2[50];
        char valor2[200];
        igual_pos = strchr(resto, '=');
        if (igual_pos == NULL) {
            printf("Registro inexistente.\n\n");
            numBusca++;
            continue;
        }

        // Extrai 'campo2'
        campo_len = igual_pos - resto;
        strncpy(campo2, resto, campo_len);
        campo2[campo_len] = '\0';

        // Extrai 'valor2'
        strcpy(valor2, igual_pos + 1);

        // Limpa o valor (remove aspas e espaços extras)
        char *valor1_final = valor1;
        if(strcmp(campo1, "nomeUsuario") == 0 || strcmp(campo1, "nomePessoa") == 0) {
            if(strcmp(valor1, "NULO") != 0) scan_string_aspas(valor1_final, valor1);
            trimSemAloc(valor1_final);
        } else if(strcmp(valor1, "NULO") != 0) {
            scan_string_aspas(valor1_final, valor1);
        }

        char *valor2_final = valor2;
        if(strcmp(campo2, "nomeUsuario") == 0 || strcmp(campo2, "nomePessoa") == 0) {
            if(strcmp(valor2, "NULO") != 0) scan_string_aspas(valor2_final, valor2);
            trimSemAloc(valor2_final);
        } else if(strcmp(valor2, "NULO") != 0) {
            scan_string_aspas(valor2_final, valor2);
        }

        // 4. Executa a busca
        fseek(pessoa_bin_file, PESSOA_HEADER_SIZE, SEEK_SET); // pula cabeçalho
        Lista *lista_registros = buscaPessoa(pessoa_bin_file, arvoreIndice, campo1, valor1_final);
        
        if(lista_registros != NULL) {
            if(lista_registros->tamanho > 0) {
                // atualizar os registros encontrados na arvore
                pessoa_header.quantidadeRemovidos += atualizaPessoa(pessoa_bin_file, arvoreIndice, lista_registros, campo2, valor2_final);            
            }

            liberarLista(lista_registros); // liberar memória
        }

        // Atualizar contagem
        numBusca++; // atualiza contagem de buscas
    }

    // Reescrever o indice
    indice_bin_file = abrirIndice(arquivoIndicePrimarioBin, "r+b"); // leitura e escrita
    if (indice_bin_file == NULL) return;

    IndexHeader indice_header;
    lerCabecIndice(indice_bin_file, &indice_header);
    if(verificaStatusIndice(&indice_header) == 0) {
        fclose(indice_bin_file);
        fclose(pessoa_bin_file);
        liberarAVL(arvoreIndice->raiz);
        free(arvoreIndice);
        return;
    }

    statusIndice(indice_bin_file, &indice_header, '0');

    fseek(indice_bin_file, INDEX_HEADER_SIZE, SEEK_SET);
    printCrescIndice(arvoreIndice->raiz, indice_bin_file);

    statusIndice(indice_bin_file, &indice_header, '1');
    fclose(indice_bin_file);

    // 5. Atualiza cabeçalho
    atualizaCabecPessoa(pessoa_bin_file, &pessoa_header);

    // 6. Libera memória
    liberarAVL(arvoreIndice->raiz);
    free(arvoreIndice);
    fclose(pessoa_bin_file);

    binarioNaTela(arquivoEntradaBin);
    binarioNaTela(arquivoIndicePrimarioBin);
}


/*
Funcionalidade 8: Criação de Arquivo Binário Segue
Cria o arquivo de segue binário com o cabeçalho inicializado e com o processamento do CSV
@param arquivoEntradaCSV: Nome do arquivo CSV de entrada.
@param arquivoSaidaBin: Nome do arquivo binário de segue a ser criado.
*/
void criarArquivoSegueBinario(char *arquivoEntradaCSV, char *arquivoSaidaBin) {
    // 1. Criar arquivo segue, inicializar cabeçalho do arquivo e abrir arquivo csv para leitura
    FILE *csv_file = fopen(arquivoEntradaCSV, "r");
    if (csv_file == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    FILE *segue_bin_file = abrirIndice(arquivoSaidaBin, "wb");
    if (segue_bin_file == NULL) return;

    SegueHeader segue_header;
    initCabecSegue(segue_bin_file, &segue_header); // Função para inicializar cabeçalho no arquivo

    // 2. Atualizar status do cabeçalho do arquivo segue para '0' (inconsistente)
    statusSegue(segue_bin_file, &segue_header, '0');    

    char line[1024];
    // Ignorar a primeira linha (cabeçalho do CSV)
    fgets(line, sizeof(line), csv_file);

    // 4. Processamento de cada linha do CSV
    fseek(segue_bin_file, SEGUE_HEADER_SIZE, SEEK_SET);
    while (fgets(line, sizeof(line), csv_file) != NULL) {
        line[strcspn(line, "\r\n")]= '\0';

        SegueRecord record;
        record.removido = NAO_REMOVIDO_CHAR;

        char *token;
        char *rest = line;

        // Extrai campos usando novo_strtok para preservar campos vazios
        
        // idPessoaQueSegue (int)
        token = novo_strtok(rest, ",", &rest);
        record.idPessoaQueSegue = (token != NULL && token[0] != '\0') ? atoi(token) : -1;
        
        // idPessoaQueESeguida(int)
        token = novo_strtok(rest, ",", &rest);
        record.idPessoaQueESeguida = (token != NULL && token[0] != '\0') ? atoi(token) : -1;

        // dataInicioQueSegue (string fixa)
        char dataInicioQueSegue_buffer[11];
        for(int i = 0; i < 10; i++) dataInicioQueSegue_buffer[i] = LIXO_CHAR;
        dataInicioQueSegue_buffer[10] = '\0';
        token = novo_strtok(NULL, ",", &rest);
        if (token != NULL && token[0] != '\0' && strcmp(token, "") != 0) {
            strncpy(dataInicioQueSegue_buffer, token, 11);
        }
        memcpy(record.dataInicioQueSegue, dataInicioQueSegue_buffer, 11);

        // dataFimQueSegue (string fixa)
        char dataFimQueSegue_buffer[11];
        for(int i = 0; i < 10; i++) dataFimQueSegue_buffer[i] = LIXO_CHAR;
        dataFimQueSegue_buffer[10] = '\0';
        token = novo_strtok(NULL, ",", &rest);
        if (token != NULL && token[0] != '\0' && strcmp(token, "") != 0) {
            strncpy(dataFimQueSegue_buffer, token, 11);
        }
        memcpy(record.dataFimQueSegue, dataFimQueSegue_buffer, 11);

        // grauAmizade (char)
        token = novo_strtok(NULL, ",", &rest);
        record.grauAmizade = (token != NULL && token[0] != '\0' && strcmp(token, "") != 0) ? token[0] : LIXO_CHAR;

        // 5. Atualizar contagem do cabeçalho
        segue_header.quantidadePessoas++;

        // 6. Escrever registro no arquivo segue.bin
        escreveSegueRecord(segue_bin_file, &record);

    }

    // 7. Atualizar cabeçalho final do arquivo pessoa.bin (dados)
    atualizaCabecSegue(segue_bin_file, &segue_header);

    // 8. Atualizar cabeçalho final do arquivo de índice
    statusSegue(segue_bin_file, &segue_header, '1');

    // 9. Fechar arquivos e liberar memória
    fclose(csv_file);
    fclose(segue_bin_file);

    binarioNaTela(arquivoSaidaBin); // Função de debug
}


/*
Funcionalidade 9: Ordenar o Arquivo Segue de acordo com o campo idPessoaQueSegue de forma crescente
Leitura do arquivo segue armazenado em RAM e ordenado de forma crescente seguindo os critérios;
ao fim, criar um novo arquivo de dados segueOrdenado
@param arquivoSegueDesordenado: Nome do arquivo binário de segue desordenado.
@param arquivoSegueOrdenado: Nome do arquivo binário de segue ordenado.
*/
void ordenarArquivoSegue(char *arquivoSegueDesordenado, char *arquivoSegueOrdenado) {
    // 1. Abrir arquivo segue e ler cabeçalho do arquivo
    FILE *desordenado_bin_file = abrirSegue(arquivoSegueDesordenado, "rb");
    if (desordenado_bin_file == NULL) return;

    SegueHeader desordenado_header;
    lerCabecSegue(desordenado_bin_file, &desordenado_header); // Função para ler cabeçalho no arquivo
    if(verificaStatusSegue(desordenado_bin_file, &desordenado_header) == 0) return; // verifica status
    
    // 2. Criar arquivo segue novo para ordenar os registros
    FILE *ordenado_bin_file = abrirSegue(arquivoSegueOrdenado, "wb");
    if (ordenado_bin_file == NULL) return;

    SegueHeader ordenado_header;
    copiaCabecSegue(ordenado_bin_file, &ordenado_header, &desordenado_header);
    statusSegue(ordenado_bin_file, &ordenado_header, '0'); // inconsistente para escrita

    // 3. Criar vetor para guardar os registros lidos
    SegueRecord segue_record[ordenado_header.quantidadePessoas];
    lerSegueEmVetor(desordenado_bin_file, &segue_record[0], ordenado_header.quantidadePessoas);

    // 4. Ordenar o vetor por QuickSort pela biblioteca c
    qsort(&segue_record[0], ordenado_header.quantidadePessoas, sizeof(SegueRecord), compare);

    // 5. Escrever o vetor em ordem no arquivo segue ordenado
    escreveVetorEmSegue(ordenado_bin_file, &segue_record[0], ordenado_header.quantidadePessoas);
    statusSegue(ordenado_bin_file, &ordenado_header, '1'); // consistente pós escrita

    // 6. Fechar os arquivos
    fclose(ordenado_bin_file);
    fclose(desordenado_bin_file);

    // 7. debug
    binarioNaTela(arquivoSegueOrdenado);
}

/*
Funcionalidade 10: Junção dos Arquivos de Dados pessoa.bin e segueOrdenado.bin 
Relaciona-se pelo idPessoa e pelo idPessoaQueSegue para mesclar os arquivos, para retornar os registros
de idPessoa presentes no arquivo segueOrdenado.bin comparado ao campo idPessoaQueSegue
*/
void juncaoPessoaSegue(char *arquivoEntradaBin, char *arquivoIndicePrimarioBin, char *arquivoSegueOrdenado, int qtdBusca) {
    // 1. Abertura dos arquivos pessoa, indice e segue
    FILE *pessoa_bin_file = abrirPessoa(arquivoEntradaBin, "rb");
    if(pessoa_bin_file == NULL) return;

    FILE *indice_bin_file = abrirIndice(arquivoIndicePrimarioBin, "rb");
    if(indice_bin_file == NULL) return;
    
    FILE *segue_bin_file = abrirSegue(arquivoSegueOrdenado, "rb");
    if(segue_bin_file == NULL) return;

    // 2. Ler cabeçalho do arquivo
    PessoaHeader pessoa_header;
    lerCabecPessoa(pessoa_bin_file, &pessoa_header);

    IndexHeader indice_header;
    lerCabecIndice(indice_bin_file, &indice_header);

    SegueHeader segue_header;
    lerCabecSegue(segue_bin_file, &segue_header);

    // 3. Verifica consistência dos arquivos
    if(verificaStatusPessoa(&pessoa_header) == 0) return;
    if(verificaStatusIndice(&indice_header) == 0) return;
    if(verificaStatusSegue(segue_bin_file, &segue_header) == 0) return;

    // 4. Carregar indice na arvore e segue em vetor em memória
    // indice na arvore AVL
    ARV* arvoreIndice = criarAVL();
    escreveIndiceArvore(indice_bin_file, arvoreIndice); // Processamento de leitura e inserção
    fclose(indice_bin_file); // fechamento do arquivo indice que nao sera mais usado

    // segue em vetor ordenado por qsort
    SegueRecord segue_record[segue_header.quantidadePessoas];
    lerSegueEmVetor(segue_bin_file, &segue_record[0], segue_header.quantidadePessoas);

    // 5. Processa a busca para junção a partir de idPessoa do arquivo Pessoa
    int numBusca = 1;
    while (numBusca <= qtdBusca) {
        // 6. Leitura do stdin para determinar busca
        char linha[256];
        
        // Lê a linha de comando de busca
        memset(linha, 0, sizeof(linha));
        if (fgets(linha, sizeof(linha), stdin) == NULL) break;
        linha[strcspn(linha, "\r\n")] = '\0'; // Remove \n e \r
        if (strlen(linha) == 0) continue; // Pula linhas vazias

        // Prepara a linha para tokenização (ex: "1 campo=valor")
        char linha_copia[256];
        strcpy(linha_copia, linha);

        char *token = strtok(linha_copia, " ");
        if (token == NULL) { 
            printf("Registro inexistente.\n\n");
            numBusca++;
            continue;
        }

        token = strtok(NULL, " "); // Pula o número da busca
        if (token == NULL) {
            printf("Registro inexistente.\n\n");
            numBusca++;
            continue;
        }

        // Processa campo=valor
        char campo[50];
        char valor[200];

        char *igual_pos = strchr(token, '=');
        if (igual_pos == NULL) {
            printf("Registro inexistente.\n\n");
            numBusca++;
            continue;
        }

        // Extrai 'campo'
        int campo_len = igual_pos - token;
        strncpy(campo, token, campo_len);
        campo[campo_len] = '\0';

        // Extrai 'valor'
        strcpy(valor, igual_pos + 1);

        // Trata valores com espaços (se houver mais tokens)
        char *resto = strtok(NULL, "");
        if (resto != NULL && strlen(resto) > 0) {
            strcat(valor, " ");
            strcat(valor, resto);
        }

        // Limpa o valor (remove aspas e espaços extras)
        char valor_final[200];
        scan_string_aspas(valor_final, valor);

        // 7. Lista com registros encontrados na busca
        fseek(pessoa_bin_file, PESSOA_HEADER_SIZE, SEEK_SET);
        Lista* lista_registros = buscaPessoa(pessoa_bin_file, arvoreIndice, campo, valor_final);
        // Busca os registros segue relacionados com o valor de busca do arq pessoa
        // além de que ja imprime no terminal os dados necessários
        buscarPessoaEmSegue(lista_registros, &segue_record[0], segue_header.quantidadePessoas);

        // 8. Libera lista
        liberarLista(lista_registros);

        numBusca++;
    }

    // 9. Libera memória
    liberarAVL(arvoreIndice->raiz);
    free(arvoreIndice);

    // 10. Fecha arquivos
    fclose(pessoa_bin_file);
    fclose(segue_bin_file);
}

/*
Funcionalidade 11: Criar grafo direcionado (lista de adjacências).
Representa quem segue quem (nomeUsuarioQueSegue -> nomeUsuarioQueESeguida).
@param arquivoPessoa: Nome do arquivo binário de pessoas.
@param arquivoIndexaPessoa: Nome do arquivo binário de índice.
@param arquivoSegueOrdenado: Nome do arquivo binário de segue ordenado.
*/
void criarGrafoDirecionado(char *arquivoPessoa, char *arquivoIndexaPessoa, char *arquivoSegueOrdenado) {
    Grafo *grafo = construirGrafo(arquivoPessoa, arquivoIndexaPessoa, arquivoSegueOrdenado, 0);
    
    if (grafo == NULL) {
        return;
    }
    
    // Imprimir grafo
    imprimirGrafo(grafo);
    
    // Liberar memória
    liberarGrafo(grafo);
}

/*
Funcionalidade 12: Criar grafo transposto.
Inverte as arestas do grafo (nomeUsuarioQueESeguida -> nomeUsuarioQueSegue).
@param arquivoPessoa: Nome do arquivo binário de pessoas.
@param arquivoIndexaPessoa: Nome do arquivo binário de índice.
@param arquivoSegueOrdenado: Nome do arquivo binário de segue ordenado.
*/
void criarGrafoTransposto(char *arquivoPessoa, char *arquivoIndexaPessoa, char *arquivoSegueOrdenado) {
    Grafo *grafo = construirGrafo(arquivoPessoa, arquivoIndexaPessoa, arquivoSegueOrdenado, 1);
    
    if (grafo == NULL) {
        return;
    }
    
    // Imprimir grafo
    imprimirGrafo(grafo);
    
    // Liberar memória
    liberarGrafo(grafo);
}


/*
Funcionalidade 13: Caminho mais curto até celebridade (BFS).
Determina o caminho mais curto de cada pessoa até uma celebridade específica.
Apenas pessoas que ainda seguem a celebridade são consideradas (dataFim == NULO).
Apenas o primeiro caminho encontrado é listado.
@param arquivoPessoa: Nome do arquivo binário de pessoas.
@param arquivoIndexaPessoa: Nome do arquivo binário de índice.
@param arquivoSegueOrdenado: Nome do arquivo binário de segue ordenado.
@param nomeUsuarioCelebridade: Nome da celebridade (entre aspas).
*/
void caminhoParaCelebridade(char *arquivoPessoa, char *arquivoIndexaPessoa, char *arquivoSegueOrdenado, char *nomeUsuarioCelebridade) {
    // Construir grafo normal
    Grafo *grafo = construirGrafo(arquivoPessoa, arquivoIndexaPessoa, arquivoSegueOrdenado, 0);
    if (grafo == NULL) {
        return;
    }
    
    // Remover aspas do nome da celebridade
    char nomeCelebridade[256];
    scan_string_aspas(nomeCelebridade, nomeUsuarioCelebridade);
    
    // Buscar índice da celebridade
    int indiceCelebridade = buscarVertice(grafo, nomeCelebridade);
    if (indiceCelebridade == -1) {
        liberarGrafo(grafo);
        return;
    }
    
    // Para cada vértice, executar BFS até a celebridade
    for (int i = 0; i < grafo->numVertices; i++) {
        if (i == indiceCelebridade) continue; // Pular a própria celebridade
        
        // Arrays de controle do BFS
        int *visitado = (int*)calloc(grafo->numVertices, sizeof(int));
        int *predecessor = (int*)malloc(grafo->numVertices * sizeof(int));
        Aresta **arestaUsada = (Aresta**)malloc(grafo->numVertices * sizeof(Aresta*));
        
        for (int j = 0; j < grafo->numVertices; j++) {
            predecessor[j] = -1;
            arestaUsada[j] = NULL;
        }
        
        // BFS
        Fila *fila = criarFila();
        enfileirar(fila, i);
        visitado[i] = 1;
        
        int encontrou = 0;
        while (!filaVazia(fila) && !encontrou) {
            int atual = desenfileirar(fila);
            
            // Percorrer arestas adjacentes
            Aresta *aresta = grafo->vertices[atual].listaArestas;
            
            while (aresta != NULL && !encontrou) {
                // Verificar se ainda segue (dataFim == NULO)
                int aindaSegue = 0;
                
                if (strcmp(aresta->dataFim, "NULO") == 0) {
                    aindaSegue = 1;
                } else if (aresta->dataFim[0] == LIXO_CHAR) {
                    aindaSegue = 1;
                }
                
                if (!aindaSegue) {
                    aresta = aresta->prox;
                    continue;
                }
                
                int indiceDestino = buscarVertice(grafo, aresta->nomeUsuario);
                
                // Se este destino ainda não foi visitado, processar
                if (indiceDestino != -1 && !visitado[indiceDestino]) {
                    visitado[indiceDestino] = 1;
                    predecessor[indiceDestino] = atual;
                    arestaUsada[indiceDestino] = aresta;
                    enfileirar(fila, indiceDestino);
                    
                    // Se encontrou a celebridade, parar imediatamente
                    if (indiceDestino == indiceCelebridade) {
                        encontrou = 1;
                        break;
                    }
                }
                
                aresta = aresta->prox;
            }
        }
        
        liberarFila(fila);
        
        // Se encontrou caminho, reconstruir e imprimir
        if (encontrou) {
            // Reconstruir caminho
            int *caminho = (int*)malloc(grafo->numVertices * sizeof(int));
            Aresta **arestasCaminho = (Aresta**)malloc(grafo->numVertices * sizeof(Aresta*));
            int tamanhoCaminho = 0;
            
            int atual = indiceCelebridade;
            while (atual != i) {
                caminho[tamanhoCaminho] = atual;
                arestasCaminho[tamanhoCaminho] = arestaUsada[atual];
                tamanhoCaminho++;
                atual = predecessor[atual];
            }
            caminho[tamanhoCaminho] = i;
            tamanhoCaminho++;
            
            // Imprimir caminho (da pessoa até a celebridade)
            for (int k = tamanhoCaminho - 1; k > 0; k--) {
                printf("%s, ", grafo->vertices[caminho[k]].nomeUsuario);
                printf("%s, ", grafo->vertices[caminho[k-1]].nomeUsuario);
                
                Aresta *a = arestasCaminho[k-1];
                
                printf("%s, ", a->dataInicio);
                printf("%s, ", a->dataFim);
                
                if (a->grauAmizade == LIXO_CHAR) {
                    printf("NULO\n");
                } else {
                    printf("%c\n", a->grauAmizade);
                }
            }
            
            printf("\n");
            
            free(caminho);
            free(arestasCaminho);
        } else {
            // Não segue a celebridade
            printf("NAO SEGUE A CELEBRIDADE\n\n");
        }
        
        free(visitado);
        free(predecessor);
        free(arestaUsada);
    }
    
    // Liberar memória
    liberarGrafo(grafo);
}


/*
Funcionalidade 14: Comprimento do ciclo da fofoca (BFS).
Determina o comprimento do caminho para que a fofoca retorne à pessoa que a gerou.
@param arquivoPessoa: Nome do arquivo binário de pessoas.
@param arquivoIndexaPessoa: Nome do arquivo binário de índice.
@param arquivoSegueOrdenado: Nome do arquivo binário de segue ordenado.
@param nomeUsuarioFofoca: Nome da pessoa que gerou a fofoca (entre aspas).
*/
void comprimentoCicloFofoca(char *arquivoPessoa, char *arquivoIndexaPessoa, char *arquivoSegueOrdenado, char *nomeUsuarioFofoca) {
    // Construir grafo normal
    Grafo *grafo = construirGrafo(arquivoPessoa, arquivoIndexaPessoa, arquivoSegueOrdenado, 0);
    
    if (grafo == NULL) {
        return;
    }
    
    // Remover aspas do nome
    char nomeFofoca[256];
    scan_string_aspas(nomeFofoca, nomeUsuarioFofoca);
    
    // Buscar índice da pessoa
    int indiceFofoca = buscarVertice(grafo, nomeFofoca);
    if (indiceFofoca == -1) {
        liberarGrafo(grafo);
        printf("Falha na execução da funcionalidade.\n");
        return;
    }
    
    // Arrays de controle do BFS
    int *visitado = (int*)calloc(grafo->numVertices, sizeof(int));
    int *distancia = (int*)malloc(grafo->numVertices * sizeof(int));
    
    for (int j = 0; j < grafo->numVertices; j++) {
        distancia[j] = -1;
    }
    
    // BFS a partir dos vizinhos da pessoa (não da própria pessoa)
    Fila *fila = criarFila();
    
    // Enfileirar vizinhos diretos (quem a pessoa segue)
    Aresta *aresta = grafo->vertices[indiceFofoca].listaArestas;
    while (aresta != NULL) {
        int indiceDestino = buscarVertice(grafo, aresta->nomeUsuario);
        if (indiceDestino != -1 && !visitado[indiceDestino]) {
            visitado[indiceDestino] = 1;
            distancia[indiceDestino] = 1;
            enfileirar(fila, indiceDestino);
        }
        aresta = aresta->prox;
    }
    
    int encontrou = 0;
    int comprimentoCiclo = -1;
    
    while (!filaVazia(fila) && !encontrou) {
        int atual = desenfileirar(fila);
        
        // Percorrer arestas adjacentes em ordem alfabética
        Aresta *arestaAtual = grafo->vertices[atual].listaArestas;
        while (arestaAtual != NULL) {
            int indiceDestino = buscarVertice(grafo, arestaAtual->nomeUsuario);
            
            // Se voltou para a pessoa original, encontrou o ciclo
            if (indiceDestino == indiceFofoca) {
                comprimentoCiclo = distancia[atual] + 1;
                encontrou = 1;
                break;
            }
            
            if (indiceDestino != -1 && !visitado[indiceDestino]) {
                visitado[indiceDestino] = 1;
                distancia[indiceDestino] = distancia[atual] + 1;
                enfileirar(fila, indiceDestino);
            }
            
            arestaAtual = arestaAtual->prox;
        }
    }
    
    liberarFila(fila);
    free(visitado);
    free(distancia);
    
    // Imprimir resultado
    if (encontrou) {
        printf("%d\n", comprimentoCiclo);
    } else {
        printf("A FOFOCA NAO RETORNOU\n");
    }
    
    // Liberar memória
    liberarGrafo(grafo);
}
