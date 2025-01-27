# AEDS 1
Trabalho de AEDS 1 - Blockchain

gcc -o saida main.c -lcrypto

Lista de tarefas:

### 1. Estruturação da Blockchain
- **Definir a estrutura de um bloco**:
  - Índice do bloco.
  - Timestamp de criação.
  - Hash do bloco anterior.
  - Nonce para Proof of Work.
  - Hash do bloco atual.
  - Raiz da Merkle Tree representando as transações.

### 2. Implementação do Proof of Work (PoW)
- **Desenvolver a lógica para calcular o nonce** que satisfaz o critério de dificuldade (número de zeros iniciais no hash).
- **Implementar ajuste dinâmico de dificuldade** para o PoW.

### 3. Gerenciamento de Transações
- **Criar funcionalidade para inserir novas transações**:
  - Inserir transações representadas como strings de dados em um bloco.
- **Armazenar transações no bloco usando Merkle Tree**:
  - Estruturar as transações de forma hierárquica usando Merkle Tree.
  - Calcular a raiz da Merkle Tree para cada conjunto de transações.

### 4. Verificação de Transações com Proof of Inclusion
- **Desenvolver método de verificação**:
  - Verificar se uma transação específica está incluída em um bloco utilizando o caminho de prova (proof path) baseado no hash.

### 5. Adicionar Blocos à Blockchain
- **Implementar a funcionalidade para minerar e adicionar blocos**:
  - Minerar (resolver o PoW) e adicionar o bloco à blockchain após validação.

### 6. Interface do Programa
- **Inserir novas transações no bloco**.
- **Minerar e adicionar um bloco à blockchain**.
- **Exibir a blockchain completa**:
  - Mostrar índices, hashes dos blocos, e raízes das Merkle Trees.
- **Verificar se uma transação está em um bloco específico**.
- **Simular um ataque à rede**:
  - Alterar uma transação e demonstrar o impacto no ajuste de nonces.

### 7. Testes e Relatório
- **Desenvolver testes para cada funcionalidade**:
  - Testar inserção de transações, mineração de blocos, verificação de transações, etc.
- **Documentar os resultados**:
  - Criar um relatório detalhado explicando os testes realizados, os resultados e como cada funcionalidade foi implementada.

### 8. Preparação para Apresentação
- **Preparar a apresentação do trabalho**:
  - Explicar a implementação, desafios enfrentados, soluções adotadas, e demonstração das funcionalidades.
- **Envio do código e relatório**:
  - Garantir que o código e o relatório sejam enviados pelo menos 3 dias antes da data de apresentação. 

Essa lista abrange todos os passos necessários para a conclusão do trabalho sobre Blockchain com Proof of Work e Merkle Tree.
