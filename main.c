#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>


/* Testendo linux */

int main() {
    Blockchain* blockchain = (Blockchain*)malloc(sizeof(Blockchain));
    loadBlockchainFromFile(blockchain);

    if (!blockchain->head) {
        printf("Nenhuma blockchain encontrada. Criando bloco inicial...\n");
        blockchain->head = createBlock(0, "0");
    }

    programLoop(blockchain);
    free(blockchain);
    return 0;
}


