#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void hashString(const char* str, char* output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)str, strlen(str), hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&output[i * 2], "%02x", hash[i]);
    }
}

MerkleNode* createMerkleTree(Transaction transactions[], int count) {
    if (count == 0) return NULL;
    if (count == 1) {
        MerkleNode* leaf = (MerkleNode*)malloc(sizeof(MerkleNode));
        hashString(transactions[0].data, leaf->hash);
        leaf->left = leaf->right = NULL;
        return leaf;
    }

    int mid = count / 2;
    MerkleNode* left = createMerkleTree(transactions, mid);
    MerkleNode* right = createMerkleTree(transactions + mid, count - mid);

    MerkleNode* node = (MerkleNode*)malloc(sizeof(MerkleNode));
    char combinedHash[(SHA256_DIGEST_LENGTH * 2 + 1) * 2];
    snprintf(combinedHash, sizeof(combinedHash), "%s%s", left->hash, right->hash);
    hashString(combinedHash, node->hash);

    node->left = left;
    node->right = right;
    return node;
}

void freeMerkleTree(MerkleNode* node) {
    if (!node) return;
    freeMerkleTree(node->left);
    freeMerkleTree(node->right);
    free(node);
}
