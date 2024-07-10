// https://github.com/andreypereira08/registrodeestoque/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define true 1
#define false 0

typedef int bool;

int globalID = 0;

typedef struct {
    int dia;
    int mes;
    int hora;
    int minuto;
    int segundo;
} DATA;

typedef struct {
    int ID;
    char nome[50];
    int quantidade;
    DATA data;
} PRODUTO;

typedef struct aux {
    PRODUTO reg;
    struct aux* prox;
} ELEMENTO;

typedef ELEMENTO *PONT;

typedef struct {
    PONT inicio;
} LISTA;

void inicializarLista(LISTA* l) {
    l->inicio = NULL;

    FILE *arquivo = fopen("estoque.bin", "rb");
    if (arquivo != NULL) {
        PRODUTO produto;
        while (fread(&produto, sizeof(PRODUTO), 1, arquivo)) {
            ELEMENTO *novo = (ELEMENTO*)malloc(sizeof(ELEMENTO));
            novo->reg = produto;
            novo->prox = l->inicio;
            l->inicio = novo;
        }
        fclose(arquivo);
    }
}

int tamanho(LISTA* l) {
    PONT end = l->inicio;
    int tam = 0;
    while (end != NULL) {
        tam++;
        end = end->prox;
    }
    return tam;
}

void exibirLista(LISTA *l) {
    PONT end = l->inicio;
    printf("=============== Lista de Produtos ===============\n\n");
    while (end != NULL) {
        printf("ID: %d\n", end->reg.ID);
        printf("Produto: %s\n", end->reg.nome);
        printf("Quantidade: %d\n", end->reg.quantidade);
        printf("Criado em: %02d/%02d - %02d:%02d:%02d\n", end->reg.data.dia, end->reg.data.mes, end->reg.data.hora, end->reg.data.minuto, end->reg.data.segundo);
        printf("-------------------------------------------\n");
        end = end->prox;
    }
}

void load_global_id() {
    FILE *file = fopen("global_id.txt", "r+");
    if (file != NULL) {
        fscanf(file, "%d", &globalID);
        fclose(file);
    }
}

void save_global_id() {
    FILE *file = fopen("global_id.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", globalID);
        fclose(file);
    }
}

void bubbleSortPorData(LISTA *listaprodutos);
int comparacaoDatas(DATA d1, DATA d2);

void atualizaestoque(LISTA *l) {
    FILE *arquivo = fopen("estoque.bin", "wb");
    if (arquivo == NULL) return;

    ELEMENTO *atual = l->inicio;
    while (atual != NULL) {
        fwrite(&atual->reg, sizeof(PRODUTO), 1, arquivo);
        atual = atual->prox;
    }
    fclose(arquivo);
}

void Registra(PRODUTO reg) {
    FILE *pa = fopen("log.txt", "a");
    if (!pa) {
        printf("Erro ao abrir o log.\n");
        exit(1);
    }
    fprintf(pa, "Foi acrescentado %d unidade(s) do produto %s (ID: %d) no estoque.\n", reg.quantidade, reg.nome, reg.ID);
    fclose(pa);
}

void RegistraSaida(PRODUTO reg) {
    FILE *pa = fopen("log.txt", "a");
    if (!pa) {
        printf("Erro ao abrir o log.\n");
        exit(1);
    }
    fprintf(pa, "Foi excluido %d unidade(s) do produto %s (ID: %d) do estoque.\n", reg.quantidade, reg.nome, reg.ID);
    fclose(pa);
}

void insere(LISTA *l, PRODUTO reg) {
    ELEMENTO *novo = (ELEMENTO*)malloc(sizeof(ELEMENTO));
    reg.ID = globalID++;
    save_global_id();
    Registra(reg);
    novo->reg = reg;
    novo->prox = l->inicio;
    l->inicio = novo;
    bubbleSortPorData(l);
    atualizaestoque(l);
}

void exclui(LISTA* l, int id) {
    ELEMENTO *atual = l->inicio;
    ELEMENTO *anterior = NULL;

    while (atual != NULL && atual->reg.ID != id) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual != NULL) {
        if (anterior == NULL) {
            l->inicio = atual->prox;
        } else {
            anterior->prox = atual->prox;
        }

        RegistraSaida(atual->reg);
        free(atual);
        printf("\nItem excluido com sucesso!!\n");
        bubbleSortPorData(l);
        atualizaestoque(l);
    } else {
        printf("\nItem não encontrado! Tente outro ID!!\n");
    }
}

void reinicializarLista(LISTA* l) {
    PONT end = l->inicio;
    while (end != NULL) {
        PONT apagar = end;
        end = end->prox;
        free(apagar);
    }
    l->inicio = NULL;
}

bool buscaSeqPorNome(LISTA *l, char *nome) {
    ELEMENTO *atual = l->inicio;
    while (atual != NULL) {
        if (strcmp(atual->reg.nome, nome) != 0)
            atual = atual->prox;
        else {
            printf("Produto '%s' encontrado. Existem %d unidades dele no estoque!!\n\n", atual->reg.nome, atual->reg.quantidade);
            return true;  // Elemento encontrado
        }
    }
    printf("Produto nao encontrado no estoque!!\n\n");
    return false;  // Elemento não encontrado
}

void menu() {
    printf("\n=============== Menu de Opcoes ===============\n");
    printf("1. Adicionar produto\n");
    printf("2. Remover Produto\n");
    printf("3. Tamanho da lista\n");
    printf("4. Listar produtos\n");
    printf("5. Buscar produtos\n");
    printf("6. Sair\n");
    printf("==============================================\n\n");
}

int comparacaoDatas(DATA d1, DATA d2) {
    if (d1.mes < d2.mes) return -1;
    if (d1.mes > d2.mes) return 1;
    if (d1.dia < d2.dia) return -1;
    if (d1.dia > d2.dia) return 1;
    if (d1.hora < d2.hora) return -1;
    if (d1.hora > d2.hora) return 1;
    if (d1.minuto < d2.minuto) return -1;
    if (d1.minuto > d2.minuto) return 1;
    if (d1.segundo < d2.segundo) return -1;
    if (d1.segundo > d2.segundo) return 1;
    return 0;
}

void bubbleSortPorData(LISTA *listaprodutos) {
    if (listaprodutos->inicio == NULL)
        return;

    bool troca;
    ELEMENTO *atual;
    ELEMENTO *proximo = NULL;

    do {
        troca = false;
        atual = listaprodutos->inicio;

        while (atual->prox != proximo) {
            if (comparacaoDatas(atual->reg.data, atual->prox->reg.data) < 0) {
                PRODUTO temp = atual->reg;
                atual->reg = atual->prox->reg;
                atual->prox->reg = temp;
                troca = true;
            }
            atual = atual->prox;
        }
        proximo = atual;
    } while (troca);
}

int main() {
    LISTA listaProdutos;
    inicializarLista(&listaProdutos);
    load_global_id();

    int opcao;
    do {
        menu();
        printf("Digite a opção desejada: ");
        scanf("%d", &opcao);
        getchar();

        system("clear");

        if (opcao == 1) {
            PRODUTO produto;
            printf("=============== Adicionar Produto ===============\n\n");
            printf("Digite o nome do produto: ");
            scanf("%s", produto.nome);
            printf("Digite a quantidade do produto: ");
            scanf("%d", &produto.quantidade);

            //definindo DATA do produto
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            produto.data.dia = tm.tm_mday;
            produto.data.mes = tm.tm_mon + 1;
            produto.data.hora = tm.tm_hour;
            produto.data.minuto = tm.tm_min;
            produto.data.segundo = tm.tm_sec;

            insere(&listaProdutos, produto);
            printf("\nProduto adicionado com sucesso!\n");
        } else if (opcao == 2) {
            int id;
            printf("=============== Remover Produto ===============\n\n");
            if (tamanho(&listaProdutos) == 0) {
                printf("Lista vazia - sem elementos para Remover!\n\n");
            } else {
                printf("ID do item: ");
                scanf("%d", &id);
                exclui(&listaProdutos, id);
            }
        } else if (opcao == 3) {
            printf("=============== Tamanho do Estoque ===============\n\n");
            printf("Existem %i produtos no estoque.\n\n", tamanho(&listaProdutos));
        } else if (opcao == 4) {
            bubbleSortPorData(&listaProdutos);
            exibirLista(&listaProdutos);
        } else if (opcao == 5) {
            printf("=============== Buscar Produto ===============\n\n");
            if (tamanho(&listaProdutos) == 0) {
              printf("Lista vazia - sem elementos para Buscar!\n\n");
            } else {
              char nomeProduto[50];
              printf("Digite o nome do produto: ");
              scanf("%s", nomeProduto);
              buscaSeqPorNome(&listaProdutos, nomeProduto);
            }
        } else if (opcao != 6) {
            printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 6);

    printf("Saindo...\n");
    return 0;
}
