// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira


#include "lista.h"


// ===============================================================================
// FUNCIONALIDADES PARA LISTA DE REGISTROS
// ===============================================================================

/*
Implementação de criarLista: Criação de uma Lista Vazia
Cria uma lista vazia para armazenar registros de pessoa.
@param lista: Ponteiro para a lista a ser criada.
*/
Lista *criarLista() {
    Lista *lista = (Lista*) malloc(sizeof(Lista));
    if (lista == NULL) return NULL;

    lista->inicio = NULL;
    lista->tamanho = 0;

    return lista;
}

/*
Implementação de addLista: Adiciona um Registro à Lista
Adiciona um novo registro à lista de registros de pessoa.
@param lista: Ponteiro para a lista a ser criada.
@param record: Ponteiro para o registro de pessoa a ser adicionado.
*/
void addLista(Lista *lista, PessoaRecord *record) {
    if (lista == NULL || record == NULL) return;

    No *novoNo = (No*) malloc(sizeof(No));
    if (novoNo == NULL) return;

    novoNo->record = *record;
    novoNo->prox = NULL;

    if (lista->inicio == NULL) {
        lista->inicio = novoNo;
    } else {
        No *atual = lista->inicio;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novoNo;
    }

    lista->tamanho++;
}


/*
Implementação de liberarLista: Libera a Memória Alocada para a Lista
Libera a memória utilizada pela lista de registros de pessoa.
@param lista: Ponteiro para a lista a ser liberada.
*/
void liberarLista(Lista *lista) {
    if(lista == NULL) return;

    No *atual = lista->inicio;
    while (atual != NULL) {
        No *temp = atual;
        atual = atual->prox;
        free(temp);
    }

    free(lista);
}


/*
Implementação de imprimirLista: Imprime os Registros da Lista
Imprime todos os registros de pessoa armazenados na lista.
@param lista: Ponteiro para a lista a ser impressa.
*/
void imprimirLista(Lista *lista) {
    if (lista == NULL) return;

    No *atual = lista->inicio;
    while (atual != NULL) {
        printNaTela(atual->record);
        printf("\n");
        atual = atual->prox;
    }

}
