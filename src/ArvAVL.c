// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#include "ArvAVL.h"

// ===============================================================================
// FUNCIONALIDADES ÁRVORE AVL
// ===============================================================================

/*
Cria e inicializa uma nova Árvore AVL.
@return: Ponteiro para a nova estrutura ARV alocada.
*/
ARV* criarAVL() {
    ARV* arv = (ARV*) calloc(1, sizeof(ARV));
    arv->raiz = NULL; // Raiz nula
    arv->nosTaman = 0; // Tamanho inicial 0
    arv->altura = 0; // Altura inicial 0
    return (arv);
}

/*
Cria e inicializa um novo nó da Árvore AVL.
@return: Ponteiro para a nova estrutura NO alocada.
*/
NO* criarNo() {
    NO* no = (NO*) calloc(1, sizeof(NO));
    no->ant = no->dir = no->esq = NULL; // Ponteiros nulos
    no->id = 0; // ID inicial 0
    no->altura = 1; // Altura inicial 1 (como folha)
    return no;
}

/*
Busca um nó na Árvore AVL com base no ID.
@param arv: Ponteiro para a Árvore AVL.
@param id: ID (chave) a ser buscada.
@return: Ponteiro para o nó encontrado ou NULL se não for encontrado.
*/
NO* buscarNo(ARV* arv, int id) {
    NO* busca = arv->raiz;
    while (busca != NULL) {
        if (busca->id == id) { // Encontrado
            break;
        }

        if (id < busca->id) { // Busca na subárvore esquerda
            busca = busca->esq;
        } else { // Busca na subárvore direita
            busca = busca->dir;
        }
    }

    return busca;
}


/*
Realiza uma Rotação Simples à Esquerda em torno do nó `no`.
@param arv: Ponteiro para a Árvore AVL.
@param no: O nó que será rotacionado para baixo e para a esquerda.
*/
void rotacEsq(ARV* arv, NO* no) {
    
    NO *busca, *pfilho;
    // 'busca' é o filho à direita do 'no' (novo pai), 'pfilho' é o filho à esquerda de 'busca'.

    busca = no->dir;
    pfilho = busca->esq;
    busca->ant = no->ant; // O pai de 'busca' passa a ser o pai de 'no'

    // Ajusta o ponteiro do pai (se existir)
    if(no->ant!=NULL)
    {
        if(no == no->ant->esq){no->ant->esq = busca;}
        else{no->ant->dir=busca;}
    }

    // Realiza a rotação
    busca->esq = no;
    no->dir = pfilho;
    no->ant = busca;
    if(pfilho!=NULL){pfilho->ant=no;}

    // Atualiza a raiz da árvore se 'no' era a raiz
    if(no == arv->raiz){arv->raiz = busca;}

    // Recalcula a altura de todos os nós após a rotação
    arv->altura = altura(arv->raiz);
}


/*
Realiza uma Rotação Simples à Direita em torno do nó `no`.
@param arv: Ponteiro para a Árvore AVL.
@param no: O nó que será rotacionado para baixo e para a direita.
*/
void rotacDir(ARV*arv, NO* no) {
    
    NO *busca, *pfilho;
    // 'busca' é o filho à esquerda do 'no' (novo pai), 'pfilho' é o filho à direita de 'busca'.

    busca = no->esq;
    pfilho = busca->dir;
    busca->dir = no; // Realiza a rotação
    no->esq = pfilho;
    if(pfilho!=NULL){pfilho->ant=no;}
    
    busca->ant=no->ant; // O pai de 'busca' passa a ser o pai de 'no'

    // Ajusta o ponteiro do pai (se existir)
    if(no->ant!=NULL)
    {
        if(no->ant->dir == no){no->ant->dir = busca;}
        else
        {
            no->ant->esq = busca;
        }
    }
    no->ant=busca;

    // Atualiza a raiz da árvore se 'no' era a raiz
    if(no == arv->raiz){arv->raiz = busca;}

    // Recalcula a altura de todos os nós após a rotação
    arv->altura = altura(arv->raiz);
}


/*
Realiza uma Rotação Dupla: Rotação à Direita no filho à direita, seguida 
por Rotação à Esquerda no nó original.
@param arv: Ponteiro para a Árvore AVL.
@param no: O nó que está desbalanceado.
*/
void rotacDirEsq(ARV* arv, NO* no) {   
    if(no == NULL || no->dir == NULL){return;}
    rotacDir(arv,no->dir); // Rotação à Direita no filho à direita
    rotacEsq(arv,no); // Rotação à Esquerda no nó original
}


/*
Realiza uma Rotação Dupla: Rotação à Esquerda no filho à esquerda, seguida 
por Rotação à Direita no nó original.
@param arv: Ponteiro para a Árvore AVL.
@param no: O nó que está desbalanceado.
*/
void rotacEsqDir(ARV* arv, NO* no) {   
    if (no == NULL || no->esq == NULL){return;}
    rotacEsq(arv,no->esq); // Rotação à Esquerda no filho à esquerda
    rotacDir(arv,no); // Rotação à Direita no nó original
}


/*
Verifica e realiza as rotações necessárias para rebalancear a árvore 
a partir do nó 'raiz' (recursivamente).
@param arv: Ponteiro para a Árvore AVL.
@param raiz: O nó raiz da subárvore a ser balanceada.
*/
void rotac(ARV* arv, NO* raiz) {   
    if (raiz == NULL) return;

    // Varre toda a árvore (pós-ordem)
    rotac(arv,raiz->esq);
    rotac(arv,raiz->dir);

    // Calcula o fator de balanceamento do nó
    int fatorB = balancear(raiz);

    // Caso 1: Desbalanceamento à esquerda (LL ou LR)
    if (fatorB >= 2) {
        if (balancear(raiz->esq) >= 0) rotacDir(arv,raiz); // LL: Rotação Simples à Direita
        else rotacEsqDir(arv, raiz); // LR: Rotação Dupla Esquerda-Direita
    }
    
    // Caso 2: Desbalanceamento à direita (RR ou RL)
    else if(fatorB <= -2) {
        if (balancear(raiz->dir) <= 0) rotacEsq(arv,raiz); // RR: Rotação Simples à Esquerda
        else rotacDirEsq(arv,raiz); // RL: Rotação Dupla Direita-Esquerda
    }
    
    // Recalcula a altura das sub-árvores
    altura(arv->raiz);
}


/*
Insere um novo nó na Árvore AVL, mantendo a propriedade de ordenação 
e rebalanceando se necessário.
@param arv: Ponteiro para a Árvore AVL.
@param id: ID (chave) do novo nó.
@param bOffset: Byte Offset associado ao ID.
*/
void insereNo(ARV* arv,int id, long int bOffset) {
    NO* no = criarNo(); // Cria o novo nó
    no->id = id;
    no->bOffset = bOffset;
    char lastmade[7] = "\0"; // Guarda a última direção de movimento

    NO* busca = buscarNo(arv, id); // Verifica se o ID já existe
    if (busca == NULL) {
        if (arv->raiz == NULL) {
            // Caso 1: Árvore vazia
            arv->raiz = no;
            arv->raiz->altura = arv->altura = 1;
        } else {
            // Caso 2: Encontra a posição de inserção (como em uma BST)
            NO* busca = arv->raiz;
            NO* ant = busca;
            while (busca != NULL) {
                ant = busca;
                if (no->id < busca->id) {
                    busca = busca->esq;
                    strcpy(lastmade,"esq");
                } else {
                    busca = busca->dir;
                    strcpy(lastmade,"dir");
                }
            }

            // Conecta o novo nó
            if (strcmp(lastmade, "esq") == 0) {
                ant->esq = no;
            } else {
                ant->dir = no;
            }

            no->ant = ant;
            
            // Reajusta alturas e rebalanceia a árvore
            arv->altura = altura(arv->raiz);
            rotac(arv, arv->raiz);
        }
        arv->nosTaman++; // Incrementa o número de nós

    } else {
        free(no);   // Nó já existe (não pode ser inserido)
    }
}


/*
Remove um nó da Árvore AVL com base no ID, tratando os casos de 0, 1 ou 2 filhos
e rebalanceando a árvore.
@param arv: Ponteiro para a Árvore AVL.
@param id: ID (chave) do nó a ser removido.
*/
void removerNo(ARV* arv, int id) {
    NO* busca = buscarNo(arv, id); // Encontra o nó a ser removido

    if (busca == NULL) {
        printf("Registro não encontrado!\n\n");
    } else {
        // Caso 1: Nó folha (0 filhos)
        if (busca->dir == NULL && busca->esq == NULL) {
            // Se for a raiz
            if (busca == arv->raiz) {
                arv->raiz = NULL;
            } else {
                // Remove a referência do pai
                if (busca->id < busca->ant->id) {
                    busca->ant->esq = NULL;
                } else {
                    busca->ant->dir = NULL;
                }
            }
        }

        // Caso 2: Nó com dois filhos
        else if (busca->dir != NULL && busca->esq != NULL) {
            // Encontra o maior nó da subárvore esquerda (antecessor na ordem)
            NO* greateresq = busca->esq;
            while (greateresq->dir != NULL) {
                greateresq = greateresq->dir;
            }

            // A lógica a seguir reconecta os ponteiros para substituir 'busca'
            // por 'greateresq' e, em seguida, remove 'busca'.
            
            // ... (Lógica de remoção complexa que move 'greateresq' para a posição de 'busca')

            // Caso 2.1: 'busca' é a raiz
            if (busca == arv->raiz) {
                if (greateresq->ant != busca) {
                    // O antecessor tem um filho à esquerda
                    greateresq->ant->dir = greateresq->esq;
                    if (greateresq->esq != NULL) {
                        greateresq->esq->ant = greateresq->ant;
                    }
                    greateresq->esq = busca->esq;
                    busca->esq->ant = greateresq;
                    greateresq->dir = busca->dir;
                    busca->dir->ant = greateresq;
                } else {
                    // O antecessor é filho direto de 'busca'
                    greateresq->dir = busca->dir;
                    busca->dir->ant = greateresq;
                }

                greateresq->ant = NULL;
                arv->raiz = greateresq;
            } 
            
            // Caso 2.2: 'busca' não é a raiz
            else {
                if (greateresq->ant != busca) {
                    // O antecessor tem um filho à esquerda
                    greateresq->ant->dir = greateresq->esq;
                    if (greateresq->esq != NULL) {
                        greateresq->esq->ant = greateresq->ant;
                    }
                    // Ajusta ponteiros do pai e filhos de 'busca' para 'greateresq'
                    greateresq->ant = busca->ant;
                    if (busca == busca->ant->esq) {
                        busca->ant->esq = greateresq;
                    } else {
                        busca->ant->dir = greateresq;
                    }
                    greateresq->dir = busca->dir;
                    busca->dir->ant = greateresq;
                    greateresq->esq = busca->esq;
                    busca->esq->ant = greateresq;
                } else if (greateresq->ant == busca) {
                    // O antecessor é filho direto de 'busca'
                    greateresq->ant = busca->ant;
                    if (busca == busca->ant->esq) {
                        busca->ant->esq = greateresq;
                    } else {
                        busca->ant->dir = greateresq;
                    }
                    greateresq->dir = busca->dir;
                    busca->dir->ant = greateresq;
                }
                
            }

            // Limpa ponteiros do nó removido
            busca->esq = busca->dir = busca->ant = NULL;

        }

        // Caso 3: Nó com um filho à direita
        else if (busca->dir != NULL && busca->esq == NULL)
        {
            if (busca == arv->raiz) {
                arv->raiz = busca->dir; // O filho se torna a nova raiz
                busca->dir->ant = NULL;
            } else {
                // Liga o filho ao avô
                busca->dir->ant = busca->ant;
                if (busca->dir->id < busca->ant->id) {
                    busca->ant->esq = busca->dir;
                } else {
                    busca->ant->dir = busca->dir;
                }
            }
            // Limpa ponteiros do nó removido
            busca->ant = NULL;
            busca->dir = NULL;
        }

        // Caso 4: Nó com um filho à esquerda
        else if (busca->esq!=NULL && busca->dir == NULL) {
            if(busca == arv->raiz) {
                arv->raiz = busca->esq; // O filho se torna a nova raiz
                busca->esq->ant = NULL;
            } else {
                // Liga o filho ao avô
                busca->esq->ant = busca->ant;
                if (busca->esq->id < busca->ant->id) {
                    busca->ant->esq = busca->esq;
                } else {
                    busca->ant->dir = busca->esq;
                }
            }
            // Limpa ponteiros do nó removido
            busca->ant = NULL;
            busca->esq = NULL;
        }

        free(busca); // Libera a memória do nó removido
        
        // Reajusta alturas e rebalanceia a árvore
        altura(arv->raiz);
        rotac(arv,arv->raiz);
    }
}

/*
Função recursiva para liberar a memória de todos os nós da árvore.
@param no: Ponteiro para o nó raiz da subárvore a ser liberada.
*/
void liberarAVL(NO* no) {
    /* Liberar todos os nós da árvore (percurso pós-ordem) */
    if (no == NULL) {
        return;
    }

    liberarAVL(no->esq);
    liberarAVL(no->dir);
    free(no);
}


/*
Calcula o fator de balanceamento de um nó.
Fator B = altura(esq) - altura(dir)
@param no: Ponteiro para o nó.
@return: Fator de balanceamento do nó.
*/
int balancear(NO* no) {   
    if(no == NULL) return 0; // Se o nó é nulo, retorna 0 (apesar do -1 ser usado internamente no código, 0 é o mais correto para o fator B)
    else {
        NO* esq = no->esq;
        NO* dir = no->dir;
        
        // Calcula a altura dos filhos (0 se for NULL)
        int altura_esq = (esq == NULL) ? 0 : esq->altura;
        int altura_dir = (dir == NULL) ? 0 : dir->altura;

        return (altura_esq - altura_dir); // Retorna a diferença
    }
}


/*
Calcula e atualiza a altura de um nó de forma recursiva.
@param no: Ponteiro para o nó raiz da subárvore.
@return: Altura da subárvore.
*/
int altura(NO* no) {
    if(no == NULL){return 0;} // Altura 0 para nó nulo
    
    else
    {
        // Altura do nó = 1 + max(altura_esq, altura_dir)
        int esq = 1 + altura(no->esq);
        int dir = 1 + altura(no->dir);
        
        if(esq>dir)
        {
            no->altura = esq;
            return(esq);
        }
        else
        {
            no->altura = dir;
            return(dir);
        }
    }
}


/*
Função auxiliar para retornar o maior entre dois inteiros.
@param a: Primeiro inteiro.
@param b: Segundo inteiro.
@return: O maior valor entre a e b.
*/
int maiorID(int a, int b) {
    return (a>b)? a: b;
}

