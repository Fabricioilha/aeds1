#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <time.h>
#include <openssl/sha.h>
#include "transactions.h"
#include "utils.h"

#define MAX_TRANSACTIONS 100
#define DIFFICULTY 4

typedef struct Block {
    int index;
    time_t timestamp;
    char previousHash[SHA256_DIGEST_LENGTH * 2 + 1];
    char merkleRoot[SHA256_DIGEST_LENGTH * 2 + 1];
    int nonce;
    char hash[SHA256_DIGEST_LENGTH * 2 + 1];
    Transaction transactions[MAX_TRANSACTIONS];
    int transactionCount;
    struct Block *next;
} Block;

typedef struct Blockchain {
    Block *head;
} Blockchain;

Block* createBlock(int index, const char* previousHash);
void addTransaction(Block* block, const char* data);
void calculateMerkleRoot(Block* block);
int proofOfWork(Block* block);
void displayBlockchain(Blockchain* bc);

#endif
