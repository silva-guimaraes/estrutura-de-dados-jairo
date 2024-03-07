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

aluno* localizar_indice(lista_de* l, int indice) {
    aluno* alvo = l->primeiro;
    for (int i = 0; alvo != NULL && i < indice; i++, alvo = alvo->next);
    assert(alvo);
    return alvo;
}

void localizar(lista_de* l, char* nome) {
    for (aluno* a = l->primeiro; a != NULL; a = a->next) {
        if (strcmp(a->nome, nome) == 0) {
            printf("nome: %s, idade: %d, tel: %s\n", a->nome, a->idade, a->telefone);
        }
    }
    printf("\n");
}

void inserir(lista_de* l, aluno* a, char pos, int indice) {
    bool is_ultimo = (indice == l->tamanho-1);
    bool is_primeiro = (indice == 0);
    l->tamanho++;

    if (!l->ultimo) {
        l->primeiro = a;
        l->ultimo = a;
        return;
    }
    
    if (pos == 'I' || is_primeiro) {
        a->next = l->primeiro;
        l->primeiro->prev = a;
        l->primeiro = a;
        return;

    } else if (pos == 'F' || is_ultimo ) {
        l->ultimo->next = a;
        a->prev = l->ultimo;
        l->ultimo = a;
        return;

    } else if (pos != 'N') {
        fprintf(stderr, "pos invalida: %c, tamanho: %d\n", pos, l->tamanho);
        exit(1);
    }

    aluno* alvo = localizar_indice(l, indice);

    a->prev = alvo->prev;
    a->next = alvo;
    alvo->prev->next = a;
    alvo->prev = a;
}

void listar(lista_de *l) {
    printf("exibindo %d %s em lista:\n", l->tamanho, l->tamanho == 1 ? "item" : "itens");

    for (aluno* a = l->primeiro; a != NULL; a = a->next) {
        printf("nome: %s, idade: %d, tel: %s\n", a->nome, a->idade, a->telefone);
    }
    printf("\n");
}

void excluir(lista_de *l, int indice) {

    bool is_ultimo = (indice == l->tamanho-1);

    l->tamanho--;

    if (indice == 0) {
        l->primeiro->next->prev = NULL;
        l->primeiro = l->primeiro->next;
        return;

    } else if (is_ultimo) {
        l->ultimo->prev->next = NULL;
        l->ultimo = l->ultimo->prev;
        return;

    } else if (indice < 0 || indice >= l->tamanho) {
        fprintf(stderr, "indice invalido: %d, tamanho: %d\n", indice, l->tamanho);
        exit(1);
    }

    aluno* alvo = localizar_indice(l, indice);

    alvo->prev->next = alvo->next;
    alvo->next->prev = alvo->prev;
}

void alterar(lista_de* l, aluno* novo, int indice) {
    // não é isso que ele quer mas funciona
    inserir(l, novo, 'N', indice);
    excluir(l, indice+1);
}

aluno* new_aluno(char* nome, char* telefone, int idade) {
    aluno* a = malloc(sizeof(aluno));
    a->nome = nome;
    a->idade = idade;

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

    inserir(lista, teste, 'F', -1);
    inserir(lista, teste2, 'F', -1);
    inserir(lista, teste3, 'F', -1);
    inserir(lista, teste4, 'N', 0);

    // inserir(lista, teste, 'F', -1);
    // inserir(lista, teste2, 'F', -1);
    // inserir(lista, teste3, 'F', -1);
    // inserir(lista, teste4, 'F', -1);

    // localizar(lista, "daniel");

    listar(lista);

    return 0;
}
