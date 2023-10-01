#ifndef BOSS_ATTACK_3_CONSTRUCTAST_H
#define BOSS_ATTACK_3_CONSTRUCTAST_H

#include <stdlib.h>

typedef enum nodeType NodeType;
typedef struct nodeAST NodeAST;

enum nodeType {
    NODE_VALUE,
    NODE_ADDITION,
    NODE_SUBTRACTION,
    NODE_PERMUTATION,
    NODE_COMBINATION
};

struct nodeAST {
    NodeType nodeType;
    int value;
    struct nodeAST* leftChild;
    struct nodeAST* rightChild;
};

int factorial(int count);
int permutation(int m, int n);
int combination(int m, int n);
NodeAST* addNode(NodeType nodeType, int value, NodeAST* leftChild, NodeAST* rightChild);
int interpretAST(NodeAST* nodeAst);


#endif //BOSS_ATTACK_3_CONSTRUCTAST_H
