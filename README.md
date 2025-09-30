# Trabalho Introdutório - Organização de Arquivos (SCC0215)

Este documento descreve a implementação de um programa em C para gerenciar arquivos de dados e índices primários, conforme as especificações do trabalho introdutório da disciplina de Organização de Arquivos (SCC0215) da USP.

## Objetivo

O objetivo principal deste trabalho é desenvolver um programa em C que permita:

1.  Obter dados de um arquivo de entrada CSV.
2.  Gerar um arquivo binário de dados (`pessoa.bin`) com base nesses dados.
3.  Gerar um arquivo binário de índice primário (`indexaPessoa.bin`) para o arquivo de dados.
4.  Realizar operações de busca nos arquivos binários, com e sem o uso do índice primário.

## Estrutura dos Arquivos

### Arquivo de Dados Pessoa (`pessoa.bin`)

O arquivo `pessoa.bin` armazena registros de pessoas. Ele é composto por um registro de cabeçalho e zero ou mais registros de dados.

#### Registro de Cabeçalho

-   **Tamanho:** 17 bytes
-   **Campos:**
    -   `status` (1 byte, string '0' ou '1'): Indica a consistência do arquivo.
    -   `quantidadePessoas` (4 bytes, inteiro): Número de registros de pessoas não removidos.
    -   `quantidadeRemovidos` (4 bytes, inteiro): Número de registros logicamente removidos.
    -   `proxByteOffset` (8 bytes, inteiro): Próximo byte offset disponível para inserção.

#### Registros de Dados

Registros de tamanho variável, com campos de tamanho fixo e variável. O método de indicador de tamanho é usado para campos variáveis.

-   **Campos Fixos:**
    -   `removido` (1 byte, string '0' ou '1'): Indica se o registro está logicamente removido.
    -   `tamanhoRegistro` (4 bytes, inteiro): Número total de bytes do registro.
    -   `idPessoa` (4 bytes, inteiro): Código sequencial único da pessoa (chave primária).
    -   `idadePessoa` (4 bytes, inteiro): Idade da pessoa.
    -   `tamanhoNomePessoa` (4 bytes, inteiro): Tamanho do campo `nomePessoa`.
    -   `tamanhoNomeUsuario` (4 bytes, inteiro): Tamanho do campo `nomeUsuario`.

-   **Campos Variáveis:**
    -   `nomePessoa` (string): Nome completo da pessoa.
    -   `nomeUsuario` (string): Nome de usuário na rede social.

-   **Observações:**
    -   Valores nulos em campos inteiros são representados por -1.
    -   Valores nulos em campos string são preenchidos com '$'.
    -   Campos variáveis nulos têm seu indicador de tamanho como zero.
    -   Lixo em registros é preenchido com '$'.

### Arquivo de Índice Primário (`indexaPessoa.bin`)

O arquivo `indexaPessoa.bin` é um índice primário sobre o campo `idPessoa` do arquivo `pessoa.bin`.

#### Registro de Cabeçalho

-   **Tamanho:** 12 bytes
-   **Campos:**
    -   `status` (1 byte, string '0' ou '1'): Indica a consistência do arquivo.
    -   Lixo (11 bytes, '$'): Preenchimento para completar 12 bytes.

#### Registros de Dados

Registros de tamanho fixo.

-   **Tamanho:** 8 bytes
-   **Campos:**
    -   `idPessoa` (4 bytes, inteiro): Chave primária do registro de pessoa.
    -   `byteOffset` (8 bytes, inteiro): Byte offset do registro correspondente em `pessoa.bin`.

-   **Observações:**
    -   Os valores de `idPessoa` devem ser ordenados de forma crescente.

## Funcionalidades do Programa

O programa (`programaTrab`) deve ser executado via linha de comando e oferecer as seguintes funcionalidades, identificadas por um número:

### [1] Criação de Arquivo de Índice Primário (`CREATE INDEX`)

Cria um arquivo de índice primário vazio (`indexaPessoa.bin`) com o registro de cabeçalho inicializado.

-   **Entrada:** `1 <arquivoIndicePrimario.bin>`
-   **Saída:** Conteúdo do arquivo de índice primário binário usando `binarioNaTela`.
-   **Erro:** `Falha no processamento do arquivo.`

### [2] Leitura de CSV e Geração de Arquivo de Dados e Índice (`CREATE TABLE`)

Lê registros de um arquivo CSV, insere-os no arquivo de dados (`pessoa.bin`) e atualiza o arquivo de índice primário (`indexaPessoa.bin`).

-   **Entrada:** `2 <arquivoEntrada.csv> <arquivoSaida.bin> <arquivoIndicePrimario.bin>`
-   **Saída:** Conteúdo dos arquivos `pessoa.bin` e `indexaPessoa.bin` usando `binarioNaTela`.
-   **Erro:** `Falha no processamento do arquivo.`

### [3] Busca por `idPessoa` (`SELECT` com índice)

Busca um registro no arquivo de dados `pessoa.bin` usando o `idPessoa` e o arquivo de índice `indexaPessoa.bin`.

-   **Entrada:** `3 <arquivoSaida.bin> <arquivoIndicePrimario.bin> <idPessoa>`
-   **Saída:** Dados do registro encontrado ou `Registro inexistente.`
-   **Erro:** `Falha no processamento do arquivo.`

### [4] Busca por `idPessoa` (sem índice)

Busca um registro no arquivo de dados `pessoa.bin` usando o `idPessoa` sem o arquivo de índice.

-   **Entrada:** `4 <arquivoSaida.bin> <idPessoa>`
-   **Saída:** Dados do registro encontrado ou `Registro inexistente.`
-   **Erro:** `Falha no processamento do arquivo.`

### [5] Busca por `nomePessoa` (sem índice)

Busca registros no arquivo de dados `pessoa.bin` usando o `nomePessoa` sem o arquivo de índice.

-   **Entrada:** `5 <arquivoSaida.bin> <nomePessoa>`
-   **Saída:** Dados dos registros encontrados ou `Registro inexistente.`
-   **Erro:** `Falha no processamento do arquivo.`

## Modularização

O código deve ser modularizado em funções e procedimentos para reutilização e organização.

## Funções Auxiliares

A função `binarioNaTela` será fornecida para visualização do conteúdo binário dos arquivos.
