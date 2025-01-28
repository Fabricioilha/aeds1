#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    Blockchain blockchain;
    inicializarBlockchain(&blockchain);

    // Criar o bloco gênesis
    Bloco blocoGenesis;
    inicializarBloco(&blocoGenesis, "0");
    provaDeTrabalho(&blocoGenesis);
    adicionarBloco(&blockchain, blocoGenesis);

    int escolha;
    char transacao[256];
    char nomeArquivo[256];
    char transacaoParaVerificar[256];
    int indiceBloco;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Inserir nova transação\n");
        printf("2. Minerar novo bloco\n");
        printf("3. Exibir blockchain\n");
        printf("4. Salvar blockchain em arquivo\n");
        printf("5. Carregar blockchain de arquivo\n");
        printf("6. Verificar se uma transação está em um bloco\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);
        getchar(); // Consumir o caractere de nova linha deixado pelo scanf

        switch (escolha) {
            case 1: // Inserir nova transação
                printf("Digite a transação: ");
                fgets(transacao, sizeof(transacao), stdin);
                transacao[strcspn(transacao, "\n")] = '\0'; // Remover o newline
                adicionarTransacao(&blockchain.blocos[blockchain.qtdBlocos - 1], transacao);
                break;

            case 2: { // Minerar novo bloco
                Bloco novoBloco;
                inicializarBloco(&novoBloco, blockchain.blocos[blockchain.qtdBlocos - 1].hash);
                provaDeTrabalho(&novoBloco);
                adicionarBloco(&blockchain, novoBloco);
                break;
            }

            case 3: // Exibir blockchain
                exibirBlockchain(&blockchain);
                break;

            case 4: // Salvar blockchain em arquivo
                printf("Digite o nome do arquivo: ");
                fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
                nomeArquivo[strcspn(nomeArquivo, "\n")] = '\0'; // Remover o newline
                salvarBlockchain(&blockchain, nomeArquivo);
                break;

            case 5: // Carregar blockchain de arquivo
                printf("Digite o nome do arquivo: ");
                fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
                nomeArquivo[strcspn(nomeArquivo, "\n")] = '\0'; // Remover o newline
                carregarBlockchain(&blockchain, nomeArquivo);
                break;

            case 6: // Verificar transação
                printf("Digite a transação a verificar: ");
                fgets(transacaoParaVerificar, sizeof(transacaoParaVerificar), stdin);
                transacaoParaVerificar[strcspn(transacaoParaVerificar, "\n")] = '\0'; // Remover o newline
                printf("Digite o índice do bloco: ");
                scanf("%d", &indiceBloco);
                getchar(); // Consumir o caractere de nova linha

                if (indiceBloco >= 0 && indiceBloco < blockchain.qtdBlocos) {
                    Bloco* bloco = &blockchain.blocos[indiceBloco];
                    int encontrada = 0;
                    for (int i = 0; i < bloco->qtdTransacoes; i++) {
                        if (strcmp(bloco->transacoes[i], transacaoParaVerificar) == 0) {
                            printf("Transação encontrada no bloco %d.\n", indiceBloco);
                            encontrada = 1;
                            break;
                        }
                    }
                    if (!encontrada) {
                        printf("Transação não encontrada no bloco %d.\n", indiceBloco);
                    }
                } else {
                    printf("Índice de bloco inválido.\n");
                }
                break;

            case 0: // Sair
                printf("Encerrando o programa.\n");
                exit(0);

            default: // Opção inválida
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    }

    return 0;
}
