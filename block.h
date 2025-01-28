#ifndef BLOCK_H
#define BLOCK_H

#include <openssl/evp.h>

#define MAX_TRANSACOES 100

typedef struct {
    int indice;
    char timestamp[64];
    char hashAnterior[EVP_MAX_MD_SIZE * 2 + 1];
    char raizMerkle[EVP_MAX_MD_SIZE * 2 + 1];
    int nonce;
    char hash[EVP_MAX_MD_SIZE * 2 + 1];
    char transacoes[MAX_TRANSACOES][256];
    int qtdTransacoes;
} Bloco;

void inicializarBloco(Bloco* bloco, const char* hashAnterior);
void adicionarTransacao(Bloco* bloco, const char* transacao);
void calcularRaizMerkle(Bloco* bloco);
void provaDeTrabalho(Bloco* bloco);
void calcularHashSHA256(const char* entrada, char hash[EVP_MAX_MD_SIZE * 2 + 1]);

#endif
