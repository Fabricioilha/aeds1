#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void verificarTransacao(const Blockchain* blockchain, int indiceBloco, const char* transacao) {
    if (indiceBloco < 0 || indiceBloco >= blockchain->qtdBlocos) {
        printf("Índice de bloco inválido. Não foi possível verificar a transação.\n");
        return;
    }

    const Bloco* bloco = &blockchain->blocos[indiceBloco];
    for (int i = 0; i < bloco->qtdTransacoes; i++) {
        if (strcmp(bloco->transacoes[i], transacao) == 0) {
            printf("Transação encontrada no bloco %d.\n", indiceBloco);
            return;
        }
    }
    printf("Transação não encontrada no bloco %d.\n", indiceBloco);
}

void simularAtaque(Blockchain* blockchain, const char* novaTransacao) {
    if (blockchain->qtdBlocos == 0) {
        printf("Blockchain vazia. Não é possível realizar o ataque.\n");
        return;
    }

    // Alterar a primeira transação do bloco gênesis
    Bloco* bloco = &blockchain->blocos[0];
    if (bloco->qtdTransacoes == 0) {
        printf("Bloco gênesis não possui transações.\n");
        return;
    }

    printf("Alterando a primeira transação do bloco gênesis...\n");
    strcpy(bloco->transacoes[0], novaTransacao);
    calcularRaizMerkle(bloco);

    // Recalcular prova de trabalho do bloco gênesis
    printf("Recalculando prova de trabalho do bloco gênesis...\n");
    clock_t inicio = clock();
    provaDeTrabalho(bloco);

    // Propagar alterações para os blocos subsequentes
    for (int i = 1; i < blockchain->qtdBlocos; i++) {
        Bloco* blocoAtual = &blockchain->blocos[i];
        strcpy(blocoAtual->hashAnterior, blockchain->blocos[i - 1].hash);
        calcularRaizMerkle(blocoAtual);
        provaDeTrabalho(blocoAtual);
    }
    clock_t fim = clock();

    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Ataque simulado concluído. Tempo total para recalcular os hashes: %.2f segundos.\n", tempoGasto);
}

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
