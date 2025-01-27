#ifndef structures_h
#define structures_h
#include <openssl/evp.h>
#define MAX_TRANSACTIONS 100
#define MAX_BLOCKS 100


typedef struct {
    int index;
    char timestamp[64];
    char previousHash[EVP_MAX_MD_SIZE*2+1];
    char merkleRoot[EVP_MAX_MD_SIZE*2+1];
    int nonce;
    char hash[EVP_MAX_MD_SIZE*2+1];
    char transactions[MAX_TRANSACTIONS][256];
    int transactionCount;
} Block;

typedef struct {
    Block blocks[MAX_BLOCKS];
    int blockCount;
} Blockchain;
#endif