// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#ifndef LISTA_H
#define LISTA_H

#include "pessoa.h"


// ===============================================================================
// DEFINIÇÕES PARA LISTA DE REGISTROS DE PESSOAS
// ===============================================================================

#define NAO_REMOVIDO_CHAR '0'

typedef struct No {
    PessoaRecord *record; // Registro de pessoa
    struct No *prox; // Ponteiro para o próximo nó
} No;

typedef struct {
    No *inicio; // Ponteiro para o início da lista
    int tamanho; // Número atual de registros na lista
} Lista;


// ===============================================================================
// FUNCIONALIDADES PARA LISTA DE REGISTROS DE PESSOAS
// ===============================================================================

//Implementação de criarLista: Criação de uma Lista Vazia
//Cria uma lista vazia para armazenar registros de pessoa.
Lista *criarLista();

//Implementação de addLista: Adiciona um Registro à Lista
//Adiciona um novo registro à lista de registros de pessoa. 
void addLista(Lista *lista, PessoaRecord *pessoa_record);

//Implementação de liberarLista: Libera a Memória Alocada para a Lista
//Libera a memória utilizada pela lista de registros de pessoa.
void liberarLista(Lista *lista);

// Implementação de imprimirLista: Imprime os Registros da Lista
// Imprime todos os registros de pessoa armazenados na lista.
void imprimirLista(Lista *lista);

//Implementação de buscaPessoa: Busca Generalizada Para Encontrar um Registro Específico
//Busca um registro de pessoa de acordo com o campo e o valor definido.
Lista *buscaPessoa(FILE *pessoa_bin_file, ARV *arvoreIndice, char *campo, char *valor_final);

#endif