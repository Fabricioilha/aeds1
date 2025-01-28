#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "block.h"

#define MAX_BLOCOS 100

typedef struct {
    Bloco blocos[MAX_BLOCOS];
    int qtdBlocos;
} Blockchain;

void inicializarBlockchain(Blockchain* blockchain);
void adicionarBloco(Blockchain* blockchain, Bloco bloco);
void salvarBlockchain(const Blockchain* blockchain, const char* arquivo);
void carregarBlockchain(Blockchain* blockchain, const char* arquivo);
void exibirBlockchain(const Blockchain* blockchain);

#endif
