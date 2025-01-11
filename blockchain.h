#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#include "structures.h"
/* ----------------------------------------- HEADERS -----------------------------------------------------*/

// Função para inicializar um novo bloco
Block* createBlock(int index, const char* timestamp, const char* previousHash, const char* merkleRoot, int nonce, const char* currentHash);
// Função para exibir os dados de um bloco
void printBlock(Block* block);
















#endif // BLOCKCHAIN_H