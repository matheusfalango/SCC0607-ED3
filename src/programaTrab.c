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
        atualizaAVL(arvoreIndice, index_record.idPessoa, index_record.byteOffset);

        // 8. Liberar memória alocada
        if (record.nomePessoa != NULL) free(record.nomePessoa);
        if (record.nomeUsuario != NULL) free(record.nomeUsuario);
    }

    // 9. Escrever a árvore AVL no terminal (em ordem crescente)
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
    FILE *pessoa_bin_file = fopen(arquivoSaidaBin, "rb");
    if (pessoa_bin_file == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

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
    FILE *pessoa_bin_file = fopen(arquivoSaidaBin, "rb");
    if (pessoa_bin_file == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // 1. Verifica consistência do arquivo de dados
    PessoaHeader pessoa_header;
    if(verificaStatusPessoa(pessoa_bin_file, &pessoa_header) == 0) return;

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
        scan_string_aspas(valor, valor_final);

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


// ===============================================================================
// PROGRAMA PRINCIPAL
// ===============================================================================


/*
Função principal (main) para controlar o fluxo do programa
com base na funcionalidade lida da entrada padrão.
@return: EXIT_SUCCESS (0) em caso de sucesso, EXIT_FAILURE (1) em caso de erro.
*/
int main() {

    int funcionalidade;

    // Buffer para ler a linha dos argumentos (funcionalidade + nomes de arquivos/qtd)
    char line[1024];
    fgets(line, sizeof(line), stdin);

    line[strcspn(line, "\n")] = '\0'; // Remove '\n' se existir

    char *argv[4] = {NULL}; // Array para armazenar os argumentos
    char *rest = strtok(line, " ");
    int index = 0;

    // Tokeniza a linha de entrada
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
    
    // Chama a funcionalidade correta
    switch (funcionalidade) {
        case 1:
        // Argumentos: 1 [nome_arquivo_indice]
        criarArquivoIndicePrimario(argv[1]);
        break;

        case 2:
        // Argumentos: 2 [arquivo_csv] [arquivo_dados_bin] [arquivo_indice_bin]
        processarCSV(argv[1], argv[2], argv[3]);
        break;

        case 3:
        // Argumentos: 3 [arquivo_dados_bin]
        listarRegistros(argv[1]);
        break;

        case 4:
        // Argumentos: 4 [arquivo_dados_bin] [arquivo_indice_bin] [qtd_buscas]
        buscarRegistros(argv[1], argv[2], atoi(argv[3]));
        break;

        case 5:
        // Argumentos: 5 [arquivo_dados_bin] [arquivo_indice_bin] [qtd_buscas]
        break;

        case 6:
        // Argumentos: 6 [arquivo_dados_bin] [arquivo_indice_bin] [qtd_buscas]
        break;

        case 7:
        // Argumentos: 7 [arquivo_dados_bin] [arquivo_indice_bin] [qtd_buscas]
        break;

        case 8:
        // Argumentos: 8 [arquivo_csv] [arquivo_segue_bin]
        break;

        case 9:
        // Argumentos: 9 [arquivo_segue_bin] [arquivo_segue_ordenado_bin]
        break;

        case 10:
        // Argumentos: 10 [arquivo_dados_bin] [arquivo_indice_bin] [arquivo_segue_ordenado_bin] [qtd_buscas]
        break;

        default:
        printf("Funcionalidade inválida");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}