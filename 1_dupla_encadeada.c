
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

void inserir(lista_de* lista, aluno* a, char pos, int indice) {
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
    
    // I == inserir node no inicio da lista
    if (pos == 'I' || is_primeiro) {
        a->next = lista->primeiro;
        lista->primeiro->prev = a;
        lista->primeiro = a;
        return;

    // F == inserir node no final da lista
    } else if (pos == 'F' || is_ultimo ) {
        lista->ultimo->next = a;
        a->prev = lista->ultimo;
        lista->ultimo = a;
        return;

    // N == inserir node no indice
    // qualquer outro caractere é um erro por parte do usuario
    } else if (pos != 'N') {
        fprintf(stderr, "pos invalida: %c, tamanho: %d\n", pos, lista->tamanho);
        exit(1);
    }

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

    for (aluno* a = l->primeiro; a != NULL; a = a->next) {
        printf("nome: %s, idade: %d, tel: %s\n", a->nome, a->idade, a->telefone);
    }
    printf("\n");
}

void excluir(lista_de *l, int indice) {

    // se elemento sendo excluido é o ultimo.
    bool is_ultimo = (indice == l->tamanho-1);

    l->tamanho--;

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

    listar(lista);

    excluir(lista, lista->tamanho-1);
    excluir(lista, 0);

    listar(lista);

    return 0;
}
