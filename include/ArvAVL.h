// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#ifndef ARVAVL_H
#define ARVAVL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Definição dos tipos de dados
typedef struct no {
    struct no* dir;
    struct no* esq;
    struct no* ant;
    int altura;
    int id;
    long long int bOffset;
} NO;

typedef struct arv {
    NO* raiz;
    int nosTaman;
    int altura;
} ARV;

//Funções

ARV* criarAVL();
NO* criarNo();
NO* buscarNo(ARV* arv, int id);

void insereNo(ARV* arv, int id, long long int bOffset);
void removerNo(ARV* arv, int id);
void liberarAVL(NO* no);

void rotacEsq(ARV* arv,NO* no);
void rotacDir(ARV* arv,NO* no);
void rotacDirEsq(ARV* arv, NO* no);
void rotacEsqDir(ARV* arv,NO* no);
void rotac(ARV* arv, NO* no);


int balancear(NO* no);
int altura(NO* no);
int maiorID (int a, int b);

#endif // ARVAVL_H