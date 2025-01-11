#ifndef STRUCTURES_H
#define STRUCTURES_H

// Estrutura do bloco
typedef struct Block {
    int index;
    char timestamp[20];
    char previousHash[64];
    char merkleRoot[64];
    int nonce;
    char currentHash[64];
} Block;

// Estrutura de uma transação (se necessária)
typedef struct Transaction {
    char data[256]; // Dados da transação
} Transaction;

// Outras estruturas relacionadas, como elementos da Merkle Tree, podem ser adicionadas aqui


#endif // STRUCTURES_H