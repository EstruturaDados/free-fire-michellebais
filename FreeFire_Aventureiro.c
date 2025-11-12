#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITENS 10  // Capacidade máxima da mochila

// Definição da struct Item
typedef struct {
    char nome[30];     // Nome do item (ex: "Pistola")
    char tipo[20];     // Tipo do item (ex: "Arma", "Munição", "Cura")
    int quantidade;    // Quantidade do item
} Item;

void adicionarItem(Item mochila[], int *total);
void removerItem(Item mochila[], int *total);
void listarItens(Item mochila[], int total);
void buscarItem(Item mochila[], int total);
int nomeDuplicado(Item mochila[], int total, char nome[]);

// Função principal
int main() {
    Item mochila[MAX_ITENS];
    int total = 0;   // Quantidade atual de itens na mochila
    int opcao;

    do {
        printf("\n* * * MOCHILA DE SOBREVIVENCIA * * *\n");
        printf("Itens da mochila %d/%d\n", total, MAX_ITENS);
        printf("1. Adicionar item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar item\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer de entrada

        switch(opcao) {
            case 1:
                adicionarItem(mochila, &total);
                break;
            case 2:
                removerItem(mochila, &total);
                break;
            case 3:
                listarItens(mochila, total);
                break;
            case 4:
                buscarItem(mochila, total);
                break;
            case 0:
                printf("Até a proxima!!\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }

    } while(opcao != 0);

    return 0;
}

int nomeDuplicado(Item mochila[], int total, char nome[]) { //pra verificar se já nao existe item igual
    for (int i = 0; i < total; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            return 1; // Já existe item com esse nome
        }
    }
    return 0;
}

// Função para Adicionar item
void adicionarItem(Item mochila[], int *total) {
    if (*total >= MAX_ITENS) {
        printf("\nMochila cheia! Não é possível adicionar mais nada!!\n");
        return;
    }

    Item novoItem;

    printf("\nNome do item (ex. arco, rifle, bastao, atadura...): ");
    fgets(novoItem.nome, sizeof(novoItem.nome), stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = '\0'; // Remove o \n


    if (strlen(novoItem.nome) == 0) {     // Não pode ficar sem nome (depois tem a busca)
        printf("Erro: o nome do item não pode estar vazio!\n");
        return;
    }

    if (nomeDuplicado(mochila, *total, novoItem.nome)) {        // Não pode ter nome duplicado
        printf("Erro: já existe um item com o nome '%s' na mochila!\n", novoItem.nome);
        return;
    }

    printf("Digite o tipo do item (ex: Arma, Munição, Cura): ");
    fgets(novoItem.tipo, sizeof(novoItem.tipo), stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = '\0';

    if (strlen(novoItem.tipo) == 0) {
        printf("Erro: o tipo do item não pode estar vazio!\n");
        return;
    }

    printf("Digite a quantidade: ");
    scanf("%d", &novoItem.quantidade);
    getchar();

    if (novoItem.quantidade <= 0) {
        printf("Erro: a quantidade deve ser maior que zero!\n");
        return;
    }

    mochila[*total] = novoItem;     // Item adicionado
    (*total)++;

    printf("\nItem cadastrado com sucesso!\n");
}

// Função para remover item
void removerItem(Item mochila[], int *total) {
    if (*total == 0) {
        printf("\nA mochila está vazia! Nada para remover.\n");
        return;
    }

    char nomeRemover[30];
    printf("\nDigite o nome do item a remover: ");
    fgets(nomeRemover, sizeof(nomeRemover), stdin);
    nomeRemover[strcspn(nomeRemover, "\n")] = '\0';

    int encontrado = 0;
    for (int i = 0; i < *total; i++) {
        if (strcmp(mochila[i].nome, nomeRemover) == 0) {
            for (int j = i; j < *total - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            (*total)--;
            encontrado = 1;
            printf("\nItem '%s' removido com sucesso!\n", nomeRemover);
            break;
        }
    }

    if (!encontrado) {
        printf("\nItem não encontrado na mochila.\n");
    }
}

// Função para listar itens
void listarItens(Item mochila[], int total) {
    if (total == 0) {
        printf("\nA mochila está vazia.\n");
        return;
    }

    printf("\n=== Itens na mochila ===\n");
    for (int i = 0; i < total; i++) {
        printf("%d. Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// Função para buscar item
void buscarItem(Item mochila[], int total) {
    if (total == 0) {
        printf("\nA mochila está vazia! Nenhum item para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item que deseja buscar: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    for (int i = 0; i < total; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\nItem encontrado!\n");
            printf("Nome: %s\nTipo: %s\nQuantidade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            return;
        }
    }

    printf("\nItem não encontrado.\n");
}
