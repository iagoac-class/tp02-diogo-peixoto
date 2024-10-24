#ifndef _H_ARVORES
#define _H_ARVORES

// Implementar qualquer struct que for necessária aqui
typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;

// Estrutura de um nó da árvore AVL
typedef struct Node_avl {
    int value;
    int height; // Altura do nó
    struct Node_avl *left;
    struct Node_avl *right;
} Node_avl;

//Funçoes da arvore desbalanceada
Node* createNode(int value);
Node* insert(Node *root, int value);
Node* findMin(Node *root);
Node* removeNode(Node *root, int value);

//Funçoes da arvore AVL
Node_avl* createNode_avl(int value);
int getHeight(Node_avl *node);
int getBalance(Node_avl *node);
void updateHeight(Node_avl *node);
Node_avl* rotateRight(Node_avl *y);
Node_avl* rotateLeft(Node_avl *x);
Node_avl* doubleRotateLeft(Node_avl *z);
Node_avl* doubleRotateRight(Node_avl *z);
Node_avl* insert_avl(Node_avl *node, int value);
Node_avl* findMin_avl(Node_avl *node); 
Node_avl* removeNode_avl(Node_avl *root, int value);

double arvore_binaria(int instancia_num, Node **root);
double arvore_balanceada(int instancia_num, Node_avl **root_avl);

#endif // _H_ARVORES
