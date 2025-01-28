#include "block.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void inicializarBloco(Bloco* bloco, const char* hashAnterior) {
    bloco->indice = 0;
    time_t agora = time(NULL);
    strftime(bloco->timestamp, sizeof(bloco->timestamp), "%Y-%m-%d %H:%M:%S", localtime(&agora));
    strcpy(bloco->hashAnterior, hashAnterior);
    bloco->qtdTransacoes = 0;
    bloco->nonce = 0;
    calcularRaizMerkle(bloco);
}

void adicionarTransacao(Bloco* bloco, const char* transacao) {
    if (bloco->qtdTransacoes < MAX_TRANSACOES) {
        strcpy(bloco->transacoes[bloco->qtdTransacoes++], transacao);
        calcularRaizMerkle(bloco);
    } else {
        printf("Bloco cheio! Não é possível adicionar mais transações.\n");
    }
}

void calcularRaizMerkle(Bloco* bloco) {
    if (bloco->qtdTransacoes == 0) {
        strcpy(bloco->raizMerkle, "0");
        return;
    }

    char hashesConcatenados[MAX_TRANSACOES * EVP_MAX_MD_SIZE * 2] = {0};
    for (int i = 0; i < bloco->qtdTransacoes; i++) {
        char hash[EVP_MAX_MD_SIZE * 2 + 1];
        calcularHashSHA256(bloco->transacoes[i], hash);
        strcat(hashesConcatenados, hash);
    }
    calcularHashSHA256(hashesConcatenados, bloco->raizMerkle);
}

void provaDeTrabalho(Bloco* bloco) {
    char dadosBloco[1024];
    do {
        bloco->nonce++;
        sprintf(dadosBloco, "%d%s%s%s%d", bloco->indice, bloco->timestamp, bloco->hashAnterior, bloco->raizMerkle, bloco->nonce);
        calcularHashSHA256(dadosBloco, bloco->hash);
    } while (strncmp(bloco->hash, "0000", 4) != 0);
}

void calcularHashSHA256(const char* entrada, char hash[EVP_MAX_MD_SIZE * 2 + 1]) {
    EVP_MD_CTX* contexto = EVP_MD_CTX_new();
    const EVP_MD* algoritmo = EVP_sha256();
    unsigned char hashBinario[EVP_MAX_MD_SIZE];
    unsigned int tamanhoHash = 0;

    if (contexto) {
        if (EVP_DigestInit_ex(contexto, algoritmo, NULL)) {
            if (EVP_DigestUpdate(contexto, entrada, strlen(entrada))) {
                if (EVP_DigestFinal_ex(contexto, hashBinario, &tamanhoHash)) {
                    for (unsigned int i = 0; i < tamanhoHash; i++) {
                        sprintf(hash + (i * 2), "%02x", hashBinario[i]);
                    }
                    hash[tamanhoHash * 2] = '\0';
                }
            }
        }
        EVP_MD_CTX_free(contexto);
    }
}
