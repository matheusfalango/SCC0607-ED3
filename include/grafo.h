// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"
#include "utilidades.h"
#include "tratarString.h"
#include "manipularAVL.h"
#include "ArvAVL.h"
#include "lista.h"

// ===============================================================================
// ESTRUTURAS DE DADOS DO GRAFO
// ===============================================================================

// Estrutura de uma aresta na lista de adjacências
typedef struct Aresta {
    char *nomeUsuario;              // Nome do usuário (destino/origem conforme o grafo)
    char dataInicio[11];            // Data de início (formato: DD/MM/AAAA)
    char dataFim[11];               // Data de fim (ou NULO)
    char grauAmizade;               // Grau de amizade (ou NULO)
    struct Aresta *prox;            // Próximo elemento da lista
} Aresta;

// Estrutura de um vértice (elemento do vetor de adjacências)
typedef struct Vertice {
    char *nomeUsuario;              // Nome do usuário (chave do vértice)
    Aresta *listaArestas;           // Lista encadeada de arestas adjacentes
} Vertice;

// Estrutura do grafo (vetor de vértices)
typedef struct Grafo {
    Vertice *vertices;              // Vetor de vértices
    int numVertices;                // Número de vértices no grafo
} Grafo;

// Estrutura para fila usada no BFS
typedef struct NoFila {
    int indiceVertice;              // Índice do vértice no vetor
    struct NoFila *prox;
} NoFila;

typedef struct Fila {
    NoFila *inicio;
    NoFila *fim;
} Fila;

// Estrutura para armazenar caminho no BFS
typedef struct ArestaInfo {
    char *nomeUsuarioQueSegue;
    char *nomeUsuarioQueESeguida;
    char dataInicio[11];
    char dataFim[11];
    char grauAmizade;
} ArestaInfo;

typedef struct ListaCaminho {
    ArestaInfo *arestas;
    int tamanho;
} ListaCaminho;


// ===============================================================================
// FUNÇÕES AUXILIARES DO GRAFO
// ===============================================================================

// Criar grafo vazio
Grafo* criarGrafo(int numVertices);

// Liberar memória do grafo
void liberarGrafo(Grafo *grafo);

// Adicionar vértice ao grafo (em ordem alfabética)
int adicionarVertice(Grafo *grafo, char *nomeUsuario);

// Adicionar aresta ao grafo (em ordem alfabética)
void adicionarAresta(Grafo *grafo, int indiceOrigem, char *nomeUsuarioDestino, char *dataInicio, char *dataFim, char grauAmizade);

// Buscar índice de um vértice pelo nome
int buscarVertice(Grafo *grafo, char *nomeUsuario);

// Imprimir grafo
void imprimirGrafo(Grafo *grafo);

// Comparar datas e strings para ordenação
int compararParaOrdenacao(char *nomeA, char *dataInicioA, char *dataFimA, char *nomeB, char *dataInicioB, char *dataFimB);

// Comparar duas datas no formato DD/MM/AAAA
int compararDatas(char *data1, char *data2);


// ===============================================================================
// FUNÇÕES AUXILIARES DE FILA (BFS)
// ===============================================================================

Fila* criarFila();
void enfileirar(Fila *fila, int indiceVertice);
int desenfileirar(Fila *fila);
int filaVazia(Fila *fila);
void liberarFila(Fila *fila);


// ===============================================================================
// FUNÇÕES AUXILIARES PARA CONSTRUÇÃO DO GRAFO
// ===============================================================================

// Construir grafo a partir dos arquivos
Grafo* construirGrafo(char *arquivoPessoa, char *arquivoIndexaPessoa, char *arquivoSegueOrdenado, int transposto);

// Carregar dados de pessoas no grafo
int carregarPessoas(FILE *arquivoPessoa, Grafo *grafo);

// Adicionar arestas do arquivo segue ao grafo
void adicionarArestasSegue(FILE *arquivoSegue, Grafo *grafo, ARV *arvoreIndice, FILE *arquivoPessoa, int transposto);

// Buscar nome de usuário por ID
char* buscarNomeUsuarioPorID(int idPessoa, ARV *arvoreIndice, FILE *arquivoPessoa);


#endif
