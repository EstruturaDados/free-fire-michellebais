#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>    // Medição de tempo (clock, CLOCKS_PER_SEC)

#define MAX_COMP 20 // Constante: número máximo de componentes

// Definição da struct Componente (nome, tipo e prioridade)
typedef struct {
    char nome[30];
    char tipo[20]; 
    int prioridade;
} Componente;

// Protótipos das funções
void cadastrarComponentes(Componente comp[], int *n);
void removerComponentes(Componente comp[], int *n);
void mostrarComponentes(Componente comp[], int n);
void bubbleSortNome(Componente comp[], int n, int *comparacoes);
void insertionSortTipo(Componente comp[], int n, int *comparacoes);
void selectionSortPrioridade(Componente comp[], int n, int *comparacoes);
int buscaBinariaPorNome(Componente comp[], int n, char chave[], int *comparacoes);
void medirTempo(void (*algoritmo)(Componente[], int, int*), Componente comp[], int n, char descricao[]);
void copiarArray(Componente origem[], Componente destino[], int n);

// Função principal (menu interativo)
int main() {
    Componente componentes[MAX_COMP]; // struct tipo 'Componente', denominada 'componentes'
    int total = 0;                    // para contar quantos já foram cadastrados
    int opcao;                        // para usar no 1º Menu
    char chave[30];                   // para localizar o componente-chave para iniciar a montagem
    int comparacoes = 0;              // para contar número de comparações realizadas em cada ordenação

    do {
        printf("\n* * * * * * * * * * * * * * * * * * *\n");
        printf("\n          MONTAGEM DA TORRE          \n");
        printf("\n       intens na mochila: %d/%d      \n", total, MAX_COMP);
        printf("\n* * * * * * * * * * * * * * * * * * *\n");
        printf("1. Adicionar itens na mochila\n");
        printf("2. Retirar itns na mochila\n");
        printf("3. Organizar e visualizar Mochila\n");
        printf("4. Buscar itens na mochila (por nome)\n");
        printf("0. Ativar Torre SAIR\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o '\n' deixado pelo scanf

        switch (opcao) {
            case 1:
                cadastrarComponentes(componentes, &total);  //'&total' pega o valor que está no endereço
                break;

            case 2:
                removerComponentes(componentes, &total);
                break;
                
            case 3:
                mostrarComponentes(componentes, total); //'total' pega a cópia do valor que está no endereço
                break;

            case 4:
                if (total == 0) {   //se não houve nenhum cadastro
                    printf("Nenhum componente cadastrado.\n");
                    break;
                } //se houve, segue...
                printf("Digite o nome do componente a buscar: "); 
                fgets(chave, sizeof(chave), stdin);
                chave[strcspn(chave, "\n")] = '\0'; // Remove o '\n'

                comparacoes = 0;
                
                int pos = buscaBinariaPorNome(componentes, total, chave, &comparacoes); //a variável 'pos' recebe o resultado da função de 'buscaBinariaPorNome' 

                if (pos != -1) { //Se a busca foi diferente de -1, então mostra o componente
                    printf("\nComponente encontrado!\n");
                    printf("Nome: %s | Tipo: %s | Prioridade: %d\n",
                           componentes[pos].nome, componentes[pos].tipo, componentes[pos].prioridade);
                } else {
                    printf("\nComponente não encontrado.\n");
                }
                printf("Comparações na busca: %d\n", comparacoes);
                break;

            case 0:
                printf("Encerrando programa...\n");
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// Função para cadastro de novos componentes
void cadastrarComponentes(Componente comp[], int *n) { 
    if (*n >= MAX_COMP) {
        printf("Capacidade máxima atingida!\n");
        return;
    }

    Componente novo;    //para cada novo item a ser cadastrado

    printf("Nome do componente (ex. Chip Central, Ferro, Parafuso): ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0'; //remove o '\n' final da string para limpar pro proximo fgets

    printf("Tipo do componente (ex. Controle, Propulsão, Estrutura...): ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    
    do {    
        printf("Prioridade de 1 a 10: ");
        scanf("%d", &novo.prioridade);
        getchar();
        if (novo.prioridade < 1 || novo.prioridade > 10)    // garante prioridade válida entre 1 e 10
            printf("Prioridade inválida! Digite entre 1 e 10.\n");
    } while (novo.prioridade < 1 || novo.prioridade > 10);  //enquanto for inválida, vai executar novamente

    comp[*n] = novo;    //recebe os valores da struct denominada 'novo'
    (*n)++;

    printf("\nComponente cadastrado com sucesso!\n");
}

//FUNÇÃO para remover componentes
void removerComponentes(Componente comp[], int *n) {
    if (*n == 0) {    //se 'inicio' for NULL ou apontar para NULL, não há o que remover (lista vazia)
        printf("A lista está vazia, Nada a remover!!\n");
    } else {
        char nome[30];  //igual ao parametro do inicio
        printf("Digite o nome do componente que deseja remover: ");
        fgets(nome, sizeof(nome), stdin);
        nome[strcspn(nome, "\n")] = '\0'; //remove o '\n' do final

        int encontrado = -1;    //marcador inicial da busca: usamos '-1', que nunca é um índice válido pq '0' pode ser uma posição válida (por exemplo, o primeiro item do vetor)
        for (int i = 0; i < *n; i++) {
            if (strcmp(comp[i].nome, nome) == 0) { //compara as strings
                encontrado = i;
                break;
            }
        }

        if (encontrado == -1) {
            printf("Componente não encontrado.\n");
            return;
        }

        for (int i = encontrado; i < *n - 1; i++) { // Deslocar os elementos à esquerda para “tampar o buraco”
            comp[i] = comp[i + 1];
        }

        (*n)--; // diminui o total de componentes
        printf("\nComponente removido com sucesso!\n");
    }
}

// Função para Exibir todos os componentes na tela
void mostrarComponentes(Componente comp[], int n) {
    if (n == 0) {
        printf("Nenhum componente cadastrado.\n");
        return;
    }
    int opcao;
    
    do {
        printf("\n * * * * * * MENU LISTA * * * * * * \n");
        printf("1. Ordenar por nome\n");
        printf("2. Ordenar por tipo\n");
        printf("3. Ordenar por prioridade\n");
        printf("4. Mostrar lista atual\n");
        printf("0. Retornar ao menu\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                medirTempo(bubbleSortNome, comp, n, "Bubble Sort (Nome)");
                break;
            case 2:
                medirTempo(insertionSortTipo, comp, n, "Insertion Sort (Tipo)");
                break;
            case 3:
                medirTempo(selectionSortPrioridade, comp, n, "Selection Sort (Prioridade)");
                break;
            case 4:
                printf("\n * * * * * * LISTA ATUAL * * * * *\n");
                for (int i = 0; i < n; i++) {
                    printf("%d. Nome: %-20s | Tipo: %-15s | Prioridade: %d\n", i + 1, comp[i].nome, comp[i].tipo, comp[i].prioridade);
                }
                break;

            case 0:
                return;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

}


// Função para Ordenar por NOME (ordem alfabética): BUBBLE SORT
void bubbleSortNome(Componente comp[], int n, int *comparacoes) { //tem como argumento a struct denominada 'comp' eseus campos '[]', o 'n' pra contagem de "campos" a serem analisados e o ponteiro 'comparacoes'
    Componente temp; //declara 'temp' pra receber os valores de forma temporaria para as trocas entre os campos
    int trocou; //declara a int 'trocou' para receber os valores já no local correto (em ordem)
    for (int i = 0; i < n - 1; i++) { 
        trocou = 0;
        for (int j = 0; j < n - i - 1; j++) {
            (*comparacoes)++; // incrementa a comparação para seguir e no outro void contabilizar na contagem
            if (strcmp(comp[j].nome, comp[j + 1].nome) > 0) { //compara as strings
                temp = comp[j];         //'temp' recebe o valor de 'comp[j]'
                comp[j] = comp[j + 1];  //'comp[j]' recebe o valor de 'comp[j + 1]', que é o próximo campo
                comp[j + 1] = temp;     //'comp[j + 1]' recebe o valor de 'temp'
                trocou = 1;             //... está trocado... quando receber valor dieerente de 1, ou seja, 0, acabou
            }
        }
        if (!trocou) break;             //encerra quando já estiver ordenado
    }
}

// Função para Ordenar por TIPO (commpara strings): INSERTION SORT
void insertionSortTipo(Componente comp[], int n, int *comparacoes) { //igual ao BUBBLE
    for (int i = 1; i < n; i++) {   //começa do 2º elemento pois o primeiro é que rege a ordenação
        Componente chave = comp[i]; //declara 'chave' pra receber os valores de cada campo do 'comp[i]'

        int j = i - 1;  //i - 1 aponta para o elemento imediatamente anterior a i

        while (j >= 0 && (strcmp(comp[j].tipo, chave.tipo) > 0)) {  //move os elementos maiores que a 'chave' uma posição à frente
            (*comparacoes)++;           //incrementa a comparação para seguir e no outro void contabilizar na contagem
            comp[j + 1] = comp[j];      //começa a comparação para empurrar elementos à direita
            j--;    //decrementa para olhar os valores à esquerda
        } 
        comp[j + 1] = chave;    //encontrou o local correto 'chave' e coloca lá
    }
}


// Função para ordenar por PRIORIDADE: SELECTION SORT
void selectionSortPrioridade(Componente comp[], int n, int *comparacoes) {
    for (int i = 0; i < n - 1; i++) {   //laço externo: percorre as posições que precisam ser preenchidas 1 a 1 da esq. para dir.
        
        int min = i;    //Assume q o menor, por enquanto, é o elemento na posição 'i'

        for (int j = i + 1; j < n; j++) {   //laço interno: busca pelo menor elemento no resto do vetor (à direita de i)
            (*comparacoes)++;
            if (comp[j].prioridade < comp[min].prioridade)  //se encontrar um elemento ainda menor
                min = j;                                    //atualiza o nosso indice do menor
        }

        if (min != i) {        //Depois q passa pelo laço interno, o 'min' contem a posição do menor elemento, 
            Componente temp = comp[i];
            comp[i] = comp[min];
            comp[min] = temp;  // Troca de posição
        }
    }
}


// Função: Busca Binaria (após ordenar por nome)
int buscaBinariaPorNome(Componente comp[], int n, char chave[], int *comparacoes) {
    int inicio = 0, fim = n - 1;
    while (inicio <= fim) {  //O loop continua enquanto a 'fatia' de busca for valida (inicio <=fim).
        int meio = (inicio + fim) / 2; //define o meio da lista
        (*comparacoes)++;   //para usar no contador depois

        int cmp = strcmp(chave, comp[meio].nome);
        if (cmp == 0) {   //Se o valor estiver exatamente no meio, encontramos!
            return meio;
        } else if (cmp < 0) {   //Se o valor no meio for menor que o valor buscado, ele está na metade à direita (maior)
            fim = meio - 1;     //Buscamos na metade esqquerda
        } else {                //Se o valor no meio for maioor que o valor buscado, ele está na metade à esquerda (menor)
            inicio = meio + 1;  //Buscamos na metade direita
        }
    }
    return -1; // Não encontrado
}


// Função: Medir Tempo e imprimir as listas
void medirTempo(void (*algoritmo)(Componente[], int, int*), Componente comp[], int n, char descricao[]) {
    if (n == 0) {
        printf("Nenhum componente para ordenar!\n");
        return;
    }

    Componente copia[MAX_COMP];
    copiarArray(comp, copia, n); // Evita modificar o vetor original

    int comparacoes = 0;
    clock_t inicio = clock(); //Mostrar o tempo de execução de cada algoritmo (usar clock())
    algoritmo(copia, n, &comparacoes);
    clock_t fim = clock();

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\n=== %s ===\n", descricao);
    for (int i = 0; i < n; i++) {
        printf("%d. Nome: %-20s | Tipo: %-15s | Prioridade: %d\n",
               i + 1, copia[i].nome, copia[i].tipo, copia[i].prioridade);
    }
    printf("\nComparações: %d\n", comparacoes);
    printf("Tempo de execução: %.6f segundos\n", tempo);
}


// Função: Copiar o Array para não modificar o valor original com a contagem
void copiarArray(Componente origem[], Componente destino[], int n) {
    for (int i = 0; i < n; i++) {
        destino[i] = origem[i];
    }
}
