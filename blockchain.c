#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCKCHAIN_FILE "blockchain.json"
void loadBlockchainFromFile(Blockchain* blockchain) {
    FILE* file = fopen(BLOCKCHAIN_FILE, "r");
    if (!file) {
        printf("Nenhum arquivo de blockchain encontrado. Criando uma nova blockchain.\n");
        blockchain->head = createBlock(0, "0");
        return;
    }

    char buffer[1024];
    Block* previousBlock = NULL;
    while (fgets(buffer, sizeof(buffer), file)) {
        Block* block = (Block*)malloc(sizeof(Block));
        sscanf(buffer, "%d %s %s %d %ld", 
            &block->index, 
            block->previousHash, 
            block->merkleRoot, 
            &block->nonce, 
            &block->timestamp);

        block->next = NULL;
        if (!previousBlock) {
            blockchain->head = block;
        } else {
            previousBlock->next = block;
        }
        previousBlock = block;
    }
    fclose(file);
    printf("Blockchain carregada do arquivo '%s'.\n", BLOCKCHAIN_FILE);
}


void saveBlockchainToFile(Blockchain* blockchain) {
    FILE* file = fopen(BLOCKCHAIN_FILE, "w");
    if (!file) {
        printf("Erro ao abrir o arquivo para salvar a blockchain!\n");
        return;
    }

    fprintf(file, "[\n");
    Block* current = blockchain->head;
    while (current) {
        fprintf(file, "  {\n");
        fprintf(file, "    \"index\": %d,\n", current->index);
        fprintf(file, "    \"timestamp\": %ld,\n", current->timestamp);
        fprintf(file, "    \"previousHash\": \"%s\",\n", current->previousHash);
        fprintf(file, "    \"hash\": \"%s\",\n", current->hash);
        fprintf(file, "    \"merkleRoot\": \"%s\",\n", current->merkleRoot);
        fprintf(file, "    \"nonce\": %d,\n", current->nonce);
        fprintf(file, "    \"transactions\": [\n");
        for (int i = 0; i < current->transactionCount; i++) {
            fprintf(file,
                    "      {\"sender\": \"%s\", \"receiver\": \"%s\", \"amount\": %d}%s\n",
                    current->transactions[i].sender,
                    current->transactions[i].receiver,
                    current->transactions[i].amount,
                    (i < current->transactionCount - 1) ? "," : "");
        }
        fprintf(file, "    ]\n");
        fprintf(file, "  }%s\n", current->next ? "," : "");
        current = current->next;
    }
    fprintf(file, "]\n");
    fclose(file);
    printf("Blockchain salva em '%s'.\n", BLOCKCHAIN_FILE);
}
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
void addTransaction(Block* block, const char* sender, const char* receiver, int amount) {
    if (block->transactionCount < MAX_TRANSACTIONS) {
        Transaction* newTransaction = &block->transactions[block->transactionCount++];
        strncpy(newTransaction->sender, sender, sizeof(newTransaction->sender) - 1);
        newTransaction->sender[sizeof(newTransaction->sender) - 1] = '\0';
        strncpy(newTransaction->receiver, receiver, sizeof(newTransaction->receiver) - 1);
        newTransaction->receiver[sizeof(newTransaction->receiver) - 1] = '\0';
        newTransaction->amount = amount;
    } else {
        printf("Máximo de transações nesse bloco atingido!\n");
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

void mineBlock(Block* block) {
    if (block->transactionCount == 0) {
        printf("Nenhuma transação para minerar!\n");
        return;
    }
    printf("Minerando o bloco %d...\n", block->index);
    calculateMerkleRoot(block);
    proofOfWork(block);
    printf("Bloco minerado com sucesso! Hash: %s\n", block->hash);
}
void displayBlockchain(Blockchain* blockchain) {
    FILE* file = fopen("blockchain.json", "w");
    if (!file) {
        printf("Erro ao abrir o arquivo para exibir a blockchain!\n");
        return;
    }

    Block* current = blockchain->head;
    while (current) {
        fprintf(file, "  {\n");
        fprintf(file, "    \"index\": %d,\n", current->index);
        fprintf(file, "    \"timestamp\": %ld,\n", current->timestamp);
        fprintf(file, "    \"previousHash\": \"%s\",\n", current->previousHash);
        fprintf(file, "    \"hash\": \"%s\",\n", current->hash);
        fprintf(file, "    \"merkleRoot\": \"%s\",\n", current->merkleRoot);
        fprintf(file, "    \"nonce\": %d,\n", current->nonce);
        fprintf(file, "    \"transactions\": [\n");

        for (int i = 0; i < current->transactionCount; i++) {
            fprintf(file,
                    "      {\"sender\": \"%s\", \"receiver\": \"%s\", \"amount\": %d}%s\n",
                    current->transactions[i].sender,
                    current->transactions[i].receiver,
                    current->transactions[i].amount,
                    (i < current->transactionCount - 1) ? "," : "");
        }

        fprintf(file, "    ]\n");
        fprintf(file, "  }%s\n", current->next ? "," : "");
        current = current->next;
    }

    fclose(file);
}

void programLoop(Blockchain* blockchain) {
    while (1) {
        printf("\nEscolha uma opção:\n");
        printf("1. Adicionar transação\n");
        printf("2. Minerar bloco atual\n");
        printf("3. Exibir blockchain\n");
        printf("4. Sair\n");
        printf("Sua escolha: ");
        int choice;
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
                printf("Digite os dados da transação (formato: pessoaX pessoaY quantidade): ");
                char sender[50], receiver[50];
                int amount;
                if (scanf("%s %s %d", sender, receiver, &amount) != 3) {
                    printf("Entrada inválida! Certifique-se de usar o formato correto.\n");
                    while (getchar() != '\n');
                    break;
                }
                while (getchar() != '\n');
                addTransaction(blockchain->head, sender, receiver, amount);
                break;
            }

            case 2: {
                mineBlock(blockchain->head);
                if (blockchain->head->nonce > 0) {
                    Block* newBlock = createBlock(blockchain->head->index + 1, blockchain->head->hash);
                    newBlock->next = blockchain->head;
                    blockchain->head = newBlock;
                    
                    saveBlockchainToFile(blockchain);
                } else {
                    printf("Erro na mineração. Tente novamente.\n");
                }

                break;
            }
            case 3: {
                displayBlockchain(blockchain);
                break;
            }
            case 4: {
                printf("Encerrando o programa...\n");
                return;
            }
            default:
                printf("Opção inválida!\n");
        }
    }
}
