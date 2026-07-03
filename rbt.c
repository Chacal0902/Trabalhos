#include <stdio.h>
#include <stdlib.h>

// Cores: 0 pra vermelho, 1 pra preto
typedef struct No {
    int dado;
    int cor; 
    struct No *esq, *dir, *pai;
} No;

No* TNULL;

// Cria o no, o aluno esqueceu de comentar aqui kkk
No* criarNo(int dado) {
    No* novo = (No*)malloc(sizeof(No));
    novo->pai = NULL;
    novo->dado = dado;
    novo->esq = TNULL;
    novo->dir = TNULL;
    novo->cor = 0; // novo sempre nasce vermelho
    return novo;
}

void rEsq(No** raiz, No* x) {
    No* y = x->dir;
    x->dir = y->esq;
    if (y->esq != TNULL) y->esq->pai = x;
    y->pai = x->pai;
    if (x->pai == NULL) *raiz = y;
    else if (x == x->pai->esq) x->pai->esq = y;
    else x->pai->dir = y;
    y->esq = x;
    x->pai = y;
}

void rDir(No** raiz, No* y) {
    No* x = y->esq;
    y->esq = x->dir;
    if (x->dir != TNULL) x->dir->pai = y;
    x->pai = y->pai;
    if (y->pai == NULL) *raiz = x;
    else if (y == y->pai->dir) y->pai->dir = x;
    else y->pai->esq = x;
    x->dir = y;
    y->pai = x;
}

// Essa parte aqui eh chata, serve pra arrumar a arvore depois de inserir
void consertar(No** raiz, No* k) {
    No* u;
    while (k->pai != NULL && k->pai->cor == 0) {
        if (k->pai == k->pai->pai->dir) {
            u = k->pai->pai->esq;
            if (u->cor == 0) {
                u->cor = 1;
                k->pai->cor = 1;
                k->pai->pai->cor = 0;
                k = k->pai->pai;
            } else {
                if (k == k->pai->esq) {
                    k = k->pai;
                    rDir(raiz, k);
                }
                k->pai->cor = 1;
                k->pai->pai->cor = 0;
                rEsq(raiz, k->pai->pai);
            }
        } else {
            u = k->pai->pai->dir;
            if (u->cor == 0) {
                u->cor = 1;
                k->pai->cor = 1;
                k->pai->pai->cor = 0;
                k = k->pai->pai;
            } else {
                if (k == k->pai->dir) {
                    k = k->pai;
                    rEsq(raiz, k);
                }
                k->pai->cor = 1;
                k->pai->pai->cor = 0;
                rDir(raiz, k->pai->pai);
            }
        }
        if (k == *raiz) break;
    }
    (*raiz)->cor = 1;
}

void inserir(No** raiz, int dado) {
    No* novo = criarNo(dado);
    No* y = NULL;
    No* x = *raiz;

    while (x != TNULL) {
        y = x;
        if (novo->dado < x->dado) x = x->esq;
        else if (novo->dado > x->dado) x = x->dir;
        else {
            // Se ja existe, o aluno so ignora e libera a memoria pra nao dar leak
            free(novo);
            return;
        }
    }

    novo->pai = y;
    if (y == NULL) *raiz = novo;
    else if (novo->dado < y->dado) y->esq = novo;
    else y->dir = novo;

    if (novo->pai == NULL) {
        novo->cor = 1;
        return;
    }
    if (novo->pai->pai == NULL) return;

    consertar(raiz, novo);
}

// Printa em pre-ordem do jeito que o prof pediu
void preOrdem(No* raiz) {
    if (raiz != TNULL) {
        printf("%d%c ", raiz->dado, raiz->cor == 0 ? 'R' : 'N');
        preOrdem(raiz->esq);
        preOrdem(raiz->dir);
    }
}

int main() {
    TNULL = (No*)malloc(sizeof(No));
    TNULL->cor = 1;
    TNULL->esq = TNULL->dir = NULL;

    No* raiz = TNULL;
    char cmd;
    int val;

    // Loop infinito ate o EOF
    while (scanf(" %c", &cmd) != EOF) {
        if (cmd == 'i') {
            scanf("%d", &val);
            inserir(&raiz, val);
        } else if (cmd == 'p') {
            preOrdem(raiz);
            printf("\n");
        }
    }

    return 0;
}
