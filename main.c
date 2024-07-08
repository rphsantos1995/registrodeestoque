/*
    TODO:
        * Melhorar o menu - CLI em geral
        * Automatizar a variavel ID - OK
        * Função de consulta:
            * por ID
            * Geral, ordenada
        * Funçaõ de ordenação:
            * bubble sort por data
 */
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

void exibirLista(LISTA* l) {
    PONT end = l->inicio;
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

void atualizaestoque(LISTA *l){
  FILE *arquivo = fopen("estoque.bin", "wb");
  if (arquivo == NULL) return;

  ELEMENTO *atual = l->inicio;
  while (atual != NULL) {
      fwrite(&atual->reg, sizeof(PRODUTO), 1, arquivo);
      atual = atual->prox;
  }
  fclose(arquivo);
}

void insere(LISTA *l, PRODUTO reg) {
    ELEMENTO *novo = (ELEMENTO*)malloc(sizeof(ELEMENTO));
    load_global_id();
    reg.ID = globalID++;
    save_global_id();
    novo->reg = reg;
    novo->prox = l->inicio;
    l->inicio = novo;
    bubbleSortPorData(l);
    atualizaestoque(l);
}

void exclui(LISTA* l, int id) {
    ELEMENTO *atual = l->inicio;
    ELEMENTO *anterior = NULL;

    while(atual != NULL && atual->reg.ID != id){
      anterior = atual;
      atual = atual->prox;
    }

    if (atual != NULL) {
      if (anterior == NULL) {
          l->inicio = atual->prox;
        } else {
          anterior->prox = atual->prox;
        }

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
        else{
            printf("Produto '%s' encontrado. Existem %d unidades dele no estoque!!\n\n", atual->reg.nome, atual->reg.quantidade);
            return true;  // Elemento encontrado
        }
    }
    printf("Produto nao encontrado!!\n\n");
    return false;  // Elemento não encontrado
}


void menu() {
    printf("Escolha uma opção:\n");
    printf("1. Adicionar produto\n");
    printf("2. Tamanho da lista\n");
    printf("3. Remover da lista\n");
    printf("4. Listar produtos\n");
    printf("5. Buscar produtos\n");
    printf("6. Sair\n");
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

    int opcao;
    do {
        menu();
        scanf("%d", &opcao);
        getchar();

        if (opcao == 1) {
            PRODUTO produto;
            system("clear");
            printf("Digite o nome do produto: ");
            scanf("%s", &produto.nome[0]);
            printf("Digite a quantidade do produto: ");
            scanf("%d", &produto.quantidade);

            //Alimentando variavel data: https://stackoverflow.com/questions/1442116/how-to-get-the-date-and-time-values-in-a-c-program
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            produto.data.dia = tm.tm_mday;
            produto.data.mes = tm.tm_mon + 1;
            produto.data.hora = tm.tm_hour;
            produto.data.minuto = tm.tm_min;
            produto.data.segundo = tm.tm_sec;

            insere(&listaProdutos, produto);
        } else if (opcao == 2){
            system("clear");
            printf("Tamanho da lista: %i\n", tamanho(&listaProdutos));
        } else if (opcao == 3){
            int id;
            system("clear");
            if (tamanho(&listaProdutos) == 0){
              printf("Lista vazia - sem elementos para remover!\n\n");
              continue;
            }
            else {
              printf("======================== Removendo item da lista!!!! ========================\n");
              printf("ID do item: ");
              scanf("%d", &id);
              exclui(&listaProdutos, id);
            }
        } else if (opcao == 4) {
            system("clear");
            bubbleSortPorData(&listaProdutos);
            exibirLista(&listaProdutos);
        } else if (opcao == 5) {
            system("clear");
            char nomeProduto[50];
            printf("Digite o nome do produto: ");
            scanf("%s", nomeProduto);
            buscaSeqPorNome(&listaProdutos, nomeProduto);
        }

    } while (opcao != 6);

    return 0;
}
