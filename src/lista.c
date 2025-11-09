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
        printNaTelaPessoa(*atual->record);
        printf("\n");
        atual = atual->prox;
    }

}


/*
Implementação de deletarPessoaDaLista: Remove os Registros da Lista
Remove os registros de pessoa da lista que possui os registros de acordo com a busca
referenciada pelo campo.
@param pessoa_bin_file: O endereço do arquivo do pessoa.
@param arvoreIndice: O arquivo índice em memória pela estrutura da árvore AVL.
@param lista: Ponteiro para a lista de registros a serem deletados.
*/
void deletarPessoaDaLista(FILE *pessoa_bin_file, ARV *arvoreIndice, Lista *lista) {
    if (lista == NULL || arvoreIndice == NULL) return;
    
    No *atual = lista->inicio;
    long int atualOffset;
    int atualId;

    while(atual != NULL) {
        // Atualiza id e offset do registro atual
        PessoaRecord *atualRecord = atual->record;
        atualId = atualRecord->idPessoa;
        atualOffset = setProcuradoOffset(arvoreIndice, atualId);

        // Remove o nó do registro na AVL
        removeNoAVL(arvoreIndice, atualId);
        
        // Remove logicamente o registro no arquivo pessoa
        atualRecord->removido = REMOVIDO_CHAR;
        fseek(pessoa_bin_file, atualOffset, SEEK_SET);
        fwrite(&atualRecord->removido, sizeof(char), 1, pessoa_bin_file);
        
        atual = atual->prox;
    }
}

/*
Implementação de atualizaPessoa: Atualiza os Registros da Lista
Atualiza os registros de pessoa da lista.
@param pessoa_bin_file: O endereço do arquivo do pessoa.
@param arvoreIndice: O arquivo índice em memória pela estrutura da árvore AVL.
@param campo1: O campo definido para busca.
@param valor1: O valor definido para busca.
@param campo2: O campo definido para atualização.
@param valor2: O valor definido para atualização.
@return: Inteiro indicando a quantidade de registros removidos durante a atualização.
*/
int atualizaPessoa(FILE *pessoa_bin_file, ARV *arvoreIndice, Lista *lista, char *campo2, char *valor2) {
    if (arvoreIndice == NULL || lista == NULL) return 0;

    int quantidadeRemovidos = 0;

    No *atual = lista->inicio;
    PessoaRecord *atualRecord;
    
    long int atualOffset;
    char atualRemovido;
    int atualTamanhoRegistro, atualTamanhoNomePessoa, atualTamanhoNomeUsuario; // variaveis cte do registro
    int tamanhoValor2 = strlen(valor2); // variavel para determinar o tamanho da string

    while(atual != NULL) {
        // Atualiza os campos localmente e offset do registro atual

        atualRecord = atual->record;
        if(atualRecord == NULL) continue;

        atualRemovido = atualRecord->removido;
        if(atualRemovido == REMOVIDO_CHAR) continue;

        atualTamanhoRegistro = atualRecord->tamanhoRegistro;
        atualTamanhoNomePessoa = atualRecord->tamanhoNomePessoa;
        atualTamanhoNomeUsuario = atualRecord->tamanhoNomeUsuario;
        atualOffset = setProcuradoOffset(arvoreIndice, atualRecord->idPessoa);

        // Busca o campo2 para ser atualizado
        if (strcmp(campo2, "idPessoa") == 0) {
            int antigoId = atualRecord->idPessoa;

            if(strcmp(valor2, "NULO") == 0) atualRecord->idPessoa = -1;
            else atualRecord->idPessoa = atoi(valor2);

            atualizarNo(arvoreIndice, antigoId, atualRecord->idPessoa, atualOffset);
            
            fseek(pessoa_bin_file, atualOffset + sizeof(char) + sizeof(int), SEEK_SET);
            fwrite(&atualRecord->idPessoa, sizeof(int), 1, pessoa_bin_file);

        } else if (strcmp(campo2, "idadePessoa") == 0) {
            if(strcmp(valor2, "NULO") == 0) atualRecord->idadePessoa = -1;
            else atualRecord->idadePessoa = atoi(valor2);
    
            fseek(pessoa_bin_file, atualOffset + sizeof(char) + 2*sizeof(int), SEEK_SET);
            fwrite(&atualRecord->idadePessoa, sizeof(int), 1, pessoa_bin_file);

        } else if (strcmp(campo2, "nomePessoa") == 0) {
            if(16 + tamanhoValor2 + atualTamanhoNomeUsuario <= atualTamanhoRegistro && strcmp(valor2, "NULO") != 0) {
                if(atualRecord->nomePessoa != NULL) free(atualRecord->nomePessoa);
                atualRecord->nomePessoa = (char *) malloc(tamanhoValor2 + 1);
                strcpy(atualRecord->nomePessoa, valor2);
                atualRecord->tamanhoNomePessoa = tamanhoValor2;

                fseek(pessoa_bin_file, atualOffset + sizeof(char) + 3*sizeof(int), SEEK_SET);
                fwrite(&atualRecord->tamanhoNomePessoa, sizeof(int), 1, pessoa_bin_file);
                fwrite(atualRecord->nomePessoa, sizeof(char), tamanhoValor2, pessoa_bin_file);
                
                for(int i = tamanhoValor2; i < atualTamanhoNomePessoa; i++) fputc(LIXO_CHAR, pessoa_bin_file);

            } else if (16 + tamanhoValor2 + atualTamanhoNomeUsuario > atualTamanhoRegistro && strcmp(valor2, "NULO") != 0) {
                fseek(pessoa_bin_file, atualOffset, SEEK_SET); 
                atualRemovido = REMOVIDO_CHAR;
                fwrite(&atualRemovido, sizeof(char), 1, pessoa_bin_file);
                quantidadeRemovidos++;
                
                PessoaRecord *copiaRecord = (PessoaRecord *) malloc(sizeof(PessoaRecord));
                copiarPessoaRecord(copiaRecord, atualRecord);
                copiaRecord->removido = NAO_REMOVIDO_CHAR;
                copiaRecord->tamanhoRegistro = 16 + tamanhoValor2 + atualTamanhoNomeUsuario;

                if(copiaRecord->nomePessoa != NULL) free(copiaRecord->nomePessoa);
                copiaRecord->nomePessoa = (char *) malloc(tamanhoValor2 + 1);
                strcpy(copiaRecord->nomePessoa, valor2);
                copiaRecord->tamanhoNomePessoa = tamanhoValor2;

                fseek(pessoa_bin_file, 0, SEEK_END);
                atualOffset = ftell(pessoa_bin_file);
                escrevePessoaRecord(pessoa_bin_file, copiaRecord);
                
                atualizarNo(arvoreIndice, copiaRecord->idPessoa, copiaRecord->idPessoa, atualOffset);
            
            } else if (strcmp(valor2, "NULO") == 0) {
                atualRecord->tamanhoNomePessoa = 0;
                if (atualRecord->nomePessoa != NULL) {
                    free(atualRecord->nomePessoa); 
                    atualRecord->nomePessoa = NULL;
                }

                long int offsetLixo = sizeof(char) + 4*sizeof(int);
                fseek(pessoa_bin_file, atualOffset + offsetLixo, SEEK_SET);
                for(int i = 0; i < atualTamanhoNomePessoa; i++) fputc(LIXO_CHAR, pessoa_bin_file);
            
            }

        } else if (strcmp(campo2, "nomeUsuario") == 0) {
            if(16 + tamanhoValor2 + atualTamanhoNomePessoa <= atualTamanhoRegistro && strcmp(valor2, "NULO") != 0) {
                if(atualRecord->nomeUsuario != NULL) free(atualRecord->nomeUsuario);
                atualRecord->nomeUsuario = (char *) malloc(tamanhoValor2 + 1);
                strcpy(atualRecord->nomeUsuario, valor2);
                atualRecord->tamanhoNomeUsuario = tamanhoValor2;

                long int offsetEscrita = sizeof(char) + 4*sizeof(int);
                if (atualTamanhoNomePessoa > 0) offsetEscrita += atualTamanhoNomePessoa;
                fseek(pessoa_bin_file, atualOffset + offsetEscrita, SEEK_SET);
                fwrite(&atualRecord->tamanhoNomeUsuario, sizeof(int), 1, pessoa_bin_file);
                fwrite(atualRecord->nomeUsuario, sizeof(char), tamanhoValor2, pessoa_bin_file);
                
                for(int i = tamanhoValor2; i < atualTamanhoNomeUsuario; i++) fputc(LIXO_CHAR, pessoa_bin_file);

            } else if (16 + tamanhoValor2 + atualTamanhoNomePessoa > atualTamanhoRegistro && strcmp(valor2, "NULO") != 0) {
                fseek(pessoa_bin_file, atualOffset, SEEK_SET);
                atualRemovido = '1';
                fwrite(&atualRemovido, sizeof(char), 1, pessoa_bin_file);
                quantidadeRemovidos++;
                
                PessoaRecord *copiaRecord = (PessoaRecord *) malloc(sizeof(PessoaRecord));
                copiarPessoaRecord(copiaRecord, atualRecord);
                copiaRecord->removido = NAO_REMOVIDO_CHAR;
                copiaRecord->tamanhoRegistro = 16 + tamanhoValor2 + atualTamanhoNomePessoa;

                if(copiaRecord->nomeUsuario != NULL) free(copiaRecord->nomeUsuario);
                copiaRecord->nomeUsuario = (char *) malloc(tamanhoValor2 + 1);
                strcpy(copiaRecord->nomeUsuario, valor2);
                copiaRecord->tamanhoNomePessoa = tamanhoValor2;

                fseek(pessoa_bin_file, 0, SEEK_END);
                atualOffset = ftell(pessoa_bin_file);
                escrevePessoaRecord(pessoa_bin_file, copiaRecord);

                atualizarNo(arvoreIndice, copiaRecord->idPessoa, copiaRecord->idPessoa, atualOffset);
            
            } else if (strcmp(valor2, "NULO") == 0) {
                atualRecord->tamanhoNomeUsuario = 0;
                if (atualRecord->nomeUsuario != NULL) {
                    free(atualRecord->nomeUsuario); 
                    atualRecord->nomeUsuario = NULL;
                }

                long int offsetLixo = sizeof(char) + 4*sizeof(int);
                fseek(pessoa_bin_file, atualOffset + offsetLixo,SEEK_SET);
                for(int i = 0; i < atualTamanhoNomeUsuario; i++) fputc(LIXO_CHAR, pessoa_bin_file);
            
            }
        }

        atual = atual->prox;
    }

    return quantidadeRemovidos;
}


/*
Implementação de buscaPessoa: Busca Generalizada Para Enocntrar um Registro Específico
Busca um registro de pessoa de acordo com o campo e o valor definido; busca sequencial para ler todos os registros
e para encontrar os campos específicos, exceto para idPessoa que utiliza-se busca binária.
@param pessoa_bin_file: O endereço do arquivo do pessoa.
@param arvoreIndice: O arquivo índice em memória pela estrutura da árvore AVL.
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
		}

	} else if (campo != NULL && valor_final != NULL && arvoreIndice != NULL) {
        if(strcmp(campo, "idPessoa") == 0) {
            // Busca no indice gravado em memória - busca binaria que percorre a arvore
            long int offset = setProcuradoOffset(arvoreIndice, atoi(valor_final));

            if (offset != -1) {
                // Achar
                fseek(pessoa_bin_file, 0, SEEK_SET);
                fseek(pessoa_bin_file, offset, SEEK_SET);

                // Colocar na lista
                temp = lerRegistroPessoa(pessoa_bin_file);					
    			addLista(lista, temp);
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
                    if (strcmp(campo, "nomeUsuario") == 0) break; // sao unicos
             	}
            }
        }
    }

	return lista;
}

