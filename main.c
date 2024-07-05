/* 
    TODO:
        * Melhorar o menu - CLI em geral
        * Automatizar a variavel ID
        * Função de consulta:
            * por ID
            * Geral, ordenada
        * Funçaõ de ordenação: 
            * bubble sort por data
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define true 1
#define false 0

typedef int bool;

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
        printf("ID: %d, Name: %s, Quantity: %d, Date: %02d/%02d - %02d:%02d:%02d\n", end->reg.ID, end->reg.nome, end->reg.quantidade, end->reg.data.dia, end->reg.data.mes, end->reg.data.hora, end->reg.data.minuto, end->reg.data.segundo);
        end = end->prox;
    }
} 

bool insere(LISTA* l, PRODUTO reg) {
    /* if (pos < 0 || pos > tamanho(l))
        return false; */
    
    ELEMENTO *novo = (ELEMENTO*)malloc(sizeof(ELEMENTO));
    novo->reg = reg;
    novo->prox = l->inicio;
    l->inicio = novo;
    /* if (pos == 0) {
        novo->prox = l->inicio;
        l->inicio = novo;
    } else {
        PONT p = l->inicio;
        for (int i = 0; i < pos - 1; i++) p = p->prox;
        novo->prox = p->prox;
        p->prox = novo;
    } */
    return true;
}

bool exclui(LISTA* l, int pos) {
    if (pos < 0 || pos > tamanho(l) - 1)
        return false;
    
    PONT apagar;
    if (pos == 0) {
        apagar = l->inicio;
        l->inicio = apagar->prox;
    } else {
        PONT p = l->inicio;
        for (int i = 0; i < pos - 1; i++) p = p->prox;
        apagar = p->prox;
        p->prox = apagar->prox;
    }
    free(apagar);
    return true;
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

void menu() {
    printf("Escolha uma opção:\n");
    printf("1. Adicionar produto\n");
    printf("2. Tamanho da lista\n");
    printf("4. Listar produtos\n");
    printf("5. Sair\n");
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
            printf("Digite o ID do produto: ");
            scanf("%d", &produto.ID);
            getchar();
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
        } else if (opcao == 4) {
            system("clear");
            bubbleSortPorData(&listaProdutos);
            exibirLista(&listaProdutos);
        }

    } while (opcao != 5);

    return 0;
}