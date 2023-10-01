#include "constructAST.h"


/* data segment */
SymbolTable* globalLinkedList;
AddressOfParameter addressOfParameterArray[256];
int top = -1;

int passedArgumentArray[256];
int esp = -1;
int ebp = -1;


NodeAST* addNode(NodeType nodeType, int integer, char* string, NodeAST* leftChild, NodeAST* rightChild) {
    NodeAST* newNode = malloc(sizeof(NodeAST));
    newNode->nodeType = nodeType;
    newNode->integer = integer;
    newNode->string = string;
    newNode->leftChild = leftChild;
    newNode->rightChild = rightChild;
    return newNode;
}

NodeAST* add_If_Node(NodeType nodeType, NodeAST* testChild, NodeAST* thenChild, NodeAST* elseChild) {
    Node_If_AST* newNode = malloc(sizeof(Node_If_AST));
    newNode->nodeType = nodeType;
    newNode->testChild = testChild;
    newNode->thenChild = thenChild;
    newNode->elseChild = elseChild;
    return (NodeAST*)newNode;
}

void traversalSTATMENT(NodeAST* nodeAst) {

    /* return part */
    if (nodeAst == NULL) {
        return;
    }

    /* recursive part */
    else if (nodeAst->nodeType == NODE_STATEMENT) {
        traversalSTATMENT(nodeAst->leftChild);
        traversalSTATMENT(nodeAst->rightChild);
    }

    /* visit part */
    else {

        /* going to print the integer */
        if (nodeAst->nodeType == NODE_PRINT_NUM) {

            NodeAST* printedNode;

            /* expression belongs to if-exp */
            if (nodeAst->leftChild->nodeType == NODE_IF_EXPRESSION) {

                /* convert NodeAST to Node_If_AST */
                Node_If_AST* node_If_Ast = (Node_If_AST*)nodeAst->leftChild;
                printedNode = evaluateNumerical_And_LogicalExpression(judgeIfExpression(node_If_Ast));
            }

                /* expression belongs to num-op/logical-op/number/bool-val */
            else {
                printedNode = evaluateNumerical_And_LogicalExpression(nodeAst->leftChild);
            }
            printf("%d\n", printedNode->integer);
        }

        /* going to print the boolean */
        else if (nodeAst->nodeType == NODE_PRINT_BOOL) {

            NodeAST* printedNode;

            /* expression belongs to if-exp */
            if (nodeAst->leftChild->nodeType == NODE_IF_EXPRESSION) {

                /* convert NodeAST to Node_If_AST */
                Node_If_AST* node_If_Ast = (Node_If_AST*)nodeAst->leftChild;
                printedNode = evaluateNumerical_And_LogicalExpression(judgeIfExpression(node_If_Ast));
            }

            /* expression belongs to num-op/logical-op/number/bool-val */
            else {
                printedNode = evaluateNumerical_And_LogicalExpression(nodeAst->leftChild);
            }
            if (printedNode->integer == 0) {
                printf("%s\n", "#f");
            }
            else if (printedNode->integer == -1) {
                printf("%s\n", "#t");
            }
        }

        /* going to define a variable */
        else if (nodeAst->nodeType == NODE_DEFINE) {

            // nodeAst->leftChild->->nodeType == NODE_STRING
            push(nodeAst->leftChild->string, nodeAst->rightChild, &globalLinkedList);
        }
    }
}

NodeAST* judgeIfExpression(Node_If_AST* node_If_Ast) {

    int judgement;

    /* expression belongs to if-exp */
    if (node_If_Ast->testChild->nodeType == NODE_IF_EXPRESSION) {

        /* convert NodeAST to Node_if_AST */
        Node_If_AST* node_if_Ast = (Node_If_AST*)node_If_Ast->testChild;
        judgement = evaluateNumerical_And_LogicalExpression(judgeIfExpression(node_if_Ast))->integer;
    }

    /* expression belongs to num-op/logical-op/number/bool-val */
    else {
        judgement = evaluateNumerical_And_LogicalExpression(node_If_Ast->testChild)->integer;
    }


    if (judgement == -1) {

        /* expression belongs to if-exp */
        if (node_If_Ast->thenChild->nodeType == NODE_IF_EXPRESSION) {

            /* convert NodeAST to Node_if_AST */
            Node_If_AST* node_if_Ast = (Node_If_AST*)node_If_Ast->thenChild;
            return judgeIfExpression(node_if_Ast);
        }

        /* expression belongs to num-op/logical-op/number/bool-val */
        else {
            return node_If_Ast->thenChild;
        }
    }


    else if (judgement == 0) {

        /* expression belongs to if-exp */
        if (node_If_Ast->elseChild->nodeType == NODE_IF_EXPRESSION) {

            /* convert NodeAST to Node_if_AST */
            Node_If_AST* node_if_Ast = (Node_If_AST*)node_If_Ast->elseChild;
            return judgeIfExpression(node_if_Ast);
        }

        /* expression belongs to num-op/logical-op/number/bool-val */
        else {
            return node_If_Ast->elseChild;
        }
    }


    else {
        /* shall never pass through here */
        return (NodeAST*)node_If_Ast;
    }
}

NodeAST* evaluateNumerical_And_LogicalExpression(NodeAST* nodeAst) {

    /* deal with variable */
    if (nodeAst->nodeType == NODE_VARIABLE) {
        nodeAst = findNode(nodeAst->leftChild->string);
    }

    /* return part for leaf node */
    if (nodeAst->nodeType == NODE_INTEGER || nodeAst->nodeType == NODE_BOOLEAN) {
        return nodeAst;
    }

    else {
        /* recursive part */
        NodeAST* left_child = evaluateNumerical_And_LogicalExpression(nodeAst->leftChild);
        NodeAST* right_child;
        // prevent NULL when visit NODE_NOT node
        if (nodeAst->rightChild != NULL) {
            right_child = evaluateNumerical_And_LogicalExpression(nodeAst->rightChild);
        }

        /* return part for internal node */
        switch (nodeAst->nodeType) {
            case NODE_GREATER:
                nodeAst->nodeType = NODE_BOOLEAN;
                nodeAst->integer = -(left_child->integer > right_child->integer);
                return nodeAst;
            case NODE_SMALLER:
                nodeAst->nodeType = NODE_BOOLEAN;
                nodeAst->integer = -(left_child->integer < right_child->integer);
                return nodeAst;
            case NODE_AND:
                nodeAst->nodeType = NODE_BOOLEAN;
                nodeAst->integer = left_child->integer & right_child->integer;
                return nodeAst;
            case NODE_OR:
                nodeAst->nodeType = NODE_BOOLEAN;
                nodeAst->integer = left_child->integer | right_child->integer;
                return nodeAst;
            case NODE_NOT:
                nodeAst->nodeType = NODE_BOOLEAN;
                nodeAst->integer = ~ left_child->integer;
                return nodeAst;
            case NODE_EQUAL:
                nodeAst->nodeType = NODE_BOOLEAN;
                /* nodeAst->leftChild is also the "expression: (exp exp)" with nodeAst in "equal: ( = (exp exp) exp ......)" */
                if (left_child->nodeType == NODE_BOOLEAN) {
                    if (left_child->integer == 0) {
                        nodeAst->integer = 0;
                    }
                    else if (left_child->integer == -1) {
                        nodeAst->integer = -(left_child->rightChild->integer == right_child->integer);
                    }
                }
                else {
                    nodeAst->integer = -(left_child->integer == right_child->integer);
                }
                return nodeAst;
            case NODE_ADDITION:
                nodeAst->nodeType = NODE_INTEGER;
                nodeAst->integer = left_child->integer + right_child->integer;
                return nodeAst;
            case NODE_SUBTRACTION:
                nodeAst->nodeType = NODE_INTEGER;
                nodeAst->integer = left_child->integer - right_child->integer;
                return nodeAst;
            case NODE_MULTIPLICATION:
                nodeAst->nodeType = NODE_INTEGER;
                nodeAst->integer = left_child->integer * right_child->integer;
                return nodeAst;
            case NODE_DIVISION:
                nodeAst->nodeType = NODE_INTEGER;
                nodeAst->integer = left_child->integer / right_child->integer;
                return nodeAst;
            case NODE_MODULUS:
                nodeAst->nodeType = NODE_INTEGER;
                nodeAst->integer = left_child->integer % right_child->integer;
                return nodeAst;
            default:
                /* shall never pass through here */
                return nodeAst;
        }
    }
}

void push(char* identifier, NodeAST* expression, SymbolTable** head) {

    SymbolTable* directTable = *head;
    SymbolTable* previousTable = NULL;

    /* create a new directTable */
    SymbolTable* newTable = malloc(sizeof(SymbolTable));
    newTable->identifier = identifier;
    newTable->expression = expression;
    newTable->nextPtr = NULL;

    /* iteration */
    while (directTable != NULL) {
        previousTable = directTable;
        directTable = directTable->nextPtr;
    }


    if (previousTable != NULL) {
        previousTable->nextPtr = newTable;
    }
    else {
        *head = newTable;
    }

}


NodeAST* findNode(char* identifier) {

    SymbolTable* table = globalLinkedList;

    /* iteration */
    while (table != NULL) {
        if (strcmp(table->identifier, identifier) == 0) {
            return table->expression;
        }
        table = table->nextPtr;
    }

    /* shall never pass through here */
    return table->expression;
}


#if defined(DEBUG)
void traversalAST_preorder(NodeAST* root) {
    printf("%d\n", root->nodeType);
    if (root->leftChild) {
        traversalAST_preorder(root->leftChild);
    }
    if (root->rightChild) {
        traversalAST_preorder(root->rightChild);
    }
}
void traversalAST_inorder(NodeAST* root) {
    if (root->leftChild) {
        traversalAST_inorder(root->leftChild);
    }
    printf("%d\n", root->nodeType);
    if (root->rightChild) {
        traversalAST_inorder(root->rightChild);
    }
}
void traversalAST_postorder(NodeAST* root) {
    if (root->leftChild) {
        traversalAST_postorder(root->leftChild);
    }
    if (root->rightChild) {
        traversalAST_postorder(root->rightChild);
    }
    printf("%d\n", root->nodeType);
}
#endif
