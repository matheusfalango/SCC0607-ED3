// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#include "manipularAVL.h"

// ===============================================================================
// FUNCIONALIDADES PARA MANIPALAR A ESTRUTURA DA AVL
// ===============================================================================

/*
Função de wrapper para inserir um nó na Árvore AVL.
@param arv: Ponteiro para a Árvore AVL.
@param id: ID do registro a ser inserido (chave primária).
@param bOffset: Byte Offset do registro no arquivo de dados.
*/
void insereNoAVL(ARV* arv, int id, long int bOffset) {
		insereNo(arv, id, bOffset);
}


/*
Função de remover um nó na Árvore AVL e atualizar a estrutura da árvore.
@param arv: Ponteiro para a Árvore AVL.
@param id: ID do registro a ser inserido (chave primária).
*/
void removeNoAVL(ARV* arv, int id) {
		removerNo(arv, id);
}


/*
Função para atualizar um nó da Árvore AVL.
@param arv: Ponteiro para a Árvore AVL.
@param id: ID do registro a ser atualizado (chave primária).
@param offset: Novo Byte Offset do registro no arquivo de dados.
*/
void atualizarNo(ARV* arv, int idAntigo, int idAtual, long int offsetAtual) {
		if(arv == NULL) return;

		if(idAntigo != idAtual) {
			removeNoAVL(arv, idAntigo);
			insereNoAVL(arv, idAtual, offsetAtual);

		} else {
			NO* noEncontrado = buscarNo(arv, idAntigo);
			if(noEncontrado != NULL) {
				noEncontrado->bOffset = offsetAtual;
			}

		}
}

/*
Função para percorrer a Árvore AVL em ordem crescente (inorder) e 
escrever os índices no arquivo binário.
@param raiz: Ponteiro para o nó raiz da subárvore atual.
@param indice_bin_file: Ponteiro para o arquivo binário de índice.
*/
void printCrescIndice(NO* raiz, FILE* indice_bin_file) {
		if (raiz == NULL) return;

		// Percorre os nós da esquerda
		printCrescIndice(raiz->esq, indice_bin_file);

		// Escreve o nó atual (idPessoa e byteOffset)
		fwrite(&raiz->id, sizeof(int), 1, indice_bin_file);
		fwrite(&raiz->bOffset, sizeof(long int), 1, indice_bin_file);

		// Percorre os nós da direita
		printCrescIndice(raiz->dir, indice_bin_file);
		
}

