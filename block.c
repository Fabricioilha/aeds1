#include "block.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "blockchain.h"

void inicializarBloco(Bloco* bloco, const char* hashAnterior, int ind) {
    bloco->indice = ind;
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

    // Vetor dinâmico para armazenar os hashes iniciais das transações
    char hashes[MAX_TRANSACOES][EVP_MAX_MD_SIZE * 2 + 1];
    int numHashes = bloco->qtdTransacoes;

    // Gerar o hash de cada transação
    for (int i = 0; i < numHashes; i++) {
        calcularHashSHA256(bloco->transacoes[i], hashes[i]);
    }

    // Reduzindo até restar um único hash
    while (numHashes > 1) {
        int novaQtd = 0;
        for (int i = 0; i < numHashes; i += 2) {
            char combinado[EVP_MAX_MD_SIZE * 4 + 1] = {0};

            // Se houver um par, concatena os dois hashes
            if (i + 1 < numHashes) {
                snprintf(combinado, sizeof(combinado), "%s%s", hashes[i], hashes[i + 1]);
            } else {
                // Se for ímpar, repete o último hash
                snprintf(combinado, sizeof(combinado), "%s%s", hashes[i], hashes[i]);
            }

            // Calcula novo hash e armazena no próximo nível
            calcularHashSHA256(combinado, hashes[novaQtd]);
            novaQtd++;
        }
        numHashes = novaQtd; // Atualiza a quantidade de hashes
    }

    // O último hash restante é a raiz de Merkle
    strcpy(bloco->raizMerkle, hashes[0]);
}

void provaDeTrabalho(Bloco* bloco) {
    char dadosBloco[1024];
    do {
        bloco->nonce++;
        sprintf(dadosBloco, "%d%s%s%s%d", bloco->indice, bloco->timestamp, bloco->hashAnterior, bloco->raizMerkle, bloco->nonce);
        calcularHashSHA256(dadosBloco, bloco->hash);
    } while (strncmp(bloco->hash, "00000000", dificuldade) != 0);
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
