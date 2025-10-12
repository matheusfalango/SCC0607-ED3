// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#ifndef ARVAVL_H
#define ARVAVL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// ===============================================================================
// DEFINIÇÕES
// ===============================================================================

//Definição da estrutura de um Nó da Árvore AVL
typedef struct no {
    struct no* dir; // Ponteiro para o filho à direita
    struct no* esq; // Ponteiro para o filho à esquerda
    struct no* ant; // Ponteiro para o nó pai (antecessor)
    int altura; // Altura da subárvore enraizada neste nó
    int id; // ID da pessoa (chave primária de busca)
    long int bOffset; // Byte offset do registro no arquivo de dados
} NO;

// Definição da estrutura da Árvore AVL
typedef struct arv {
    NO* raiz; // Ponteiro para o nó raiz da árvore
    int nosTaman; // Número total de nós na árvore
    int altura; // Altura da árvore
} ARV;


// ===============================================================================
// FUNCIONALIDADES
// ===============================================================================

// Funções de manipulação e construção da AVL

// Cria e inicializa a estrutura ARV.
ARV* criarAVL();

// Cria e inicializa um novo nó NO.
NO* criarNo();

// Busca um nó com um ID específico na árvore.
// @return: Ponteiro para o nó encontrado ou NULL.
NO* buscarNo(ARV* arv, int id);

// Insere um novo nó na árvore, mantendo a propriedade de AVL (balanceamento).
void insereNo(ARV* arv, int id, long int bOffset);

// Remove um nó da árvore, mantendo a propriedade de AVL.
void removerNo(ARV* arv, int id);

// Libera a memória de todos os nós da árvore recursivamente.
void liberarAVL(NO* no);

// Funções de Rotação para balanceamento

// Rotação Simples à Esquerda
void rotacEsq(ARV* arv,NO* no);

// Rotação Simples à Direita
void rotacDir(ARV* arv,NO* no);

// Rotação Dupla Direita-Esquerda (RL)
void rotacDirEsq(ARV* arv, NO* no);

// Rotação Dupla Esquerda-Direita (LR)
void rotacEsqDir(ARV* arv,NO* no);

// Verifica e aplica as rotações necessárias para balancear a subárvore a partir do nó.
void rotac(ARV* arv, NO* no);


// Funções de Altura e Balanceamento

// Calcula o fator de balanceamento do nó (altura(esq) - altura(dir)).
// @return: Fator de balanceamento.
int balancear(NO* no);

// Calcula a altura da subárvore e atualiza o campo 'altura' do nó.
// @return: Altura da subárvore.
int altura(NO* no);

// Retorna o maior entre dois inteiros. (Função auxiliar)
int maiorID (int a, int b);

#endif // ARVAVL_H