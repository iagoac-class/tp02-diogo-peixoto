#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>       
#include "arvores.h"

// Inicio das funçoes da arvore desbalanceda 

// Função para criar um novo nó
Node* createNode(int value) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Função para inserir um valor 
Node* insert(Node *root, int value) {
    if (root == NULL) {
        return createNode(value);
    }

    if (value < root->value) {
        root->left = insert(root->left, value);
    } else if (value > root->value) {
        root->right = insert(root->right, value);
    }

    return root;
}

// Função para encontrar o valor mínimo
Node* findMin(Node *root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

// Função para remover um valor 
Node* removeNode(Node *root, int value) {
    if (root == NULL) {
        return root;
    }

    // Procurar o nó a ser removido
    if (value < root->value) {
        root->left = removeNode(root->left, value);
    } else if (value > root->value) {
        root->right = removeNode(root->right, value);
    } else {
        // Caso 1: Nó não tem filho
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        // Caso 2: Nó tem apenas um filho
        else if (root->left == NULL) {
            Node *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node *temp = root->left;
            free(root);
            return temp;
        }
        // Caso 3: Nó tem dois filhos
        else {
            Node *temp = findMin(root->right); // Encontra o menor valor na subárvore direita
            root->value = temp->value; // Substitui o valor do nó pelo valor mínimo encontrado
            root->right = removeNode(root->right, temp->value); // Remove o nó que continha o menor valor
        }
    }

    return root;
}


double arvore_binaria(int instancia_num, Node **root) {
    double tempo = 0;
    clock_t begin = clock();

    char filename[50]; // para armazenar o nome do arquivo

    // Formatar o nome do arquivo
    sprintf(filename, "instancias/%d", instancia_num);

    // Abrir o arquivo para leitura
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    char operation;
    int value;
    while (fscanf(file, " %c %d", &operation, &value) != EOF) {
        if (operation == 'I') {
            *root = insert(*root, value);
        } else if (operation == 'R') {
            *root = removeNode(*root, value);
        }
    }

    fclose(file);
    
    clock_t end = clock();
    // calcula o tempo decorrido encontrando a diferença (end - begin) e
    // dividindo a diferença por CLOCKS_PER_SEC para converter em segundos
    tempo += (double)(end - begin) / CLOCKS_PER_SEC;
    return (tempo);
}
// Fim das funçoes da arvore desbalanceada

//Inicio das funçoes da arvore AVL

// Função para criar um novo nó
Node_avl* createNode_avl(int value) {
    Node_avl *newNode = (Node_avl*)malloc(sizeof(Node_avl));
    newNode->value = value;
    newNode->height = 1; // Inicialmente, a altura é 1
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Função para obter a altura de um nó
int getHeight(Node_avl *node) {
    return node ? node->height : 0;
}

// Função para obter o fator de balanceamento
int getBalance(Node_avl *node) {
    if (node == NULL) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

// Função para atualizar a altura de um nó
void updateHeight(Node_avl *node) {
    if (node) {
        node->height = 1 + (getHeight(node->left) > getHeight(node->right) ? 
                            getHeight(node->left) : getHeight(node->right));
    }
}

// Rotação simples à direita
Node_avl* rotateRight(Node_avl *y) {
    Node_avl *x = y->left;
    Node_avl *T2 = x->right;

    // Realiza a rotação
    x->right = y;
    y->left = T2;

    // Atualiza alturas
    updateHeight(y);
    updateHeight(x);

    return x; // Novo raiz
}

// Rotação simples à esquerda
Node_avl* rotateLeft(Node_avl *x) {
    Node_avl *y = x->right;
    Node_avl *T2 = y->left;

    // Realiza a rotação
    y->left = x;
    x->right = T2;

    // Atualiza alturas
    updateHeight(x);
    updateHeight(y);

    return y; // Novo raiz
}

// Rotação dupla à esquerda
Node_avl* doubleRotateLeft(Node_avl *z) {
    z->right = rotateRight(z->right);
    return rotateLeft(z);
}

// Rotação dupla à direita
Node_avl* doubleRotateRight(Node_avl *z) {
    z->left = rotateLeft(z->left);
    return rotateRight(z);
}

// Função para inserir um valor na árvore AVL
Node_avl* insert_avl(Node_avl *node, int value) {
    if (node == NULL) {
        return createNode_avl(value);
    }

    if (value < node->value) {
        node->left = insert_avl(node->left, value);
    } else if (value > node->value) {
        node->right = insert_avl(node->right, value);
    } else {
        return node;
    }

    // Atualiza a altura do nó pai
    updateHeight(node);

    // Verifica o fator de balanceamento e aplica as rotações necessárias
    int balance = getBalance(node);

    // Caso 1: Rotação a direita
    if (balance > 1 && value < node->left->value) {
        return rotateRight(node);
    }

    // Caso 2: Rotação a esquerda
    if (balance < -1 && value > node->right->value) {
        return rotateLeft(node);
    }

    // Caso 3: Rotação dupla a direita
    if (balance > 1 && value > node->left->value) {
        return doubleRotateRight(node);
    }

    // Caso 4: Rotação dupla a esquerda
    if (balance < -1 && value < node->right->value) {
        return doubleRotateLeft(node);
    }

    return node; // Retorna o nó
}

// Função para encontrar o nó com o valor mínimo
Node_avl* findMin_avl(Node_avl *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

// Função para remover um valor da árvore AVL
Node_avl* removeNode_avl(Node_avl *root, int value) {
    if (root == NULL) {
        return root;
    }

    // Procura pelo nó a ser removido
    if (value < root->value) {
        root->left = removeNode_avl(root->left, value);
    } else if (value > root->value) {
        root->right = removeNode_avl(root->right, value);
    } else {
        // Caso 1: Nó nao tem filho
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        // Caso 2: Nó tem apenas um filho
        else if (root->left == NULL) {
            Node_avl *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node_avl *temp = root->left;
            free(root);
            return temp;
        }
        // Caso 3: Nó tem dois filhos
        else {
            Node_avl *temp = findMin_avl(root->right); // Encontra o menor valor na subárvore direita
            root->value = temp->value; // Substitui o valor do nó pelo valor mínimo encontrado
            root->right = removeNode_avl(root->right, temp->value); // Remove o nó que continha o menor valor
        }
    }

    // Atualiza a altura do nó pai
    updateHeight(root);

    // Verifica o fator de balanceamento e aplica as rotações necessárias
    int balance = getBalance(root);

    // Caso 1: Rotação a direita
    if (balance > 1 && getBalance(root->left) >= 0) {
        return rotateRight(root);
    }

    // Caso 2: Rotação à esquerda
    if (balance < -1 && getBalance(root->right) <= 0) {
        return rotateLeft(root);
    }

    // Caso 3: Rotação dupla à direita
    if (balance > 1 && getBalance(root->left) < 0) {
        return doubleRotateRight(root);
    }

    // Caso 4: Rotação dupla à esquerda
    if (balance < -1 && getBalance(root->right) > 0) {
        return doubleRotateLeft(root);
    }

    return root; // Retorna o nó
}

double arvore_balanceada(int instancia_num, Node_avl **root_avl) {
    double tempo = 0;
    clock_t begin = clock();

    char filename[50]; // para armazenar o nome do arquivo

    // Formatar o nome do arquivo
    sprintf(filename, "instancias/%d", instancia_num);

    // Abrir o arquivo para leitura
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    char operation;
    int value;
    while (fscanf(file, " %c %d", &operation, &value) != EOF) {
        if (operation == 'I') {
            *root_avl = insert_avl(*root_avl, value);
        } else if (operation == 'R') {
            *root_avl = removeNode_avl(*root_avl, value);
        }
    }

    fclose(file);

    clock_t end = clock();
    // calcula o tempo decorrido encontrando a diferença (end - begin) e
    // dividindo a diferença por CLOCKS_PER_SEC para converter em segundos
    tempo += (double)(end - begin) / CLOCKS_PER_SEC;
    return (tempo);
}

//Fim das funçoes da arvore AVL

int main(int argc, char* argv[]) {
    ///////////////////////////////////////////////////////////
    /////////////////// Leitor de instâncias //////////////////
    ///////////////// Não deve ser modificado /////////////////
    ///////////////////////////////////////////////////////////
    int instancia_num = -1;
    instancia_num = atoi(argv[1]);
    if (instancia_num <= 0 || instancia_num > 10) {
        printf("Para executar o código, digite ./arvores x\nonde x é um número entre 1 e 10 que simboliza a instância utilizada\n");
        exit(0);
    }
    Node *root = NULL;
    Node_avl *root_avl = NULL;

    double tempo_n_balanceada = arvore_binaria(instancia_num, &root);
    double tempo_balanceada = arvore_balanceada(instancia_num, &root_avl);

    

    
    printf("%f\n%f\n", tempo_n_balanceada, tempo_balanceada);

    return (1);
}
