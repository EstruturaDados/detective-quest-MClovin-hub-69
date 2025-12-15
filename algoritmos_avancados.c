#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// Estrutura da sala (árvore binária)
typedef struct Sala {
    char nome[50];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

// Estrutura da pista (BST)
typedef struct Pista {
    char descricao[50];
    struct Pista *esq;
    struct Pista *dir;
} Pista;

// Estrutura da tabela hash
typedef struct HashItem {
    char pista[50];
    char suspeito[50];
    struct HashItem *prox;
} HashItem;

HashItem *tabelaHash[TAM_HASH];

// Função hash simples
int funcaoHash(const char *str) {
    int soma = 0;
    for (int i = 0; str[i] != '\0'; i++)
        soma += str[i];
    return soma % TAM_HASH;
}

// criarSala – cria dinamicamente um cômodo
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esq = nova->dir = NULL;
    return nova;
}

// inserirPista – insere pista na BST
Pista* inserirPista(Pista *raiz, const char *descricao) {
    if (raiz == NULL) {
        Pista *nova = (Pista*) malloc(sizeof(Pista));
        strcpy(nova->descricao, descricao);
        nova->esq = nova->dir = NULL;
        return nova;
    }

    if (strcmp(descricao, raiz->descricao) < 0)
        raiz->esq = inserirPista(raiz->esq, descricao);
    else if (strcmp(descricao, raiz->descricao) > 0)
        raiz->dir = inserirPista(raiz->dir, descricao);

    return raiz;
}

// inserirNaHash – associa pista a suspeito
void inserirNaHash(const char *pista, const char *suspeito) {
    int idx = funcaoHash(pista);
    HashItem *novo = (HashItem*) malloc(sizeof(HashItem));

    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[idx];
    tabelaHash[idx] = novo;
}

// encontrarSuspeito – busca suspeito da pista
char* encontrarSuspeito(const char *pista) {
    int idx = funcaoHash(pista);
    HashItem *aux = tabelaHash[idx];

    while (aux) {
        if (strcmp(aux->pista, pista) == 0)
            return aux->suspeito;
        aux = aux->prox;
    }
    return NULL;
}

// explorarSalas – navegação pela mansão
void explorarSalas(Sala *atual, Pista **bstPistas) {
    char escolha;

    while (atual) {
        printf("\nVocê entrou em: %s\n", atual->nome);

        // Definição das pistas por sala
        if (strcmp(atual->nome, "Biblioteca") == 0) {
            printf("Pista encontrada: Livro rasgado\n");
            *bstPistas = inserirPista(*bstPistas, "Livro rasgado");
        }
        else if (strcmp(atual->nome, "Cozinha") == 0) {
            printf("Pista encontrada: Faca suja\n");
            *bstPistas = inserirPista(*bstPistas, "Faca suja");
        }
        else if (strcmp(atual->nome, "Jardim") == 0) {
            printf("Pista encontrada: Pegadas\n");
            *bstPistas = inserirPista(*bstPistas, "Pegadas");
        }

        printf("\n(e) Esquerda | (d) Direita | (s) Sair: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esq)
            atual = atual->esq;
        else if (escolha == 'd' && atual->dir)
            atual = atual->dir;
        else if (escolha == 's')
            return;
        else
            printf("Caminho inválido!\n");
    }
}

// listarPistas – percurso em ordem (BST)
void listarPistas(Pista *raiz) {
    if (raiz) {
        listarPistas(raiz->esq);
        printf("- %s (Suspeito: %s)\n",
               raiz->descricao,
               encontrarSuspeito(raiz->descricao));
        listarPistas(raiz->dir);
    }
}

// verificarSuspeitoFinal – julgamento
int contarPistasSuspeito(Pista *raiz, const char *suspeito) {
    if (!raiz) return 0;

    int cont = 0;
    char *assoc = encontrarSuspeito(raiz->descricao);

    if (assoc && strcmp(assoc, suspeito) == 0)
        cont = 1;

    return cont +
           contarPistasSuspeito(raiz->esq, suspeito) +
           contarPistasSuspeito(raiz->dir, suspeito);
}


int main() {
    // Inicializar hash
    for (int i = 0; i < TAM_HASH; i++)
        tabelaHash[i] = NULL;

    // Associações pista → suspeito
    inserirNaHash("Livro rasgado", "Mordomo");
    inserirNaHash("Faca suja", "Cozinheira");
    inserirNaHash("Pegadas", "Mordomo");

    // Montagem da mansão
    Sala *hall = criarSala("Hall");
    hall->esq = criarSala("Biblioteca");
    hall->dir = criarSala("Cozinha");
    hall->esq->dir = criarSala("Jardim");

    Pista *bstPistas = NULL;

    printf("=== DETECTIVE QUEST – CAPÍTULO FINAL ===\n");
    explorarSalas(hall, &bstPistas);

    printf("\nPistas coletadas:\n");
    listarPistas(bstPistas);

    char acusado[50];
    printf("\nQuem é o culpado? ");
    scanf(" %49[^\n]", acusado);

    int total = contarPistasSuspeito(bstPistas, acusado);

    if (total >= 2)
        printf("\n%s é culpado! Provas suficientes (%d pistas).\n", acusado, total);
    else
        printf("\nAcusação fraca! Apenas %d pista(s).\n", total);

    return 0;
}
