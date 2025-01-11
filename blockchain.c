#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "blockchain.h"
#include "transactions.h"
#include "utils.h"

// Função para inicializar um novo bloco
Block* createBlock(int index, const char* timestamp, const char* previousHash, const char* merkleRoot, int nonce, const char* currentHash) {
    Block* newBlock = (Block*)malloc(sizeof(Block));

    newBlock->index = index;
    strncpy(newBlock->timestamp, timestamp, sizeof(newBlock->timestamp));
    strncpy(newBlock->previousHash, previousHash, sizeof(newBlock->previousHash));
    strncpy(newBlock->merkleRoot, merkleRoot, sizeof(newBlock->merkleRoot));
    newBlock->nonce = nonce;
    strncpy(newBlock->currentHash, currentHash, sizeof(newBlock->currentHash));

    return newBlock;
}


// Função para exibir os dados de um bloco
void printBlock(Block* block) {
    if (block) {
        printf("Block %d:\n", block->index);
        printf("Timestamp: %s\n", block->timestamp);
        printf("Previous Hash: %s\n", block->previousHash);
        printf("Merkle Root: %s\n", block->merkleRoot);
        printf("Nonce: %d\n", block->nonce);
        printf("Current Hash: %s\n", block->currentHash);
    }
}