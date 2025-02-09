#define _GNU_SOURCE
#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int dificuldade = 4;  // Começamos com 4 zeros no hash

#define TEMPO_ALVO 10  // Tempo alvo para minerar um bloco (segundos)
#define AJUSTE_INTERVALO 5  // Ajustar a dificuldade a cada 5 blocos
#define DIFICULDADE_MIN 2  // Dificuldade mínima permitida
#define DIFICULDADE_MAX 6  // Dificuldade máxima permitida

void ajustarDificuldade(Blockchain* blockchain) {
    if (blockchain->qtdBlocos < AJUSTE_INTERVALO) {
        return;  // Só ajusta após um certo número de blocos
    }

    Bloco* blocoAtual = &blockchain->blocos[blockchain->qtdBlocos - 1];
    Bloco* blocoAnterior = &blockchain->blocos[blockchain->qtdBlocos - AJUSTE_INTERVALO];

    struct tm tmInicio, tmFim;
    strptime(blocoAnterior->timestamp, "%Y-%m-%d %H:%M:%S", &tmInicio);
    strptime(blocoAtual->timestamp, "%Y-%m-%d %H:%M:%S", &tmFim);

    time_t tempoInicio = mktime(&tmInicio);
    time_t tempoFim = mktime(&tmFim);

    double tempoMedio = (double)(tempoFim - tempoInicio) / AJUSTE_INTERVALO;
    
    if (tempoMedio < TEMPO_ALVO) {
        if (dificuldade < DIFICULDADE_MAX) {
            dificuldade++;
            printf("Dificuldade aumentada para %d\n", dificuldade);
        }
    } else if (tempoMedio > TEMPO_ALVO) {
        if (dificuldade > DIFICULDADE_MIN) {
            dificuldade--;
            printf("Dificuldade reduzida para %d\n", dificuldade);
        }
    }
}

void verificarTransacaoMerkle(const Bloco* bloco, const char* transacao) {
    char hashTransacao[EVP_MAX_MD_SIZE * 2 + 1];
    calcularHashSHA256(transacao, hashTransacao);

    char hashes[MAX_TRANSACOES][EVP_MAX_MD_SIZE * 2 + 1];
    int numHashes = bloco->qtdTransacoes;
    int encontrada = -1;

    // Gerar os hashes das transações e encontrar o índice da transação desejada
    for (int i = 0; i < numHashes; i++) {
        calcularHashSHA256(bloco->transacoes[i], hashes[i]);
        if (strcmp(hashes[i], hashTransacao) == 0) {
            encontrada = i;
        }
    }

    if (encontrada == -1) {
        printf("Transação não encontrada na árvore de Merkle.");
        return;
    }

    // Propagar o hash até a raiz da árvore de Merkle
    while (numHashes > 1) {
        int novaQtd = 0;
        for (int i = 0; i < numHashes; i += 2) {
            char combinado[EVP_MAX_MD_SIZE * 4 + 1] = {0};
            if (i + 1 < numHashes) {
                snprintf(combinado, sizeof(combinado), "%s%s", hashes[i], hashes[i + 1]);
            } else {
                snprintf(combinado, sizeof(combinado), "%s%s", hashes[i], hashes[i]);
            }
            calcularHashSHA256(combinado, hashes[novaQtd]);

            if (i == encontrada || i + 1 == encontrada) {
                encontrada = novaQtd;
            }
            novaQtd++;
        }
        numHashes = novaQtd;
    }

    if (strcmp(hashes[0], bloco->raizMerkle) == 0) {
        printf("Transação encontrada e verificada na árvore de Merkle.\n");
    } else {
        printf("Falha na verificação da transação na árvore de Merkle.\n");
    }
}

void verificarTransacao(const Blockchain* blockchain, int indiceBloco, const char* transacao) {
    if (indiceBloco < 0 || indiceBloco >= blockchain->qtdBlocos) {
        printf("Índice de bloco inválido.\n");
        return;
    }

    verificarTransacaoMerkle(&blockchain->blocos[indiceBloco], transacao);
}

void simularAtaque(Blockchain* blockchain, const char* novaTransacao) {
    if (blockchain->qtdBlocos == 0) {
        printf("Blockchain vazia. Não é possível realizar o ataque.\n");
        return;
    }

    Bloco* bloco = &blockchain->blocos[0];

    if (bloco->qtdTransacoes == 0) {
        printf("Bloco gênesis não possui transações.\n");
        return;
    }

    printf("\n======= INICIANDO SIMULAÇÃO DE ATAQUE =======\n");

    // Alterar a primeira transação do bloco gênesis
    printf("1. Alterando a primeira transação do bloco gênesis...\n");
    strncpy(bloco->transacoes[0], novaTransacao, sizeof(bloco->transacoes[0]) - 1);
    bloco->transacoes[0][sizeof(bloco->transacoes[0]) - 1] = '\0';

    printf("   Nova transação: %s\n", bloco->transacoes[0]);

    // Recalcular a raiz de Merkle do bloco gênesis
    printf("2. Recalculando a raiz de Merkle do bloco gênesis...\n");
    calcularRaizMerkle(bloco);
    printf("   Nova raiz Merkle: %s\n", bloco->raizMerkle);

    // Recalcular prova de trabalho do bloco gênesis
    printf("3. Recalculando a prova de trabalho do bloco gênesis...\n");
    clock_t inicio = clock();
    provaDeTrabalho(bloco);
    clock_t fim = clock();
    printf("   Novo hash do bloco gênesis: %s\n", bloco->hash);

    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("   Tempo para recalcular prova de trabalho: %.2f segundos\n", tempoGasto);

    // Propagar a alteração para os blocos subsequentes
    for (int i = 1; i < blockchain->qtdBlocos; i++) {
        printf("\n4. Atualizando bloco %d...\n", i);
        Bloco* blocoAtual = &blockchain->blocos[i];

        printf("   Atualizando hash anterior...\n");
        strncpy(blocoAtual->hashAnterior, blockchain->blocos[i - 1].hash, sizeof(blocoAtual->hashAnterior) - 1);
        blocoAtual->hashAnterior[sizeof(blocoAtual->hashAnterior) - 1] = '\0';
        printf("   Novo hash anterior: %s\n", blocoAtual->hashAnterior);

        printf("   Recalculando a raiz de Merkle...\n");
        calcularRaizMerkle(blocoAtual);
        printf("   Nova raiz Merkle: %s\n", blocoAtual->raizMerkle);

        printf("   Recalculando a prova de trabalho...\n");
        clock_t inicioBloco = clock();
        provaDeTrabalho(blocoAtual);
        clock_t fimBloco = clock();
        printf("   Novo hash do bloco %d: %s\n", i, blocoAtual->hash);

        double tempoBloco = (double)(fimBloco - inicioBloco) / CLOCKS_PER_SEC;
        printf("   Tempo para recalcular prova de trabalho: %.2f segundos\n", tempoBloco);
    }

    printf("\n======= SIMULAÇÃO DE ATAQUE CONCLUÍDA =======\n");
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
