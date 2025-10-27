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
@param temp: Ponteiro para o registro de pessoa a ser adicionado.
*/
void addLista(Lista *lista, PessoaRecord *temp) {
    if (lista == NULL || temp == NULL) return;

    No *novoNo = (No*) malloc(sizeof(No));
    if (novoNo == NULL) return;

    novoNo->record = temp;
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

        // Libera a memória alocada para as strings do registro atual
        if (temp->record->nomePessoa != NULL) free(temp->record->nomePessoa);
        if (temp->record->nomeUsuario != NULL) free(temp->record->nomeUsuario);
        free(temp->record);

        // Libera a memória do nó
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
        printNaTela(*atual->record);
        printf("\n");
        atual = atual->prox;
    }

}


/*
Implementação de buscaPessoa: Busca Generalizada Para Enocntrar um Registro Específico
Busca um registro de pessoa de acordo com o campo e o valor definido.
@param pessoa_bin_file: O endereço do arquivo do pessoa.
@param campo: O campo definido para busca.
@param valor_final: O valor definido para busca.
@return: Lista com os registros
*/
Lista *buscaPessoa(FILE *pessoa_bin_file, ARV *arvoreIndice, char *campo, char *valor_final) {
    // Lista para os possíveis resultados
	Lista *lista = criarLista();
	if (lista == NULL) return NULL;

    // Inicialização do registro
    PessoaRecord *temp;
	
	// Sem campo e valor_final -> todos
	if (campo == NULL && valor_final == NULL) {
		// Ler do arquivo de dados e salvar na lista
		while (1) {
			temp = lerRegistroPessoa(pessoa_bin_file);
			if (temp == NULL) {
				// Acabou
				break;
			}
			
			addLista(lista, temp);
            lista->tamanho++;
		}

	} else if (campo != NULL && valor_final != NULL && arvoreIndice != NULL) {
        if(strcmp(campo, "idPessoa") == 0) {
            // Busca no indice gravado em memória
            long int offset = setProcuradoOffset(arvoreIndice, atoi(valor_final));

            if (offset != -1) {
                // Achar
                fseek(pessoa_bin_file, 0, SEEK_SET);
                fseek(pessoa_bin_file, offset, SEEK_SET);

                // Colocar na lista
                temp = lerRegistroPessoa(pessoa_bin_file);					
    			addLista(lista, temp);
                lista->tamanho++;
            } 
        } else {
            // Busca sequencial no arquivo de pessoas
            while(1){ //ou seja, enquanto for possivel ler os registros de dados
            	temp = lerRegistroPessoa(pessoa_bin_file);
    			if (temp == NULL) {
    				// Acabou
    				break;
    			}
            
    	        if(temp->removido == NAO_REMOVIDO_CHAR && filtroCampoPessoa(temp, campo, valor_final)){ // se os parametros foram encontrados no registro, add na lista					
    				addLista(lista, temp);
                    lista->tamanho++;
             	}
            }
        }
    }

	return lista;
}