#ifndef UTILS_H
#define UTILS_H

#include <openssl/sha.h>
#include "transactions.h"

typedef struct MerkleNode {
    char hash[SHA256_DIGEST_LENGTH * 2 + 1];
    struct MerkleNode *left;
    struct MerkleNode *right;
} MerkleNode;

void hashString(const char* str, char* output);
MerkleNode* createMerkleTree(Transaction transactions[], int count);
void freeMerkleTree(MerkleNode* node);

#endif
