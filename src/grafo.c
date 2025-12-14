// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#include "grafo.h"

// ===============================================================================
// FUNÇÕES AUXILIARES DO GRAFO
// ===============================================================================

/*
Criar grafo vazio com capacidade para numVertices vértices.
@param numVertices: Número de vértices do grafo.
@return: Ponteiro para o grafo criado.
*/
Grafo* criarGrafo(int numVertices) {
    Grafo *grafo = (Grafo*)malloc(sizeof(Grafo));
    if (grafo == NULL) return NULL;
    
    grafo->numVertices = 0;
    grafo->vertices = (Vertice*)malloc(numVertices * sizeof(Vertice));
    if (grafo->vertices == NULL) {
        free(grafo);
        return NULL;
    }
    
    // Inicializar vértices
    for (int i = 0; i < numVertices; i++) {
        grafo->vertices[i].nomeUsuario = NULL;
        grafo->vertices[i].listaArestas = NULL;
    }
    
    return grafo;
}

/*
Liberar memória alocada pelo grafo.
@param grafo: Ponteiro para o grafo a ser liberado.
*/
void liberarGrafo(Grafo *grafo) {
    if (grafo == NULL) return;
    
    for (int i = 0; i < grafo->numVertices; i++) {
        if (grafo->vertices[i].nomeUsuario != NULL) {
            free(grafo->vertices[i].nomeUsuario);
        }
        
        // Liberar lista de arestas
        Aresta *atual = grafo->vertices[i].listaArestas;
        while (atual != NULL) {
            Aresta *prox = atual->prox;
            if (atual->nomeUsuario != NULL) {
                free(atual->nomeUsuario);
            }
            free(atual);
            atual = prox;
        }
    }
    
    free(grafo->vertices);
    free(grafo);
}

/*
Comparar duas datas no formato DD/MM/AAAA.
@param data1: Primeira data.
@param data2: Segunda data.
@return: < 0 se data1 < data2, 0 se iguais, > 0 se data1 > data2.
*/
int compararDatas(char *data1, char *data2) {
    // Desempate por dataFimQueSegue
    int val_data1  = data_to_int(data1);				
    int val_data2  = data_to_int(data2);

    // Ultimo desempate retorna a diferença
    return val_data1 - val_data2;
}

/*
Comparar para ordenação (nome, data início, data fim).
@return: < 0 se A < B, 0 se iguais, > 0 se A > B.
*/
int compararParaOrdenacao(char *nomeA, char *dataInicioA, char *dataFimA, char *nomeB, char *dataInicioB, char *dataFimB) {
    // Primeiro ordena por nome
    int cmpNome = strcmp(nomeA, nomeB);
    if (cmpNome != 0) return cmpNome;
    
    // Em caso de empate, ordena por data de início
    int cmpDataInicio = compararDatas(dataInicioA, dataInicioB);
    if (cmpDataInicio != 0) return cmpDataInicio;
    
    // Em caso de empate, ordena por data de fim
    return compararDatas(dataFimA, dataFimB);
}

/*
Adicionar vértice ao grafo em ordem alfabética.
@param grafo: Ponteiro para o grafo.
@param nomeUsuario: Nome do usuário a ser adicionado.
@return: Índice do vértice adicionado ou encontrado.
*/
int adicionarVertice(Grafo *grafo, char *nomeUsuario) {
    // Buscar se já existe
    for (int i = 0; i < grafo->numVertices; i++) {
        if (strcmp(grafo->vertices[i].nomeUsuario, nomeUsuario) == 0) {
            return i; // Já existe
        }
    }
    
    // Adicionar novo vértice
    int idx = grafo->numVertices;
    grafo->vertices[idx].nomeUsuario = (char*)malloc((strlen(nomeUsuario) + 1) * sizeof(char));
    strcpy(grafo->vertices[idx].nomeUsuario, nomeUsuario);
    grafo->vertices[idx].listaArestas = NULL;
    grafo->numVertices++;
    
    return idx;
}

/*
Adicionar aresta ao grafo mantendo ordem alfabética na lista de arestas.
@param grafo: Ponteiro para o grafo.
@param indiceOrigem: Índice do vértice de origem.
@param nomeUsuarioDestino: Nome do usuário de destino.
@param dataInicio: Data de início.
@param dataFim: Data de fim.
@param grauAmizade: Grau de amizade.
*/
void adicionarAresta(Grafo *grafo, int indiceOrigem, char *nomeUsuarioDestino, char *dataInicio, char *dataFim, char grauAmizade) {
    // Criar nova aresta
    Aresta *nova = (Aresta*)malloc(sizeof(Aresta));
    nova->nomeUsuario = (char*)malloc((strlen(nomeUsuarioDestino) + 1) * sizeof(char));
    strcpy(nova->nomeUsuario, nomeUsuarioDestino);
    
    // Copiar datas
    if (dataInicio != NULL && dataInicio[0] != LIXO_CHAR) {
        strncpy(nova->dataInicio, dataInicio, 10);
        nova->dataInicio[10] = '\0';
    } else {
        strcpy(nova->dataInicio, "NULO");
    }
    
    if (dataFim != NULL && dataFim[0] != LIXO_CHAR) {
        strncpy(nova->dataFim, dataFim, 10);
        nova->dataFim[10] = '\0';
    } else {
        strcpy(nova->dataFim, "NULO");
    }
    
    nova->grauAmizade = grauAmizade;
    nova->prox = NULL;
    
    // Inserir na lista ordenada
    Aresta **atual = &grafo->vertices[indiceOrigem].listaArestas;
    
    while (*atual != NULL) {
        // Compara APENAS por nome (ordem alfabética)
        int cmp = strcmp(nomeUsuarioDestino, (*atual)->nomeUsuario);
        
        // Se nomes são iguais, desempata por data de início (mais antiga primeiro)
        if (cmp == 0) {
            cmp = compararDatas(nova->dataInicio, (*atual)->dataInicio);
        }
        
        // Se datas de início também são iguais, desempata por data de fim
        if (cmp == 0) {
            cmp = compararDatas(nova->dataFim, (*atual)->dataFim);
        }
        
        if (cmp < 0) break;
        
        atual = &(*atual)->prox;
    }
    
    // Inserir na posição correta
    nova->prox = *atual;
    *atual = nova;
}

/*
Buscar índice de um vértice pelo nome do usuário.
@param grafo: Ponteiro para o grafo.
@param nomeUsuario: Nome do usuário a ser buscado.
@return: Índice do vértice ou -1 se não encontrado.
*/
int buscarVertice(Grafo *grafo, char *nomeUsuario) {
    for (int i = 0; i < grafo->numVertices; i++) {
        if (strcmp(grafo->vertices[i].nomeUsuario, nomeUsuario) == 0) {
            return i;
        }
    }
    return -1;
}

/*
Imprimir grafo completo.
@param grafo: Ponteiro para o grafo.
*/
void imprimirGrafo(Grafo *grafo) {
    for (int i = 0; i < grafo->numVertices; i++) {
        Aresta *aresta = grafo->vertices[i].listaArestas;
        
        if (aresta == NULL) continue; // Pula vértices sem arestas
        
        while (aresta != NULL) {
            // Imprimir nome do vértice de origem
            printf("%s, ", grafo->vertices[i].nomeUsuario);
            
            // Imprimir nome do vértice de destino
            printf("%s, ", aresta->nomeUsuario);
            
            // Imprimir data de início
            printf("%s, ", aresta->dataInicio);
            
            // Imprimir data de fim
            printf("%s, ", aresta->dataFim);
            
            // Imprimir grau de amizade
            if (aresta->grauAmizade == LIXO_CHAR) {
                printf("NULO");
            } else {
                printf("%c", aresta->grauAmizade);
            }
            
            printf("\n");
            aresta = aresta->prox;
        }
        
        // Pular linha após cada vértice
        if (i < grafo->numVertices - 1) {
            printf("\n");
        }
    }
    printf("\n");   
}


// ===============================================================================
// FUNÇÕES AUXILIARES DE FILA (BFS)
// ===============================================================================

/*
Criar fila vazia para BFS.
@return: Ponteiro para a fila criada.
*/
Fila* criarFila() {
    Fila *fila = (Fila*)malloc(sizeof(Fila));
    fila->inicio = NULL;
    fila->fim = NULL;
    return fila;
}

/*
Enfileirar elemento na fila.
@param fila: Ponteiro para a fila.
@param indiceVertice: Índice do vértice a ser enfileirado.
*/
void enfileirar(Fila *fila, int indiceVertice) {
    NoFila *novo = (NoFila*)malloc(sizeof(NoFila));
    novo->indiceVertice = indiceVertice;
    novo->prox = NULL;
    
    if (fila->fim == NULL) {
        fila->inicio = novo;
        fila->fim = novo;
    } else {
        fila->fim->prox = novo;
        fila->fim = novo;
    }
}

/*
Desenfileirar elemento da fila.
@param fila: Ponteiro para a fila.
@return: Índice do vértice desenfileirado ou -1 se fila vazia.
*/
int desenfileirar(Fila *fila) {
    if (fila->inicio == NULL) return -1;
    
    NoFila *temp = fila->inicio;
    int indice = temp->indiceVertice;
    
    fila->inicio = fila->inicio->prox;
    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }
    
    free(temp);
    return indice;
}

/*
Verificar se fila está vazia.
@param fila: Ponteiro para a fila.
@return: 1 se vazia, 0 caso contrário.
*/
int filaVazia(Fila *fila) {
    return fila->inicio == NULL;
}

/*
Liberar memória da fila.
@param fila: Ponteiro para a fila.
*/
void liberarFila(Fila *fila) {
    while (!filaVazia(fila)) {
        desenfileirar(fila);
    }
    free(fila);
}


// ===============================================================================
// FUNÇÕES AUXILIARES PARA CONSTRUÇÃO DO GRAFO
// ===============================================================================

/*
Buscar nome de usuário por ID no arquivo de pessoas usando funções modularizadas.
@param idPessoa: ID da pessoa.
@param arvoreIndice: Árvore AVL com índice primário.
@param arquivoPessoa: Arquivo binário de pessoas.
@return: Nome de usuário (deve ser liberado) ou NULL se não encontrado.
*/
char* buscarNomeUsuarioPorID(int idPessoa, ARV *arvoreIndice, FILE *arquivoPessoa) {
    // Buscar byte offset na árvore AVL usando sua função modularizada
    long int byteOffset = setProcuradoOffset(arvoreIndice, idPessoa);
    if (byteOffset == -1) return NULL;
    
    // Posicionar no registro
    fseek(arquivoPessoa, byteOffset, SEEK_SET);
    
    // Ler registro usando sua função modularizada
    PessoaRecord *record = lerRegistroPessoa(arquivoPessoa);
    
    if (record == NULL) return NULL;
    if (record->removido == REMOVIDO_CHAR) {
        // Liberar e retornar NULL se removido
        if (record->nomePessoa != NULL) free(record->nomePessoa);
        if (record->nomeUsuario != NULL) free(record->nomeUsuario);
        free(record);
        return NULL;
    }
    
    // Copiar nomeUsuario para retornar
    char *nomeUsuario = NULL;
    if (record->nomeUsuario != NULL && record->tamanhoNomeUsuario > 0) {
        nomeUsuario = (char*)malloc((record->tamanhoNomeUsuario + 1) * sizeof(char));
        strcpy(nomeUsuario, record->nomeUsuario);
    }
    
    // Liberar registro e retornar nomeUsuario
    if (record->nomePessoa != NULL) free(record->nomePessoa);
    if (record->nomeUsuario != NULL) free(record->nomeUsuario);
    free(record);
    
    return nomeUsuario;
}

/*
Carregar todos os nomes de usuários do arquivo de pessoas usando funções modularizadas.
@param arquivoPessoa: Arquivo binário de pessoas.
@param grafo: Ponteiro para o grafo onde os vértices serão adicionados.
@return: Número de pessoas carregadas.
*/
int carregarPessoas(FILE *arquivoPessoa, Grafo *grafo) {
    // Posicionar no início dos registros
    fseek(arquivoPessoa, PESSOA_HEADER_SIZE, SEEK_SET);
    
    int count = 0;
    
    // Ler registros usando sua função modularizada
    while (1) {
        PessoaRecord *record = lerRegistroPessoa(arquivoPessoa);
        
        // Se retornou NULL, acabaram os registros
        if (record == NULL) break;
        
        // Se não está removido e tem nome de usuário, adiciona ao grafo
        if (record->removido == NAO_REMOVIDO_CHAR && 
            record->nomeUsuario != NULL && 
            record->tamanhoNomeUsuario > 0) {
            adicionarVertice(grafo, record->nomeUsuario);
            count++;
        }
        
        // Liberar memória do registro
        if (record->nomePessoa != NULL) free(record->nomePessoa);
        if (record->nomeUsuario != NULL) free(record->nomeUsuario);
        free(record);
    }
    
    return count;
}

/*
Adicionar arestas do arquivo segue ao grafo usando leitura em vetor.
@param arquivoSegue: Arquivo binário de segue ordenado.
@param grafo: Ponteiro para o grafo.
@param arvoreIndice: Árvore AVL com índice primário.
@param arquivoPessoa: Arquivo binário de pessoas.
@param transposto: 1 para grafo transposto, 0 para grafo normal.
*/
void adicionarArestasSegue(FILE *arquivoSegue, Grafo *grafo, ARV *arvoreIndice, 
                          FILE *arquivoPessoa, int transposto) {
    // Ler cabeçalho
    SegueHeader segue_header;
    lerCabecSegue(arquivoSegue, &segue_header);
    
    if (segue_header.quantidadePessoas == 0) return;
    
    // Criar vetor para guardar os registros lidos (usando função modularizada)
    SegueRecord *segue_records = (SegueRecord*)malloc(segue_header.quantidadePessoas * sizeof(SegueRecord));
    lerSegueEmVetor(arquivoSegue, segue_records, segue_header.quantidadePessoas);
    
    // Processar cada registro
    for (int i = 0; i < segue_header.quantidadePessoas; i++) {
        if (segue_records[i].removido == REMOVIDO_CHAR) continue;
        
        // Buscar nomes de usuário
        char *nomeQueSegue = buscarNomeUsuarioPorID(segue_records[i].idPessoaQueSegue, arvoreIndice, arquivoPessoa);
        char *nomeQueESeguida = buscarNomeUsuarioPorID(segue_records[i].idPessoaQueESeguida, arvoreIndice, arquivoPessoa);
        
        if (nomeQueSegue == NULL || nomeQueESeguida == NULL) {
            if (nomeQueSegue != NULL) free(nomeQueSegue);
            if (nomeQueESeguida != NULL) free(nomeQueESeguida);
            continue;
        }
        
        // Adicionar aresta
        if (transposto) {
            // Grafo transposto: quem é seguido -> quem segue
            int indiceOrigem = buscarVertice(grafo, nomeQueESeguida);
            if (indiceOrigem != -1) {
                adicionarAresta(grafo, indiceOrigem, nomeQueSegue, 
                               segue_records[i].dataInicioQueSegue, 
                               segue_records[i].dataFimQueSegue, 
                               segue_records[i].grauAmizade);
            }
        } else {
            // Grafo normal: quem segue -> quem é seguido
            int indiceOrigem = buscarVertice(grafo, nomeQueSegue);
            if (indiceOrigem != -1) {
                adicionarAresta(grafo, indiceOrigem, nomeQueESeguida, 
                               segue_records[i].dataInicioQueSegue, 
                               segue_records[i].dataFimQueSegue, 
                               segue_records[i].grauAmizade);
            }
        }
        
        // Liberar memória
        free(nomeQueSegue);
        free(nomeQueESeguida);
    }
    
    // Liberar vetor
    free(segue_records);
}

/*
Construir grafo a partir dos arquivos.
@param arquivoPessoa: Nome do arquivo de pessoas.
@param arquivoIndexaPessoa: Nome do arquivo de índice.
@param arquivoSegueOrdenado: Nome do arquivo de segue ordenado.
@param transposto: 1 para grafo transposto, 0 para grafo normal.
@return: Ponteiro para o grafo construído.
*/
Grafo* construirGrafo(char *arquivoPessoa, char *arquivoIndexaPessoa, char *arquivoSegueOrdenado, int transposto) {
    // Abrir arquivos
    FILE *pessoa_bin_file = abrirPessoa(arquivoPessoa, "rb");
    if (pessoa_bin_file == NULL) {
        return NULL;
    }
    
    FILE *indice_bin_file = abrirIndice(arquivoIndexaPessoa, "rb");
    if (indice_bin_file == NULL) {
        fclose(pessoa_bin_file);
        return NULL;
    }
    
    FILE *segue_bin_file = abrirSegue(arquivoSegueOrdenado, "rb");
    if (segue_bin_file == NULL) {
        fclose(pessoa_bin_file);
        fclose(indice_bin_file);
        return NULL;
    }
    
    // Verificar consistência dos arquivos
    PessoaHeader pessoa_header;
    lerCabecPessoa(pessoa_bin_file, &pessoa_header);
    if (verificaStatusPessoa(&pessoa_header) == 0) {
        fclose(pessoa_bin_file);
        fclose(indice_bin_file);
        fclose(segue_bin_file);
        return NULL;
    }
    
    IndexHeader indice_header;
    lerCabecIndice(indice_bin_file, &indice_header);
    if (verificaStatusIndice(&indice_header) == 0) {
        fclose(pessoa_bin_file);
        fclose(indice_bin_file);
        fclose(segue_bin_file);
        return NULL;
    }
    
    SegueHeader segue_header;
    lerCabecSegue(segue_bin_file, &segue_header);
    if (verificaStatusSegue(segue_bin_file, &segue_header) == 0) {
        fclose(pessoa_bin_file);
        fclose(indice_bin_file);
        fclose(segue_bin_file);
        return NULL;
    }
    
    // Criar grafo
    Grafo *grafo = criarGrafo(pessoa_header.quantidadePessoas);
    if (grafo == NULL) {
        fclose(pessoa_bin_file);
        fclose(indice_bin_file);
        fclose(segue_bin_file);
        return NULL;
    }
    
    // Carregar pessoas (vértices)
    carregarPessoas(pessoa_bin_file, grafo);
    
    // Ordenar vértices por nome de usuário (bubble sort simples)
    for (int i = 0; i < grafo->numVertices - 1; i++) {
        for (int j = i + 1; j < grafo->numVertices; j++) {
            if (strcmp(grafo->vertices[i].nomeUsuario, grafo->vertices[j].nomeUsuario) > 0) {
                // Trocar vértices
                Vertice temp = grafo->vertices[i];
                grafo->vertices[i] = grafo->vertices[j];
                grafo->vertices[j] = temp;
            }
        }
    }
    
    // Carregar índice na árvore AVL
    ARV *arvoreIndice = criarAVL();
    escreveIndiceArvore(indice_bin_file, arvoreIndice);
    
    // Adicionar arestas
    adicionarArestasSegue(segue_bin_file, grafo, arvoreIndice, pessoa_bin_file, transposto);
    
    // Liberar memória e fechar arquivos
    liberarAVL(arvoreIndice->raiz);
    free(arvoreIndice);
    fclose(pessoa_bin_file);
    fclose(indice_bin_file);
    fclose(segue_bin_file);
    
    return grafo;
}