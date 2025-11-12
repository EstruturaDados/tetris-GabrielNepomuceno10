#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

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

// Estrutura da pilha
struct Pilha {
    struct Peca pecas[TAMANHO_PILHA];
    int topo;
};

// Função para gerar uma nova peça aleatória
struct Peca gerarPeca(int id) {
    struct Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    nova.nome = tipos[rand() % 7];
    nova.id = id;
    return nova;
}

// Inicializa a fila
void inicializarFila(struct Fila *fila) {
    fila->inicio = 0;
    fila->fim = 0;
    fila->total = 0;
}

// Inicializa a pilha
void inicializarPilha(struct Pilha *pilha) {
    pilha->topo = -1;
}

// Verifica se a fila está cheia
int filaCheia(struct Fila *fila) {
    return fila->total == TAMANHO_FILA;
}

// Verifica se a fila está vazia
int filaVazia(struct Fila *fila) {
    return fila->total == 0;
}

// Verifica se a pilha está cheia
int pilhaCheia(struct Pilha *pilha) {
    return pilha->topo == TAMANHO_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(struct Pilha *pilha) {
    return pilha->topo == -1;
}

// Enfileirar (adiciona peça no final)
void enfileirar(struct Fila *fila, struct Peca nova) {
    if (filaCheia(fila)) {
        printf("A fila esta cheia! Nao e possivel adicionar nova peca.\n");
        return;
    }
    fila->pecas[fila->fim] = nova;
    fila->fim = (fila->fim + 1) % TAMANHO_FILA;
    fila->total++;
}

// Desenfileirar (remove peça da frente)
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

// Empilhar (reservar peça)
void empilhar(struct Pilha *pilha, struct Peca nova) {
    if (pilhaCheia(pilha)) {
        printf("A pilha de reserva esta cheia! Nao e possivel guardar mais pecas.\n");
        return;
    }
    pilha->topo++;
    pilha->pecas[pilha->topo] = nova;
    printf("Peca %c (ID %d) reservada com sucesso.\n", nova.nome, nova.id);
}

// Desempilhar (usar peça reservada)
struct Peca desempilhar(struct Pilha *pilha) {
    struct Peca removida = {'-', -1};

    if (pilhaVazia(pilha)) {
        printf("A pilha de reserva esta vazia! Nenhuma peca para usar.\n");
        return removida;
    }

    removida = pilha->pecas[pilha->topo];
    pilha->topo--;
    return removida;
}

// Exibir a fila
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

// Exibir a pilha
void exibirPilha(struct Pilha *pilha) {
    printf("\n--- Pilha de Pecas Reservadas ---\n");

    if (pilhaVazia(pilha)) {
        printf("Pilha vazia.\n");
        return;
    }

    for (int i = pilha->topo; i >= 0; i--) {
        printf("Posicao %d -> Peca: %c | ID: %d\n", i + 1,
               pilha->pecas[i].nome, pilha->pecas[i].id);
    }
    printf("-------------------------------\n");
}

int main() {
    struct Fila fila;
    struct Pilha pilha;
    int opcao;
    int proximoId = 1;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enfileirar(&fila, gerarPeca(proximoId++));
    }

    do {
        printf("\n===== TETRIS STACK - NIVEL AVENTUREIRO =====\n");
        printf("1. Jogar peca (remover da frente da fila)\n");
        printf("2. Reservar peca (guardar na pilha)\n");
        printf("3. Usar peca reservada (retirar da pilha)\n");
        printf("0. Sair\n");
        printf("============================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                struct Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1) {
                    printf("Peca jogada: %c | ID: %d\n", jogada.nome, jogada.id);
                    enfileirar(&fila, gerarPeca(proximoId++)); // mantém a fila sempre cheia
                }
                break;
            }

            case 2: {
                struct Peca frente = desenfileirar(&fila);
                if (frente.id != -1) {
                    empilhar(&pilha, frente);
                    enfileirar(&fila, gerarPeca(proximoId++)); // repõe a fila
                }
                break;
            }

            case 3: {
                struct Peca usada = desempilhar(&pilha);
                if (usada.id != -1) {
                    printf("Peca usada da reserva: %c | ID: %d\n", usada.nome, usada.id);
                }
                break;
            }

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

        // Exibe o estado atual das estruturas
        exibirFila(&fila);
        exibirPilha(&pilha);

    } while (opcao != 0);

    return 0;
}
