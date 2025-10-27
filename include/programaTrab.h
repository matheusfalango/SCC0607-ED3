// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira


#ifndef PROGRAMA_TRAB_H
#define PROGRAMA_TRAB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilidades.h"
#include "tratarString.h"
#include "manipularAVL.h"
#include "ArvAVL.h"
#include "lista.h"
#include "def.h"


// ===============================================================================
// FUNCIONALIDADES - PARTE INTRODUTÓRIA
// ===============================================================================

// Funcionalidade 1: Criação de Arquivo de Índice Primário
// Cria o arquivo de índice binário apenas com o cabeçalho inicializado.
void criarArquivoIndicePrimario(char *nomeArquivoIndice);

// Funcionalidade 2: Leitura de CSV e Geração de Arquivo de Dados e Índice
// Lê o CSV, gera o arquivo de dados binário e o arquivo de índice primário (usando AVL em memória).
void processarCSV(char *arquivoEntradaCSV, char *arquivoSaidaBin, char *arquivoIndicePrimarioBin);

// Funcionalidade 3: Listar todo o registro de dados
// Faz uma varredura sequencial no arquivo de dados e lista os registros não removidos.
void listarRegistros(char *arquivoSaidaBin);

// Funcionalidade 4: Busca por campo de referencia
// Realiza buscas indexadas (por idPessoa) ou sequenciais (outros campos) no arquivo de dados.
void buscarRegistros(char *arquivoSaidaBin, char *arquivoIndicePrimarioBin, int qtdBusca);


// ===============================================================================
// FUNCIONALIDADES - PARTE 1
// ===============================================================================

// Funcionalidade 5: Remoção Lógica de um registro de dados do arquivo pessoa.bin
// Seguindo a funcionalidade 4 (busca), encontra o registro e remove-o logicamente
void deletarRegistros(char *arquivoEntradaBin, char *arquivoIndicePrimarioBin, int qtdBusca);

// Funcionalidade 6: Inserção de Novos Registros de dados no arquivo pessoa.bin
// Insere o novo registro ao final do arquivo pessoa.bin sem reaproveitamento de espaço dos registros removidos
void inserirRegistros(char *arquivoEntradaBin, char *arquivoIndicePrimarioBin, int qtdBusca);

// Funcionalidade 7: Atualização de Registros existentes do arquivo pessoa.bin com busca do campo a ser alterado
// Busca do registro com referência ao campo, tratando o reaproveitamento de espaço da modificação dos registros
void atualizarRegistros(char *arquivoEntradaBin, char *arquivoIndicePrimarioBin, int qtdBusca);

// Funcionalidade 8: Criação de Arquivo Binário Segue
// Cria o arquivo de segue binário com o cabeçalho inicializado e com o processamento do CSV
void criarArquivoSegueBinario(char *arquivoEntradaCSV, char *arquivoSaidaBin);

// Funcionalidade 9: Ordenar o Arquivo Segue de acordo com o campo idPessoaQueSegue de forma crescente
// Leitura do arquivo segue armazenado em RAM e ordenado de forma crescente seguindo os critérios;
// ao fim, criar um novo arquivo de dados segueOrdenado
void ordenarArquivoSegue(char *arquivoSegueDesordenado, char *arquivoSegueOrdenado);

// Funcionalidade 10: Junção dos Arquivos de Dados pessoa.bin e segueOrdenado.bin 
// Relaciona-se pelo idPessoa e pelo idPessoaQueSegue para mesclar os arquivos, para retornar os registros
// de idPessoa presentes no arquivo segueOrdenado.bin comparado ao campo idPessoaQueSegue
void juncaoPessoaSegue(char *arquivoEntradaBin, char *arquivoIndicePrimarioBin, char *arquivoSegueOrdenado, int qtdBusca);


#endif