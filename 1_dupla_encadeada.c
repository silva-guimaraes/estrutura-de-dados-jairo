
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>


// node da lista encadeada
typedef struct aluno {
    char* nome;
    char* telefone;
    int idade;
    struct aluno *next, *prev;
} aluno;

typedef struct lista_duplamente_encadeada {
    aluno *primeiro, *ultimo;
    int tamanho;
} lista_de;

// localizar por indice = lista[indice]. nao o que ele pediu
aluno* localizar_indice(lista_de* l, int indice) {
    aluno* alvo = l->primeiro;

    for (int i = 0; alvo != NULL && i < indice; i++, alvo = alvo->next);
    assert(alvo);
    return alvo;
}

aluno* localizar(lista_de* l, char* nome) {
    for (aluno* a = l->primeiro; a != NULL; a = a->next) {
        if (strcmp(a->nome, nome) == 0) {
            // printf("nome: %s, idade: %d, tel: %s\n", a->nome, a->idade, a->telefone);
            return a;
        }
    }

    return NULL;
    // printf("\n");
}

void inserir(lista_de* lista, int indice, aluno* a) {

    if (indice == -1) 
        indice = lista->tamanho-1;

    // se elemento sendo adicionado sera o ultimo.
    bool is_ultimo = (indice == lista->tamanho-1);
    // se elemento sendo adicionado será o primeiro.
    bool is_primeiro = (indice == 0);

    lista->tamanho++;

    // caso nenhum elemento esteja presente na lista, fazer com que primeiro e ultimo
    // apontem para o unico elemento da lista.
    if (!lista->ultimo) {
        lista->primeiro = a;
        lista->ultimo = a;
        return;
    }
    
    if (is_primeiro) {
        a->next = lista->primeiro;
        lista->primeiro->prev = a;
        lista->primeiro = a;
        return;

    } else if (is_ultimo ) {
        lista->ultimo->next = a;
        a->prev = lista->ultimo;
        lista->ultimo = a;
        return;
    }

    // else if (pos != 'N') {
    //     fprintf(stderr, "pos invalida: %c, tamanho: %d\n", pos, lista->tamanho);
    //     exit(1);
    // }

    // suponhamos a seguinte lista:
    // a b c d e f g h i
    // suponhamos inserir um elemento novo (x) no indice 2, contando do 0:
    // a b c d e f g h i
    //     ^
    // resultado:
    // a b x c d e f g h i

    // pegar referencia do node na lista. seguindo o exemplo acima, esse seria o 'c'
    aluno* alvo = localizar_indice(lista, indice);

    // prévio de 'x' aponta para prévio de 'c': 'b'
    a->prev = alvo->prev;
    // proximo de 'x' aponta para 'c'
    a->next = alvo;
    // proximo de 'b', prévio de 'c', aponta para 'x' 
    alvo->prev->next = a;
    // prévio de 'c' aponta para 'x'
    alvo->prev = a;

    // voilà
}

void listar(lista_de *l) {
    printf("exibindo %d %s em lista:\n", l->tamanho, l->tamanho == 1 ? "item" : "itens");

    int i = 0;
    for (aluno* a = l->primeiro; a != NULL; a = a->next, i++) {
        printf("#%d nome: %s, idade: %d, tel: %s\n", i, a->nome, a->idade, a->telefone);
    }
    printf("\n");
}

void excluir(lista_de *l, int indice) {

    // se elemento sendo excluido é o ultimo.
    bool is_ultimo = (indice == l->tamanho-1);

    l->tamanho--;

    if (indice == 0 && is_ultimo) {
        // todo: 
        // free(l->primeiro);

        l->primeiro = NULL;
        l->ultimo = NULL;
        return;
    }
    // se elemento for o primeiro, alterar para quem aponta o 'primeiro' da lista.
    if (indice == 0) {
        aluno* primeiro = l->primeiro;

        // prévio do segundo agora é nulo
        l->primeiro->next->prev = NULL;
        // primeiro da lista aponta para o segundo
        l->primeiro = l->primeiro->next;

        // liberar primeiro
        // free(primeiro);
        return;

    } else if (is_ultimo) {
        // lista já guarda endereço do ultimo node
        aluno* ultimo = l->ultimo;

        // proximo do penultimo vira nulo
        l->ultimo->prev->next = NULL;
        // ultimo da lista aponta para o penultimo
        l->ultimo = l->ultimo->prev;

        // liberar ultimo
        // free(ultimo);
        return;

    } else if (indice < 0 || indice >= l->tamanho) {
        fprintf(stderr, "indice invalido: %d, tamanho: %d\n", indice, l->tamanho);
        exit(1);
    }

    aluno* alvo = localizar_indice(l, indice);

    alvo->prev->next = alvo->next;
    alvo->next->prev = alvo->prev;
    // todo: fazer free
}

void alterar(lista_de* l, aluno* novo, int indice) {
    // não é isso que ele quer mas funciona
    inserir(l, indice, novo);
    excluir(l, indice+1);
}

aluno* new_aluno(char* nome, char* telefone, int idade) {
    aluno* a = malloc(sizeof(aluno));
    a->nome = nome;
    a->idade = idade;

    return a;
}

aluno* prompt_novo_aluno(lista_de *lista) {
    char* nome = malloc(256 * sizeof(char));
    printf("nome: ");
    scanf("%s", nome);

    int idade;
    printf("idade: ");
    scanf("%d", &idade);

    char* telefone = malloc(256 * sizeof(char));
    printf("telefone: ");
    scanf("%s", telefone);

    aluno* a = malloc(sizeof(aluno));
    *a = (aluno) {
        .nome = nome, .idade = idade, .telefone = telefone,
            .prev = NULL, .next = NULL
    };

    return a;
}

int main(void) {

    // favor nunca jamais adicionar o mesmo aluno mais de uma vez na lista.
    aluno* teste = &(aluno) { 
        .nome = "daniel", .idade = 22, .telefone = "123123",
        .prev = NULL, .next = NULL,
    };
    aluno* teste2 = &(aluno) { 
        .nome = "breno", .idade = 20, .telefone = "123123",
        .prev = NULL, .next = NULL,
    };
    aluno* teste3 = &(aluno) { 
        .nome = "raphael", .idade = 21, .telefone = "123123",
        .prev = NULL, .next = NULL,
    };
    aluno* teste4 = &(aluno) { 
        .nome = "daniel", .idade = 21, .telefone = "123123",
        .prev = NULL, .next = NULL,
    };

    lista_de* lista = &(lista_de){ 0 };

    bool cont = true;
    do {
        int input;
        int indice;
        aluno* a = NULL;
        printf("(1) mostrar lista\n");
        printf("(2) inserir aluno\n");
        printf("(3) excluir aluno\n");
        printf("(4) alterar aluno\n");
        printf("(5) localizar por nome\n");
        scanf("%d", &input);

        switch (input) {
            case -1:
                cont = false;
                break;
            case 1:
                listar(lista);
                break;
            case 2:
                a = prompt_novo_aluno(lista);

                printf("\n");
                printf("(f)inal da lista\n");
                printf("(i)nicio da lista\n");
                printf("i(n)serir em indice\n");

                printf("selecione: ");

                char posicao;
                int indice;
                scanf(" %c", &posicao);

                if (posicao == 'i') 
                    indice = 0;

                else if (posicao == 'f') 
                    indice = lista->tamanho - 1;

                else if (posicao == 'n') {
                    printf("insira indice para adicionar aluno (0 - %d):", lista->tamanho-1);
                    scanf("%d", &indice);
                } 
                else {
                    fprintf(stderr, "posição invalida (%c)\n", posicao);
                    exit(1);
                }

                inserir(lista, indice, a);
                printf("aluno (nome: %s, idade: %d, tel: %s) inserido!\n", a->nome, a->idade, a->telefone);
                printf("\n");
                break;
            case 3:
                listar(lista);
                printf("indice: ");
                scanf("%d", &indice);
                aluno temp = *localizar_indice(lista, indice);
                excluir(lista, indice);
                printf("aluno (nome: %s, idade: %d, tel: %s) excluido!\n",
                        temp.nome, temp.idade, temp.telefone);
                printf("\n");
                break;
            case 4:
                listar(lista);
                printf("indice: ");
                scanf("%d", &indice);

                aluno* antigo = localizar_indice(lista, indice);
                a = prompt_novo_aluno(lista);

                alterar(lista, a, indice);

                printf("aluno (nome: %s, idade: %d, tel: %s) alterado para: aluno \
(nome: %s, idade: %d, tel: %s)!\n",
                        antigo->nome, antigo->idade, antigo->telefone,
                        a->nome, a->idade, a->telefone
                        );
                printf("\n");
                break;
            case 5:
                printf(""); // meu lsp (clangd) reclama sem essa linha ??????
                char nome[256];
                printf("nome: ");
                scanf("%s", nome);
                a = localizar(lista, nome);
                if (a == NULL) {
                    fprintf(stderr, "aluno \"%s\"não encontrado\n", nome);
                    continue;
                }
                printf("nome: %s, idade: %d, tel: %s\n", a->nome, a->idade, a->telefone);
                printf("\n");
                break;
            default:
                fprintf(stderr, "comando não reconhecido\n");
                break;
        }
    }
    while (cont);



    inserir(lista, -1, teste);
    inserir(lista, -1, teste2);
    inserir(lista, -1, teste3);
    inserir(lista, 0, teste4);

    listar(lista);

    excluir(lista, lista->tamanho-1);
    excluir(lista, 0);

    listar(lista);

    return 0;
}
