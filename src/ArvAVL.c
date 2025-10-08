// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#include "ArvAVL.h"
#define HIGH 1

ARV* criarAVL() {
    ARV* arv = (ARV*) calloc(1, sizeof(ARV));
    arv->raiz = NULL;
    arv->nosTaman = 0;
    arv->altura = 0;
    return (arv);
}


NO* criarNo() {
    NO* no = (NO*) calloc(1, sizeof(NO));
    no->ant = no->dir = no->esq = NULL;
    no->id = 0;
    no->altura = 1;
    return no;
}

NO* buscarNo(ARV* arv, int id) {
    NO* busca = arv->raiz;
    while (busca != NULL) {
        if (busca->id == id) {
            break;
        }

        if (id < busca->id) {
            busca = busca->esq;
        } else {
            busca = busca->dir;
        }
    }

    return busca;
}


void rotacEsq(ARV* arv, NO* no) {
    
    NO *busca, *pfilho;
    // busca == Nó imediatamente à direita do nó a ser rotado, pfilho == filho à esquerda do nó busca (pode ser NULL).

    busca = no->dir;
    pfilho = busca->esq;
    busca->ant = no->ant;

    //Caso o nó não seja a raíz da árvore
    if(no->ant!=NULL)
    {
        if(no == no->ant->esq){no->ant->esq = busca;}
        else{no->ant->dir=busca;}
    }

    busca->esq = no;
    no->dir = pfilho;
    no->ant = busca;
    if(pfilho!=NULL){pfilho->ant=no;}

    //Atualiza a raíz da árvore
    if(no == arv->raiz){arv->raiz = busca;}

    //Recacula a altura de todos os nós da árvore após a rotação
    arv->altura = altura(arv->raiz);
}


void rotacDir(ARV*arv, NO* no) {
    
    NO *busca, *pfilho;
    //busca == Filho imediatamende à esquerda do nó a ser rotacionado; pfilho == Filho à direita de busca (pode ser NULL)

    busca = no->esq;
    pfilho = busca->dir;
    busca->dir = no;
    no->esq = pfilho;
    if(pfilho!=NULL){pfilho->ant=no;}
    busca->ant=no->ant;

    //Caso não seja raíz
    if(no->ant!=NULL)
    {
        if(no->ant->dir == no){no->ant->dir = busca;}
        else
        {
            no->ant->esq = busca;
        }
    }
    no->ant=busca;

    //Atualiza a raíz da árvore
    if(no == arv->raiz){arv->raiz = busca;}

    //Recalcula a altura de todos os nós da árvore apó a rotação
    arv->altura = altura(arv->raiz);
}


void rotacDirEsq(ARV* arv, NO* no) {   
    if(no == NULL || no->dir == NULL){return;}
    rotacDir(arv,no->dir);
    rotacEsq(arv,no);
}


void rotacEsqDir(ARV* arv, NO* no) {   
    if (no == NULL || no->esq == NULL){return;}
    rotacEsq(arv,no->esq);
    rotacDir(arv,no);
}


void rotac(ARV* arv, NO* raiz) {   
    if (raiz == NULL) return;

    //Varre toda a árvore
    rotac(arv,raiz->esq);
    rotac(arv,raiz->dir);

    //Calcula o fato de balanceamento do nó
    int fatorB = balancear(raiz);

    //Caso a árvore só esteja desbalanceada à esquerda
    if (fatorB >= 2 && balancear(raiz->esq)>=0) rotacDir(arv,raiz);

    //Caso a árvore só esteja desbalanceada à direita
    else if(fatorB <= -2 && balancear(raiz->dir) <= 0) rotacEsq(arv,raiz);
    

    //Caso a árvore esteja desbalanceada à esquerda e a sub-árvore esteja desbalanceada para a direita
    else if (fatorB >= 2 && balancear(raiz->esq) <=0) rotacEsqDir(arv, raiz);

    //Caso a árvore esteja desbalanceada à direita e a sub-árvore esteja desbalanceada à esquerda
    else if (fatorB <= -2 && balancear(raiz->dir) >=0) rotacDirEsq(arv,raiz);

    //Recalcula a altura das sub-árvores
    altura(arv->raiz);
}


void insereNo(ARV* arv,int id, long long bOffset) {
    NO* no = criarNo();
    no->id = id;
    no->bOffset = bOffset;
    char lastmade[7] = "\0";

    NO* busca = buscarNo(arv, id);
    if (busca == NULL) {
        if (arv->raiz == NULL) {
            arv->raiz = no;
            arv->raiz->altura = arv->altura = 1;
        } else {
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

            if (strcmp(lastmade, "esq") == 0) {
                ant->esq = no;
            } else {
                ant->dir = no;
            }

            no->ant = ant;
            arv->altura = altura(arv->raiz);
            rotac(arv, arv->raiz);
        }
        arv->nosTaman++;

    } else {
        printf("Nó já foi inserido anteriormente.\n");
    }
}


void printInOrder(FILE* indice_bin_file, NO* no, int nivel) {
    /* Imprime a árvore em ordem crescente no arquivo binario de índice */
    fseek(indice_bin_file, 0, SEEK_END);
    if (no != NULL) {
        printInOrder(indice_bin_file, no->esq, nivel + 1);
        fwrite(&no->id, sizeof(int), 1, indice_bin_file);
        fwrite(&no->bOffset, sizeof(long long), 1, indice_bin_file);
        printInOrder(indice_bin_file, no->dir, nivel + 1);
    }
}


void removerNo(ARV* arv, int id) {
    NO* busca = buscarNo(arv, id);

    if (busca == NULL) {
        printf("Registro não encontrado!\n\n");
    } else {
        //Caso seja folha
        if (busca->dir == NULL && busca->esq == NULL) {
            if (busca == arv->raiz) {
                arv->raiz = NULL;
            } else {
                if (busca->id < busca->ant->id) {
                    busca->ant->esq = NULL;
                    busca->ant = NULL;
                } else {
                    busca->ant->dir = NULL;
                    busca->ant = NULL;
                }
            }
        }

        //Caso tenha dois filhos
        else if (busca->dir != NULL && busca->esq != NULL) {
            NO* greateresq = busca->esq;

            while (greateresq->dir != NULL) {
                greateresq = greateresq->dir;
            }

            if (busca == arv->raiz) {
                if (greateresq->ant != busca) {
                    greateresq->ant->dir = greateresq->esq;
                    if (greateresq->esq != NULL) {
                        greateresq->esq->ant = greateresq->ant;
                    }
                    greateresq->esq = busca->esq;
                    busca->esq->ant = greateresq;
                    greateresq->dir = busca->dir;
                    busca->dir->ant = greateresq;
                } else {
                    greateresq->dir = busca->dir;
                    busca->dir->ant = greateresq;
                }

                greateresq->ant = NULL;
                arv->raiz = greateresq;
            } else {
                if (greateresq->ant != busca) {
                    greateresq->ant->dir = greateresq->esq;
                    if (greateresq->esq != NULL) {
                        greateresq->esq->ant = greateresq->ant;
                    }
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

            busca->esq = busca->dir = busca->ant = NULL;

        }

        //Caso tenha um filho à direita
        else if (busca->dir != NULL && busca->esq == NULL)
        {
            if (busca == arv->raiz) {
                arv->raiz = busca->dir;
                busca->dir->ant = NULL;
                busca->dir = NULL;
            } else {
                busca->dir->ant = busca->ant;
                if (busca->dir->id < busca->ant->id) {
                    busca->ant->esq = busca->dir;
                } else {
                    busca->ant->dir = busca->dir;
                }
                busca->ant = NULL;
                busca->dir = NULL;
            }
        }

        //Caso tenha um filho à esquerda
        else if (busca->esq!=NULL && busca->dir == NULL) {
            if(busca == arv->raiz) {
                arv->raiz = busca->esq;
                busca->esq->ant = NULL;
                busca->esq = NULL;
            } else {
                busca->esq->ant = busca->ant;
                if (busca->esq->id < busca->ant->id) {
                    busca->ant->esq = busca->esq;
                } else {
                    busca->ant->dir = busca->esq;
                }
                busca->ant = NULL;
                busca->esq = NULL;
            }
        }

        free(busca);
        altura(arv->raiz);
        rotac(arv,arv->raiz);
    }
}

void liberarAVL(NO* no) {
    /* Liberar todos os nós da árvore */
    if (no == NULL) {
        return;
    }

    liberarAVL(no->esq);
    liberarAVL(no->dir);
    free(no);
}


int balancear(NO* no) {   
    if(no == NULL) return -1;
    else {
        NO* esq = no->esq;
        NO* dir = no->dir;
        if(esq == NULL && dir != NULL) return (0 - dir->altura);
        else if(esq !=  NULL && dir == NULL) return (esq->altura);
        else if(esq == NULL && dir == NULL) return (0);
        else if(esq != NULL && dir != NULL) return (esq->altura - dir->altura);
        else return 0;
    }
}


int altura(NO* no) {
    if(no == NULL){return 0;}
    
    else
    {
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


int maiorID(int a, int b) {
    return (a>b)? a: b;
} 

