// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#ifndef MANIPULAR_AVL_H
#define MANIPULAR_AVL_H

#include "ArvAVL.h"

// ===============================================================================
// FUNCIONALIDADES PARA MANIPULAR A ESTRUTURA DA AVL
// ===============================================================================

// Wrapper para insereNo da AVL.
void atualizaAVL(ARV* arv, int id, long int bOffset);

// Percorre a AVL em ordem crescente (inorder) e escreve os índices no arquivo binário.
void printCrescIndice(NO* raiz, FILE* indice_bin_file);


#endif