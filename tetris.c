#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3
#define HISTORICO_MAX 10

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

// Histórico para desfazer ações
struct Historico {
    struct Peca ultimaPeca;
    int tipoAcao; // 1 = jogada, 2 = reserva, 3 = uso
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

// Inicializa o histórico
void inicializarHistorico(struct Historico historico[], int *topoHist) {
    *topoHist = -1;
}

// Verificações básicas
int filaCheia(struct Fila *fila) {
    return fila->total == TAMANHO_FILA;
}

int filaVazia(struct Fila *fila) {
    return fila->total == 0;
}

int pilhaCheia(struct Pilha *pilha) {
    return pilha->topo == TAMANHO_PILHA - 1;
}

int pilhaVazia(struct Pilha *pilha) {
    return pilha->topo == -1;
}

// Enfileirar
void enfileirar(struct Fila *fila, struct Peca nova) {
    if (filaCheia(fila)) return;
    fila->pecas[fila->fim] = nova;
    fila->fim = (fila->fim + 1) % TAMANHO_FILA;
    fila->total++;
}

// Desenfileirar
struct Peca desenfileirar(struct Fila *fila) {
    struct Peca removida = {'-', -1};
    if (filaVazia(fila)) return removida;

    removida = fila->pecas[fila->inicio];
    fila->inicio = (fila->inicio + 1) % TAMANHO_FILA;
    fila->total--;
    return removida;
}

// Empilhar
void empilhar(struct Pilha *pilha, struct Peca nova) {
    if (pilhaCheia(pilha)) return;
    pilha->topo++;
    pilha->pecas[pilha->topo] = nova;
}

// Desempilhar
struct Peca desempilhar(struct Pilha *pilha) {
    struct Peca removida = {'-', -1};
    if (pilhaVazia(pilha)) return removida;
    removida = pilha->pecas[pilha->topo];
    pilha->topo--;
    return removida;
}

// Exibir fila
void exibirFila(struct Fila *fila) {
    printf("\n--- Fila de Pecas Futuras ---\n");
    if (filaVazia(fila)) {
        printf("Fila vazia.\n");
        return;
    }
    for (int i = 0; i < fila->total; i++) {
        int idx = (fila->inicio + i) % TAMANHO_FILA;
        printf("Posicao %d -> Peca: %c | ID: %d\n", i + 1,
               fila->pecas[idx].nome, fila->pecas[idx].id);
    }
}

// Exibir pilha
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
}

// Registrar no histórico
void registrarHistorico(struct Historico historico[], int *topoHist, struct Peca p, int tipo) {
    if (*topoHist < HISTORICO_MAX - 1) {
        (*topoHist)++;
        historico[*topoHist].ultimaPeca = p;
        historico[*topoHist].tipoAcao = tipo;
    }
}

// Desfazer última jogada
void desfazerUltima(struct Historico historico[], int *topoHist,
                    struct Fila *fila, struct Pilha *pilha) {
    if (*topoHist == -1) {
        printf("Nao ha jogadas para desfazer.\n");
        return;
    }

    struct Historico ultima = historico[*topoHist];
    (*topoHist)--;

    if (ultima.tipoAcao == 1) {
        // Recoloca peça jogada no início da fila
        if (!filaCheia(fila)) {
            fila->inicio = (fila->inicio - 1 + TAMANHO_FILA) % TAMANHO_FILA;
            fila->pecas[fila->inicio] = ultima.ultimaPeca;
            fila->total++;
            printf("Jogada desfeita: peca %c (ID %d) devolvida a fila.\n",
                   ultima.ultimaPeca.nome, ultima.ultimaPeca.id);
        }
    } else if (ultima.tipoAcao == 2) {
        // Remove da pilha se foi reserva
        if (!pilhaVazia(pilha)) {
            desempilhar(pilha);
            printf("Desfeita a reserva da peca %c (ID %d).\n",
                   ultima.ultimaPeca.nome, ultima.ultimaPeca.id);
        }
    } else if (ultima.tipoAcao == 3) {
        // Recoloca peça usada na pilha
        if (!pilhaCheia(pilha)) {
            empilhar(pilha, ultima.ultimaPeca);
            printf("Desfeita a acao de usar peca reservada.\n");
        }
    }
}

// Trocar peça do topo da pilha com a da frente da fila
void trocarPecas(struct Fila *fila, struct Pilha *pilha) {
    if (pilhaVazia(pilha) || filaVazia(fila)) {
        printf("Nao e possivel trocar: pilha ou fila vazia.\n");
        return;
    }
    struct Peca temp = pilha->pecas[pilha->topo];
    pilha->pecas[pilha->topo] = fila->pecas[fila->inicio];
    fila->pecas[fila->inicio] = temp;
    printf("Pecas trocadas entre topo da pilha e frente da fila.\n");
}

// Inverter todos os elementos entre pilha e fila
void inverterEstruturas(struct Fila *fila, struct Pilha *pilha) {
    struct Peca tempFila[TAMANHO_FILA];
    struct Peca tempPilha[TAMANHO_PILHA];
    int i, totalFila = fila->total, totalPilha = pilha->topo + 1;

    // Copia dados
    for (i = 0; i < totalFila; i++) {
        int idx = (fila->inicio + i) % TAMANHO_FILA;
        tempFila[i] = fila->pecas[idx];
    }
    for (i = 0; i < totalPilha; i++) {
        tempPilha[i] = pilha->pecas[i];
    }

    // Troca
    inicializarFila(fila);
    inicializarPilha(pilha);

    for (i = 0; i < totalPilha && i < TAMANHO_FILA; i++) {
        enfileirar(fila, tempPilha[i]);
    }
    for (i = 0; i < totalFila && i < TAMANHO_PILHA; i++) {
        empilhar(pilha, tempFila[i]);
    }

    printf("Fila e pilha foram invertidas com sucesso.\n");
}

int main() {
    struct Fila fila;
    struct Pilha pilha;
    struct Historico historico[HISTORICO_MAX];
    int topoHist;
    int opcao;
    int proximoId = 1;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    inicializarHistorico(historico, &topoHist);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enfileirar(&fila, gerarPeca(proximoId++));
    }

    do {
        printf("\n===== TETRIS STACK - NIVEL MESTRE =====\n");
        printf("1. Jogar peca\n");
        printf("2. Reservar peca\n");
        printf("3. Usar peca reservada\n");
        printf("4. Trocar peca do topo da pilha com a da frente da fila\n");
        printf("5. Desfazer ultima jogada\n");
        printf("6. Inverter fila com pilha\n");
        printf("0. Sair\n");
        printf("=======================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                struct Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1) {
                    printf("Peca jogada: %c | ID: %d\n", jogada.nome, jogada.id);
                    registrarHistorico(historico, &topoHist, jogada, 1);
                    enfileirar(&fila, gerarPeca(proximoId++));
                }
                break;
            }
            case 2: {
                struct Peca frente = desenfileirar(&fila);
                if (frente.id != -1) {
                    empilhar(&pilha, frente);
                    registrarHistorico(historico, &topoHist, frente, 2);
                    enfileirar(&fila, gerarPeca(proximoId++));
                }
                break;
            }
            case 3: {
                struct Peca usada = desempilhar(&pilha);
                if (usada.id != -1) {
                    printf("Peca usada da reserva: %c | ID: %d\n", usada.nome, usada.id);
                    registrarHistorico(historico, &topoHist, usada, 3);
                }
                break;
            }
            case 4:
                trocarPecas(&fila, &pilha);
                break;
            case 5:
                desfazerUltima(historico, &topoHist, &fila, &pilha);
                break;
            case 6:
                inverterEstruturas(&fila, &pilha);
                break;
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }

        exibirFila(&fila);
        exibirPilha(&pilha);

    } while (opcao != 0);

    return 0;
}
