// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#include "main.h"

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
        deletarRegistros(argv[1] , argv[2], atoi(argv[3]));
        break;

        case 6:
        // Argumentos: 6 [arquivo_dados_bin] [arquivo_indice_bin] [qtd_buscas]
        inserirRegistros(argv[1], argv[2], atoi(argv[3]));
        break;

        case 7:
        // Argumentos: 7 [arquivo_dados_bin] [arquivo_indice_bin] [qtd_buscas]
        atualizarRegistros(argv[1], argv[2], atoi(argv[3]));
        break;

        case 8:
        // Argumentos: 8 [arquivo_csv] [arquivo_segue_bin]
        criarArquivoSegueBinario(argv[1], argv[2]);
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

