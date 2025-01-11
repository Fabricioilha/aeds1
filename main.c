#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "blockchain.h"
#include "transactions.h"
#include "utils.h"

int main() {
    //criação de um bloco
    Block* genesisBlock = createBlock(0, "2025-01-09 10:00:00", "0", "abc123", 0, "def456");

    printBlock(genesisBlock);

    free(genesisBlock);

    return 0;
}
