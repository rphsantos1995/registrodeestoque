// https://github.com/andreypereira08/registrodeestoque/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

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

// Le os produtos do 'estoque.bin' e insere na lista
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

// retorna quantidade de produtos
int tamanho(LISTA* l) {
    PONT end = l->inicio;
    int tam = 0;
    while (end != NULL) {
        tam++;
        end = end->prox;
    }
    return tam;
}

// Exibe os produtos salvos
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

// Le o ID do 'global_id.txt'
void load_global_id() {
    FILE *file = fopen("global_id.txt", "r+");
    if (file != NULL) {
        fscanf(file, "%d", &globalID);
        fclose(file);
    }
}

// Salva o proximo ID no 'global_id.txt'
void save_global_id() {
    FILE *file = fopen("global_id.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", globalID);
        fclose(file);
    }
}

void bubbleSortPorData(LISTA *listaprodutos);
int comparacaoDatas(DATA d1, DATA d2);

// Registra produtos da lista no 'estoque.bin'
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

// Registra a adição do produto em 'log.txt'
void Registra(PRODUTO reg) {
    FILE *pa = fopen("log.txt", "a");
    if (!pa) {
        printf("Erro ao abrir o log.\n");
        exit(1);
    }
    fprintf(pa, "Foi acrescentado %d unidade(s) do produto %s (ID: %d) no estoque.\n", reg.quantidade, reg.nome, reg.ID);
    fclose(pa);
}

// Registra a exclusão do produto em 'log.txt'
void RegistraSaida(PRODUTO reg) {
    FILE *pa = fopen("log.txt", "a");
    if (!pa) {
        printf("Erro ao abrir o log.\n");
        exit(1);
    }
    fprintf(pa, "Foi excluido %d unidade(s) do produto %s (ID: %d) do estoque.\n", reg.quantidade, reg.nome, reg.ID);
    fclose(pa);
}

// Insere o elemento da lista, Organiza a lista e atualiza o 'estoque.bin'
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

// Exclui o elemento da lista por ID, Organiza a lista e atualiza o 'estoque.bin'
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

// Altera a quantidade e data
void altera(LISTA *l, int quantidade, PRODUTO produto){
  PONT atual = l->inicio;

    while (atual != NULL) {
        if (strcmp(atual->reg.nome, produto.nome) == 0) {
            atual->reg.quantidade += quantidade;
            atual->reg.data = produto.data;
        }
        atual = atual->prox;
    }
    atualizaestoque(l);
}

// Verifica se produto está no estoque
int estaNoEstoque(LISTA listaProdutos, PRODUTO produto) {
    PONT atual = listaProdutos.inicio;

    while (atual != NULL) {
        if (strcmp(atual->reg.nome, produto.nome) == 0) {
            return 1;
        }
        atual = atual->prox;
    }
    return 0;
}

// ---------------
void reinicializarLista(LISTA* l) {
    PONT end = l->inicio;
    while (end != NULL) {
        PONT apagar = end;
        end = end->prox;
        free(apagar);
    }
    l->inicio = NULL;
}

// Busca na lista o elemento por nome
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

// Apresenta as opções na interface
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

// Função auxiliar do bubblesort
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

// Ordena a lista por data
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

// Verifica se o valor digitado é um numero
int ehNumero(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0; // Não é um número
        }
    }
    return 1; // É um número
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
            
            //definindo DATA do produto
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            produto.data.dia = tm.tm_mday;
            produto.data.mes = tm.tm_mon + 1;
            produto.data.hora = tm.tm_hour;
            produto.data.minuto = tm.tm_min;
            produto.data.segundo = tm.tm_sec;
            printf("Digite o nome do produto: ");
            scanf("%s", produto.nome);

            if(estaNoEstoque(listaProdutos, produto)){
              printf("\nEsse produto já existe");
              printf("\nDigite uma quantidade para acresentar: ");
              char quant[20];
              scanf("%s", quant);
              if(ehNumero(quant)){
                int quantidade = atoi(quant);

                altera(&listaProdutos, quantidade, produto);
                printf("\nProduto Atualizado com sucesso!\n");
              } else {
                printf("\nQuantidade invalida!! Tente um numero!\n");
              }
            } else {
              printf("Digite a quantidade do produto: ");
              char quant[20];
              scanf("%s", quant);
              if(ehNumero(quant)){
                produto.quantidade = atoi(quant);

                insere(&listaProdutos, produto);
                printf("\nProduto adicionado com sucesso!\n");
              } else {
                printf("\nQuantidade invalida!! Tente um numero!\n");
              }

            }

        } else if (opcao == 2) {
            int id;
            printf("=============== Remover Produto ===============\n\n");
            if (tamanho(&listaProdutos) == 0) {
                printf("Lista vazia - sem elementos para Remover!\n\n");
            } else {
              printf("ID do item: ");
              char idProd[20];
              scanf("%s", idProd);
              if(ehNumero(idProd)){
                int id = atoi(idProd);
                exclui(&listaProdutos, id);
              } else {
                printf("\n ID invalido!! Tente um numero!\n");
              }
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
