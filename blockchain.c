#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inicializarBlockchain(Blockchain* blockchain) {
    blockchain->qtdBlocos = 0;
}

void adicionarBloco(Blockchain* blockchain, Bloco bloco) {
    if (blockchain->qtdBlocos < MAX_BLOCOS) {
        blockchain->blocos[blockchain->qtdBlocos++] = bloco;
    } else {
        printf("Blockchain cheia! Não é possível adicionar mais blocos.\n");
    }
}

void salvarBlockchain(const Blockchain* blockchain, const char* arquivo) {
    FILE* file = fopen(arquivo, "wb");
    if (!file) {
        printf("Erro ao abrir o arquivo para salvar.\n");
        return;
    }
    fwrite(blockchain, sizeof(Blockchain), 1, file);
    fclose(file);
    printf("Blockchain salva com sucesso em '%s'.\n", arquivo);
}

void carregarBlockchain(Blockchain* blockchain, const char* arquivo) {
    FILE* file = fopen(arquivo, "rb");
    if (!file) {
        printf("Erro ao abrir o arquivo para carregar.\n");
        return;
    }
    fread(blockchain, sizeof(Blockchain), 1, file);
    fclose(file);
    printf("Blockchain carregada com sucesso de '%s'.\n", arquivo);
}

void exibirBlockchain(const Blockchain* blockchain) {
    for (int i = 0; i < blockchain->qtdBlocos; i++) {
        if(i == 0 ){
            printf("\n\n----------------------------------------------------------\n");
        }
        const Bloco* bloco = &blockchain->blocos[i];
        printf("Bloco %d:\n", bloco->indice);
        printf("  Timestamp: %s\n", bloco->timestamp);
        printf("  Hash Anterior: %s\n", bloco->hashAnterior);
        printf("  Raiz Merkle: %s\n", bloco->raizMerkle);
        printf("  Nonce: %d\n", bloco->nonce);
        printf("  Hash: %s\n", bloco->hash);
        printf("  Transações:\n");
        for (int j = 0; j < bloco->qtdTransacoes; j++) {
            printf("    %s\n", bloco->transacoes[j]);
        }
        printf("----------------------------------------------------------\n");
    }
}
