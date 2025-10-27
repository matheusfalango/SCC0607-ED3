// 15479691 Matheus Soares Falango
// 15578973 Murilo Gonzales Vieira

#ifndef SEGUE_H
#define SEGUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilidades.h"
#include "tratarString.h"
#include "manipularAVL.h"
#include "ArvAVL.h"

// ===============================================================================
// DEFINIÇÕES - ARQUIVO SEGUE
// ===============================================================================

#define SEGUE_HEADER_SIZE 9 // Tamanho fixo do cabeçalho de segue (status + quantidadePessoas + proxRRN)

// Estrutura do cabeçalho do arquivo segue (segue.bin)
typedef struct {
    char status; // '0' (inconsistente) ou '1' (consistente)
    int quantidadePessoas; // Número total de pessoas que seguem outras pessoas presentes no arquivo
    int proxRRN; // Armazena o valor do proximo byte offset disponivel
} SegueHeader;

// Estrutura do registro de dados do segue (segue.bin)
typedef struct {
    char removido; // '0' não removido, '1' removido
    int idPessoaQueSegue; // identifica o idPessoa que esta seguindo
    int idPessoaQueESeguida; // identifica o idPessoa que é seguida por outra pessoa
    char dataInicioQueSegue[10]; // data de inicio que o id segue começou a seguir a id seguida
    char dataFimQueSegue[10]; // data de fim que o id segue terminou de seguir a id seguida
    char grauAmizade; // segue uma pessoa porque é; '0' celebridade, '1' amiga da minha amiga, '2' amiga
} SegueRecord;


#endif