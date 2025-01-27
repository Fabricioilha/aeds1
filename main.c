#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void printBlockchain(Blockchain blockchain);
void computeSHA256(char* str, char outputBuffer[EVP_MAX_MD_SIZE*2+1]);
void calculateMerkleRoot(char transactions[MAX_TRANSACTIONS][256], int count, char merkleRoot[EVP_MAX_MD_SIZE*2+1]);
void createBlock(Block* block, char previousHash[EVP_MAX_MD_SIZE*2+1]);
void addTransaction(Block* block, char* transaction);
void proofOfWork(Block* block);
void addBlock(Blockchain* blockchain, Block block);
int verifyTransactionInBlock(Block* block, char* transaction);
void simulateAttack(Blockchain* blockchain, char* newTransaction);
void saveBlockchain(Blockchain blockchain, const char* filename);
void loadBlockchain(Blockchain* blockchain, const char* filename);

Blockchain blockchain;

int main() {
    blockchain.blockCount = 0;
    Block genesisBlock;
    char genesisPreviousHash[EVP_MAX_MD_SIZE*2+1] = "0";
    createBlock(&genesisBlock, genesisPreviousHash);
    proofOfWork(&genesisBlock);
    addBlock(&blockchain, genesisBlock);

    int choice;
    char transaction[256];
    char filename[256];
    char transactionToVerify[256];
    char newTransaction[256];

    while(1) {
        printf("Menu:\n");
        printf("1. Inserir nova transação\n");
        printf("2. Minerar novo bloco\n");
        printf("3. Exibir blockchain\n");
        printf("4. Salvar blockchain\n");
        printf("5. Carregar blockchain\n");
        printf("6. Verificar se uma transação está incluída em um bloco\n");
        printf("7. Simular um ataque à rede\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &choice);
        getchar(); // consume newline
        switch(choice) {
            case 1:
                printf("Digite a transação: ");
                fgets(transaction, sizeof(transaction), stdin);
                transaction[strcspn(transaction, "\n")] = '\0'; // remove newline
                addTransaction(&blockchain.blocks[blockchain.blockCount - 1], transaction);
                break;
            case 2:
                {
                    Block newBlock;
                    createBlock(&newBlock, blockchain.blocks[blockchain.blockCount - 1].hash);
                    proofOfWork(&newBlock);
                    addBlock(&blockchain, newBlock);
                }
                break;
            case 3:
                printBlockchain(blockchain);
                break;
            case 4:
                printf("Digite o nome do arquivo: ");
                                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0'; // remove newline
                saveBlockchain(blockchain, filename);
                break;
            case 5:
                printf("Digite o nome do arquivo: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0'; // remove newline
                loadBlockchain(&blockchain, filename);
                break;
            case 6:
                printf("Digite a transação a verificar: ");
                fgets(transactionToVerify, sizeof(transactionToVerify), stdin);
                transactionToVerify[strcspn(transactionToVerify, "\n")] = '\0'; // remove newline
                printf("Digite o índice do bloco a verificar: ");
                int blockIndex;
                scanf("%d", &blockIndex);
                getchar(); // consume newline
                if(blockIndex >= 0 && blockIndex < blockchain.blockCount) {
                    verifyTransactionInBlock(&blockchain.blocks[blockIndex], transactionToVerify);
                } else {
                    printf("Índice do bloco inválido.\n");
                }
                break;
            case 7:
                printf("Digite a nova transação para o bloco inicial: ");
                fgets(newTransaction, sizeof(newTransaction), stdin);
                newTransaction[strcspn(newTransaction, "\n")] = '\0'; // remove newline
                simulateAttack(&blockchain, newTransaction);
                break;
            case 0:
                exit(0);
            default:
                printf("Escolha inválida.\n");
                break;
        }
    }

    return 0;
}


void printBlockchain(Blockchain blockchain) {
    for(int i = 0; i < blockchain.blockCount; i++) {
        Block block = blockchain.blocks[i];
        printf("----------------------------------------------------------\n");
        printf("Bloco %d:\n", block.index);
        printf("  Timestamp: %s\n", block.timestamp);
        printf("  Hash anterior: %s\n", block.previousHash);
        printf("  Raiz Merkle: %s\n", block.merkleRoot);
        printf("  Nonce: %d\n", block.nonce);
        printf("  Hash: %s\n", block.hash);
        printf("  Transações:\n");
        for(int j = 0; j < block.transactionCount; j++) {
            printf("    %s\n", block.transactions[j]);
        }
        printf("----------------------------------------------------------\n");
    }
}

// Calcular HASH
void computeSHA256(char* str, char outputBuffer[EVP_MAX_MD_SIZE*2+1]) {
    EVP_MD_CTX* context = EVP_MD_CTX_new(); //cria um novo contexto de digest (hash) usando a função EVP_MD_CTX_new do OpenSSL.
    const EVP_MD* md = EVP_sha256(); // obtém a referência para o algoritmo de hash SHA-256 usando EVP_sha256().
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;

    if(context != NULL) {
        if(EVP_DigestInit_ex(context, md, NULL)) {
            if(EVP_DigestUpdate(context, str, strlen(str))) { //diciona a string de entrada str ao contexto de digest.
                if(EVP_DigestFinal_ex(context, hash, &lengthOfHash)) { //Finaliza o digest e armazena o hash resultante no array hash
                    for(int i = 0; i < lengthOfHash; i++) {
                        sprintf(outputBuffer + (i * 2), "%02x", hash[i]); //Converte o hash binário em uma representação hexadecimal legível e armazena no outputBuffer
                    }
                    outputBuffer[lengthOfHash * 2] = 0;
                }
            }
        }
        EVP_MD_CTX_free(context);
    }
}

// Calcula a raiz da árvore Merkle (Merkle Root) para um conjunto de transações.
void calculateMerkleRoot(char transactions[MAX_TRANSACTIONS][256], int count, char merkleRoot[EVP_MAX_MD_SIZE*2+1]) {
    if(count == 0) {
        strcpy(merkleRoot, "0");
        return;
    }
    if(count == 1) {
        computeSHA256(transactions[0], merkleRoot);
        return;
    }
    char concatenatedHashes[MAX_TRANSACTIONS*EVP_MAX_MD_SIZE*2];
    concatenatedHashes[0] = 0;
    for(int i = 0; i < count; i++) {
        char hash[EVP_MAX_MD_SIZE*2+1];
        computeSHA256(transactions[i], hash);
        strcat(concatenatedHashes, hash);
    }
    computeSHA256(concatenatedHashes, merkleRoot);
}

// Cria o bloco
void createBlock(Block* block, char previousHash[EVP_MAX_MD_SIZE*2+1]) {
    block->index = blockchain.blockCount;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(block->timestamp, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    strcpy(block->previousHash, previousHash);
    block->transactionCount = 0;
    calculateMerkleRoot(block->transactions, block->transactionCount, block->merkleRoot);
    block->nonce = 0;
    char blockData[1024];
    sprintf(blockData, "%d%s%s%s%d", block->index, block->timestamp, block->previousHash, block->merkleRoot, block->nonce);
    computeSHA256(blockData, block->hash);
}

// Adciona transação ao bloco atual
void addTransaction(Block* block, char* transaction) {
    if(block->transactionCount < MAX_TRANSACTIONS) {
        strcpy(block->transactions[block->transactionCount], transaction);
        block->transactionCount++;
        calculateMerkleRoot(block->transactions, block->transactionCount, block->merkleRoot);
    } else {
        printf("Transação não adicionada: bloco cheio.\n");
    }
}

// as informacoes do blocos sao formatados em um hash para tentar encontrar os 4 zeros
void proofOfWork(Block* block) {
    char blockData[1024];
    do {
        block->nonce++; // Nonce é um número unico que é alterado a cada tentativa de decifrar a hash do bloco para encontrar os 4 zeros
        sprintf(blockData, "%d%s%s%s%d", block->index, block->timestamp, block->previousHash, block->merkleRoot, block->nonce);
        computeSHA256(blockData, block->hash);
    } while(strncmp(block->hash, "0000", 4) != 0);
}

// Adciona o bloco atual a blockchain
void addBlock(Blockchain* blockchain, Block block) {
    if(blockchain->blockCount < MAX_BLOCKS) {
        blockchain->blocks[blockchain->blockCount] = block;
        blockchain->blockCount++;
    } else {
        printf("Blockchain cheia: bloco não adicionado.\n");
    }
}

// Verificar transação
int verifyTransactionInBlock(Block* block, char* transaction) {
    for(int i = 0; i < block->transactionCount; i++) {
        if(strcmp(block->transactions[i], transaction) == 0) {
            printf("Transação encontrada no bloco %d.\n", block->index);
            return 1;
        }
    }
    printf("Transação não encontrada no bloco %d.\n", block->index);
    return 0;
}

// Simular ataque
void simulateAttack(Blockchain* blockchain, char* newTransaction) {
    if(blockchain->blockCount == 0) {
        printf("Blockchain vazia.\n");
        return;
    }

    // Altera a transação do bloco inicial
    Block* block = &blockchain->blocks[0];
    if(block->transactionCount == 0) {
        printf("Bloco inicial não possui transações.\n");
        return;
    }

    strcpy(block->transactions[0], newTransaction);
    calculateMerkleRoot(block->transactions, block->transactionCount, block->merkleRoot);

    // Reajusta todos os nonces
    clock_t start = clock();
    proofOfWork(block);

    for(int i = 1; i < blockchain->blockCount; i++) {
        strcpy(blockchain->blocks[i].previousHash, blockchain->blocks[i-1].hash);
        calculateMerkleRoot(blockchain->blocks[i].transactions, blockchain->blocks[i].transactionCount, blockchain->blocks[i].merkleRoot);
        proofOfWork(&blockchain->blocks[i]);
    }
    clock_t end = clock();
    double timeSpent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Ataque simulado. Tempo para reajustar todos os nonces: %.2f segundos.\n", timeSpent);
}





// Salvar blockchain em arquivo
void saveBlockchain(Blockchain blockchain, const char* filename) {
    FILE* file = fopen(filename, "w");
    if(file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    fwrite(&blockchain, sizeof(Blockchain), 1, file);
    fclose(file);
}

// Carregar blockchain de arquivo
void loadBlockchain(Blockchain* blockchain, const char* filename) {
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    fread(blockchain, sizeof(Blockchain), 1, file);
    fclose(file);
}


