#ifndef headers_h
#define headers_h
#include "structures.h"



void printBlockchain(Blockchain blockchain);
void computeSHA256(char* str, char outputBuffer[EVP_MAX_MD_SIZE*2+1]);
void calculateMerkleRoot(char transactions[MAX_TRANSACTIONS][256], int count, char merkleRoot[EVP_MAX_MD_SIZE*2+1]);
void createBlock(Block* block, char previousHash[EVP_MAX_MD_SIZE*2+1]);
void addTransaction(Block* block, char* transaction);
void proofOfWork(Block* block);
void addBlock(Blockchain* blockchain, Block block);
int verifyTransactionInBlock(Block* block, char* transaction);
void simulateAttack(Blockchain* blockchain, char* newTransaction);
void saveBlockchain(Blockchain blockchain, const char* filename);
void loadBlockchain(Blockchain* blockchain, const char* filename);

#endif
