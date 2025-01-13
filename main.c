#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    Blockchain* blockchain = (Blockchain*)malloc(sizeof(Blockchain));
    blockchain->head = createBlock(0, "0");

    addTransaction(blockchain->head, "Transaction 1");
    addTransaction(blockchain->head, "Transaction 2");

    calculateMerkleRoot(blockchain->head);
    proofOfWork(blockchain->head);

    Block* newBlock = createBlock(1, blockchain->head->hash);
    blockchain->head->next = newBlock;

    displayBlockchain(blockchain);

    free(blockchain);
    return 0;
}
