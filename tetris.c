#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5

// Estrutura da peça
struct Peca {
    char nome;
    int id;
};

// Estrutura da fila circular
struct Fila {
    struct Peca pecas[TAMANHO_FILA];
    int inicio;
    int fim;
    int total;
};

// Função para gerar uma nova peça aleatória
struct Peca gerarPeca(int id) {
    struct Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    nova.nome = tipos[rand() % 7];
    nova.id = id;
    return nova;
}

// Função para inicializar a fila
void inicializarFila(struct Fila *fila) {
    fila->inicio = 0;
    fila->fim = 0;
    fila->total = 0;
}

// Verifica se a fila está cheia
int filaCheia(struct Fila *fila) {
    return fila->total == TAMANHO_FILA;
}

// Verifica se a fila está vazia
int filaVazia(struct Fila *fila) {
    return fila->total == 0;
}

// Enfileirar (inserir peça no final)
void enfileirar(struct Fila *fila, struct Peca nova) {
    if (filaCheia(fila)) {
        printf("A fila esta cheia! Nao e possivel adicionar nova peca.\n");
        return;
    }
    fila->pecas[fila->fim] = nova;
    fila->fim = (fila->fim + 1) % TAMANHO_FILA;
    fila->total++;
}

// Desenfileirar (remover peça da frente)
struct Peca desenfileirar(struct Fila *fila) {
    struct Peca removida = {'-', -1};

    if (filaVazia(fila)) {
        printf("A fila esta vazia! Nenhuma peca para jogar.\n");
        return removida;
    }

    removida = fila->pecas[fila->inicio];
    fila->inicio = (fila->inicio + 1) % TAMANHO_FILA;
    fila->total--;

    return removida;
}

// Exibir o estado atual da fila
void exibirFila(struct Fila *fila) {
    printf("\n--- Fila de Pecas Futuras ---\n");

    if (filaVazia(fila)) {
        printf("Fila vazia.\n");
        return;
    }

    int i, index;
    for (i = 0; i < fila->total; i++) {
        index = (fila->inicio + i) % TAMANHO_FILA;
        printf("Posicao %d -> Peca: %c | ID: %d\n", i + 1,
               fila->pecas[index].nome, fila->pecas[index].id);
    }
    printf("-----------------------------\n");
}

int main() {
    struct Fila fila;
    int opcao;
    int proximoId = 1;

    srand(time(NULL));
    inicializarFila(&fila);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enfileirar(&fila, gerarPeca(proximoId++));
    }

    do {
        printf("\n===== TETRIS STACK - FILA DE PECAS =====\n");
        printf("1. Visualizar fila\n");
        printf("2. Jogar peca (remover da frente)\n");
        printf("3. Inserir nova peca\n");
        printf("0. Sair\n");
        printf("========================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                exibirFila(&fila);
                break;

            case 2: {
                struct Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1) {
                    printf("Peca jogada: %c | ID: %d\n", jogada.nome, jogada.id);
                }
                break;
            }

            case 3:
                enfileirar(&fila, gerarPeca(proximoId++));
                printf("Nova peca adicionada ao final da fila.\n");
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
