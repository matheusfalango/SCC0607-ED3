// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#ifndef MANIPULAR_AVL_H
#define MANIPULAR_AVL_H

#include "ArvAVL.h"

// ===============================================================================
// FUNCIONALIDADES PARA MANIPULAR A ESTRUTURA DA AVL
// ===============================================================================

// Wrapper para insereNo da AVL.
void insereNoAVL(ARV* arv, int id, long int bOffset);

// Função de remover um nó na Árvore AVL.
void removeNoAVL(ARV* arv, int id);

// Função para atualizar um nó da AVL.
void atualizarNo(ARV* arv, int idAntigo, int idAtual, long int offsetAtual);

// Percorre a AVL em ordem crescente (inorder) e escreve os índices no arquivo binário.
void printCrescIndice(NO* raiz, FILE* indice_bin_file);


#endif