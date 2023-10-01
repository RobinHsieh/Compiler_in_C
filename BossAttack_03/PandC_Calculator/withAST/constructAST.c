#include "constructAST.h"

int factorial(int max) {
    int total = 1;
    if (max != 0) {
        int count;
        for (count = 1; count <= max; count++) {
            total *= count;
        }
    }
    return total;
}

int permutation(int m, int n) {
    return factorial(m) / factorial(m - n);
}

int combination(int m, int n) {
    return permutation(m, n) / factorial(n);
}

NodeAST* addNode(NodeType nodeType, int value, NodeAST* leftChild, NodeAST* rightChild) {
    NodeAST* newNode = malloc(sizeof(NodeAST));
    newNode->nodeType = nodeType;
    newNode->value = value;
    newNode->leftChild = leftChild;
    newNode->rightChild = rightChild;
    return newNode;
}

int interpretAST(NodeAST* nodeAst) {

    // return part for leaf node
    if (nodeAst->nodeType == NODE_VALUE) {
        return nodeAst->value;
    }

    else {

        // recursive part
        int left_value = interpretAST(nodeAst->leftChild);
        int right_value = interpretAST(nodeAst->rightChild);

        // return part for internal node
        switch (nodeAst->nodeType) {
            case NODE_ADDITION:
                return left_value + right_value;
            case NODE_SUBTRACTION:
                return left_value - right_value;
            case NODE_PERMUTATION:
                return permutation(left_value, right_value);
            case NODE_COMBINATION:
                return combination(left_value, right_value);
        }
    }
}