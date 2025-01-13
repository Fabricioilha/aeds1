#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Block* createBlock(int index, const char* previousHash) {
    Block* newBlock = (Block*)malloc(sizeof(Block));
    newBlock->index = index;
    newBlock->timestamp = time(NULL);
    strcpy(newBlock->previousHash, previousHash);
    newBlock->nonce = 0;
    newBlock->transactionCount = 0;
    newBlock->next = NULL;
    return newBlock;
}

void addTransaction(Block* block, const char* data) {
    if (block->transactionCount < MAX_TRANSACTIONS) {
        strcpy(block->transactions[block->transactionCount].data, data);
        block->transactionCount++;
    } else {
        printf("Máximo de transações nesse bloco!\n");
    }
}

void calculateMerkleRoot(Block* block) {
    MerkleNode* root = createMerkleTree(block->transactions, block->transactionCount);
    if (root) {
        strcpy(block->merkleRoot, root->hash);
        freeMerkleTree(root);
    }
}

int proofOfWork(Block* block) {
    while (1) {
        char blockData[1024];
        snprintf(blockData, sizeof(blockData), "%d%s%s%d", block->index, block->previousHash, block->merkleRoot, block->nonce);
        hashString(blockData, block->hash);
        if (strncmp(block->hash, "0000", DIFFICULTY) == 0) {
            return 1;
        }
        block->nonce++;
    }
}

void displayBlockchain(Blockchain* bc) {
    Block* current = bc->head;
    while (current != NULL) {
        printf("\n\nBlock %d:\n", current->index);
        if (current->hash != NULL) {
            printf("Hash: %s\n", current->hash);
        } else {
            printf("Hash: NULL\n");
        }
        if (current->previousHash != NULL) {
            printf("Previous Hash: %s\n", current->previousHash);
        } else {
            printf("Previous Hash: NULL\n");
        }
        if (current->merkleRoot != NULL) {
            printf("Merkle Root: %s\n", current->merkleRoot);
        } else {
            printf("Merkle Root: NULL\n");
        }

        printf("Timestamp: %s", ctime(&current->timestamp));

        printf("Nonce: %d\n", current->nonce);
        printf("Transactions:\n");
        for (int i = 0; i < current->transactionCount; i++) {
            printf("    %s\n", current->transactions[i].data);
        }
        current = current->next;
    }
}
