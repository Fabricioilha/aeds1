#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inserirTransacoesEMinerar(Blockchain* bc);

int main() {
	Blockchain blockchain;
	inicializarBlockchain(&blockchain);

	// Criar o bloco genesis
	Bloco blocoGenesis;
	inicializarBloco(&blocoGenesis, "0", 0);
	provaDeTrabalho(&blocoGenesis);
	adicionarBloco(&blockchain, blocoGenesis);

	int opcao;
	char transacao[256];
	char nomeArquivo[256];
	char transacaoParaVerificar[256];
	char novaTransacao[256];
	int indiceBloco;

	while (1) {
		printf("Dificuldade: %d\n", dificuldade);
		printf("\nMenu:\n");
		printf("1. Inserir nova transacao\n");
		printf("2. Minerar novo bloco\n");
		printf("3. Exibir blockchain\n");
		printf("4. Salvar blockchain em arquivo\n");
		printf("5. Carregar blockchain de arquivo\n");
		printf("6. Verificar transacao em um bloco\n");
		printf("7. Simular ataque na blockchain\n");
		printf("8. Inserir blocos automaticamente (testes)\n");
		printf("0. Sair\n");
		printf("Escolha uma opcao: ");
		scanf("%d", &opcao);
		getchar(); // Consumir o newline deixado pelo scanf

		switch (opcao) {
		case 1: // Inserir nova transaC'C#o
			printf("Digite a transacao: ");
			fgets(transacao, sizeof(transacao), stdin);
			transacao[strcspn(transacao, "\n")] = '\0'; // Remover o newline
			adicionarTransacao(&blockchain.blocos[blockchain.qtdBlocos - 1], transacao);
			break;

		case 2: { // Minerar novo bloco
			Bloco novoBloco;
			inicializarBloco(&novoBloco, blockchain.blocos[blockchain.qtdBlocos - 1].hash, blockchain.qtdBlocos);
			provaDeTrabalho(&novoBloco);
			adicionarBloco(&blockchain, novoBloco);
			ajustarDificuldade(&blockchain); // Ajusta a dificuldade dinamicamente
			break;
		}

		case 3: // Exibir blockchain
			exibirBlockchain(&blockchain);
			break;

		case 4: // Salvar blockchain em arquivo
			printf("Digite o nome do arquivo para salvar: ");
			fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
			nomeArquivo[strcspn(nomeArquivo, "\n")] = '\0'; // Remover o newline
			salvarBlockchain(&blockchain, nomeArquivo);
			break;

		case 5: // Carregar blockchain de arquivo
			printf("Digite o nome do arquivo para carregar: ");
			fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
			nomeArquivo[strcspn(nomeArquivo, "\n")] = '\0'; // Remover o newline
			carregarBlockchain(&blockchain, nomeArquivo);
			break;

		case 6: // Verificar transaC'C#o em um bloco
			printf("Digite a transacao a verificar: ");
			fgets(transacaoParaVerificar, sizeof(transacaoParaVerificar), stdin);
			transacaoParaVerificar[strcspn(transacaoParaVerificar, "\n")] = '\0'; // Remover o newline
			printf("Digite o inndice do bloco para verificar: ");
			scanf("%d", &indiceBloco);
			getchar(); // Consumir o newline deixado pelo scanf

			if (indiceBloco >= 0 && indiceBloco < blockchain.qtdBlocos) {
				verificarTransacao(&blockchain, indiceBloco, transacaoParaVerificar);
			} else {
				printf("indice do bloco invalido.\n");
			}
			break;

		case 7: // Simular ataque
			printf("Digite a nova transacao para substituir no bloco genesis: ");
			fgets(novaTransacao, sizeof(novaTransacao), stdin);
			novaTransacao[strcspn(novaTransacao, "\n")] = '\0'; // Remover o newline
			simularAtaque(&blockchain, novaTransacao);
			break;
		case 8:
			printf("Deseja iniciar a inserC'C#o de transaC'C5es automC!ticas para testes?\n");
			printf("1 - Sim | 0 - NC#o\n");

			int aux;
			scanf("%d", &aux);
			getchar(); // Consome o newline do scanf

			if (aux != 1 && aux != 0) { // Corrigido o erro lC3gico
				printf("Entrada invC!lida! Digite 0 ou 1\n");
				break;
			}

			if (aux == 1) {
				inserirTransacoesEMinerar(&blockchain);
			}

			// Continua para o menu principal apC3s a execuC'C#o
			break;

		case 0: // Sair
			printf("Encerrando o programa.\n");
			return 0;

		default:
			printf("OpC'C#o invC!lida! Tente novamente.\n");
			break;
		}
	}

	return 0;
}



void inserirTransacoesEMinerar(Blockchain* blockchain) {
    char transacao[256];
    int blocosMinerados = 0;

    while (blocosMinerados < 20) {
        sprintf(transacao, "Transação automática #%d", blockchain->qtdBlocos);
        adicionarTransacao(&blockchain->blocos[blockchain->qtdBlocos - 1], transacao);
        printf("Transação '%s' adicionada ao bloco #%d\n", transacao, blockchain->qtdBlocos - 1);

        // Minera um novo bloco a cada 3 transações
        if (blockchain->blocos[blockchain->qtdBlocos - 1].qtdTransacoes % 3 == 0) {
            Bloco novoBloco;
            inicializarBloco(&novoBloco, blockchain->blocos[blockchain->qtdBlocos - 1].hash, blockchain->qtdBlocos);
            provaDeTrabalho(&novoBloco);
            adicionarBloco(blockchain, novoBloco);
            ajustarDificuldade(blockchain);

            blocosMinerados++;
            printf("Novo bloco #%d minerado com sucesso!\n", blockchain->qtdBlocos - 1);
        }
    }

    printf("\n✅ 20 blocos minerados automaticamente!\n");
    printf("Pressione ENTER para voltar ao menu principal...\n");
    getchar(); // Aguarda ENTER antes de voltar ao menu
}






