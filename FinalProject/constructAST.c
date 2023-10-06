#include "constructAST.h"


/* testData segment */
GlobalSymbolTable* globalLinkedList;
AddressOfParameterTable addressOfParameterTable[2048];
int topOfAddressOfParameterTable = -1;

char* activeFunctionNameStack[2048];
int topOfActiveFunctionNameStack = 0;

int anonymousFunctionOrder = 1;

int passedArgumentStack[2048];
int stackPtrOfArgument = -1;
int basePtrOfArgument = -1;


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

        switch (nodeAst->nodeType) {

            /* THIS PART IS PRINT STATEMENT */
            /* going to print the integer */
            case NODE_PRINT_NUM:
            {
                NodeAST* printedNode;
                printedNode = evaluateExpression(nodeAst->leftChild);

                printf("%d\n", printedNode->integer);
                break;
            }
            /* going to print the boolean */
            case NODE_PRINT_BOOL:
            {
                NodeAST* printedNode;
                printedNode = evaluateExpression(nodeAst->leftChild);

                if (printedNode->integer == 0) {
                    printf("%s\n", "#f");
                }
                else if (printedNode->integer == -1) {
                    printf("%s\n", "#t");
                }
                break;
            }

            /* THIS PART IS DEFINE STATEMENT */
            case NODE_DEFINE:
            {
                /* going to define a variable */
                if (nodeAst->rightChild->nodeType != NODE_FUNCTION) {
                    /* bind the variable name `nodeAst->leftChild->string` with the expression `nodeAst->rightChild` */
                    pushIntoGlobalSymbolTable(nodeAst->leftChild->string, nodeAst->rightChild, &globalLinkedList);
                }

                /* going to define a function */
                else {
                    /* bind the function name `nodeAst->leftChild->string` with the expression `nodeAst->rightChild->rightChild` */
                    pushIntoGlobalSymbolTable(nodeAst->leftChild->string, nodeAst->rightChild->rightChild, &globalLinkedList);

                    /* push the parameters into AddressOfParameterTable */
                    traversalPARAMETER(nodeAst->rightChild->leftChild, nodeAst->leftChild->string, 0);
                }
                break;
            }
            default:
                /* shall never pass through here */
                break;
        }
    }
}

NodeAST* evaluateExpression(NodeAST* nodeAst) {


    /* return part: leaf node */
    if (nodeAst->nodeType == NODE_VARIABLE) {

        NodeAST* newLeafNodeAst = malloc(sizeof(NodeAST));

        /* deal with variable */
        if (strcmp(activeFunctionNameStack[topOfActiveFunctionNameStack], "-none") == 0) {
            newLeafNodeAst = evaluateExpression(findGlobalSymbolTableNode(nodeAst->leftChild->string));
        }

        else {
            int address = findAddressOfParameterTable(activeFunctionNameStack[topOfActiveFunctionNameStack], nodeAst->leftChild->string);
            if (address != 0) {
                newLeafNodeAst->nodeType = NODE_INTEGER; // It can also be NODE_BOOLEAN, later we will deal with it.
                newLeafNodeAst->integer = passedArgumentStack[basePtrOfArgument - address];
            }
            else {
                newLeafNodeAst = evaluateExpression(findGlobalSymbolTableNode(nodeAst->leftChild->string));
            }
        }
        return newLeafNodeAst;
    }

    else if (nodeAst->nodeType == NODE_INTEGER || nodeAst->nodeType == NODE_BOOLEAN) {
        NodeAST* newLeafNodeAst = malloc(sizeof(NodeAST));
        newLeafNodeAst->nodeType = nodeAst->nodeType;
        newLeafNodeAst->integer = nodeAst->integer;
        return newLeafNodeAst;
    }


    /* recursive part: calculate from top to bottom */
    else if (nodeAst->nodeType == NODE_IF_EXPRESSION) {
        return judgeIF_EXPRESSION((Node_If_AST*)nodeAst);
    }

    else if (nodeAst->nodeType == NODE_FUNCTION_CALL) {

        /* handle the function call */
        return handleFUNCTION_CALL( nodeAst);
    }

    else {
        /* recursive part: calculate from bottom to top */
        NodeAST* left_child = evaluateExpression(nodeAst->leftChild);
        NodeAST* right_child;

        /* prevent NULL when visit NODE_NOT node */
        if (nodeAst->rightChild != NULL) {
            right_child = evaluateExpression(nodeAst->rightChild);
        }

        NodeAST* newNodeAst = malloc(sizeof(NodeAST));

        /* return part: internal node */
        switch (nodeAst->nodeType) {
            case NODE_GREATER:
                newNodeAst->nodeType = NODE_BOOLEAN;
                newNodeAst->integer = -(left_child->integer > right_child->integer);
                return newNodeAst;
            case NODE_SMALLER:
                newNodeAst->nodeType = NODE_BOOLEAN;
                newNodeAst->integer = -(left_child->integer < right_child->integer);
                return newNodeAst;
            case NODE_AND:
                newNodeAst->nodeType = NODE_BOOLEAN;
                newNodeAst->integer = left_child->integer & right_child->integer;
                return newNodeAst;
            case NODE_OR:
                newNodeAst->nodeType = NODE_BOOLEAN;
                newNodeAst->integer = left_child->integer | right_child->integer;
                return newNodeAst;
            case NODE_NOT:
                newNodeAst->nodeType = NODE_BOOLEAN;
                newNodeAst->integer = ~ left_child->integer;
                return newNodeAst;
            case NODE_EQUAL:
                newNodeAst->nodeType = NODE_BOOLEAN;

                /* nodeAst->leftChild is also the "expression: (exp exp)" with nodeAst in "equal: ( = (exp exp) exp ......)" */
                if (left_child->nodeType == NODE_BOOLEAN) {
                    if (left_child->integer == 0) {
                        newNodeAst->integer = 0;
                    }
                    else if (left_child->integer == -1) {
                        newNodeAst->integer = -(left_child->rightChild->integer == right_child->integer);
                        newNodeAst->rightChild = right_child;
                    }
                }
                /* nodeAst->leftChild is a number */
                else {
                    newNodeAst->integer = -(left_child->integer == right_child->integer);
                    newNodeAst->rightChild = right_child;
                }
                return newNodeAst;
            case NODE_ADDITION:
                newNodeAst->nodeType = NODE_INTEGER;
                newNodeAst->integer = left_child->integer + right_child->integer;
                return newNodeAst;
            case NODE_SUBTRACTION:
                newNodeAst->nodeType = NODE_INTEGER;
                newNodeAst->integer = left_child->integer - right_child->integer;
                return newNodeAst;
            case NODE_MULTIPLICATION:
                newNodeAst->nodeType = NODE_INTEGER;
                newNodeAst->integer = left_child->integer * right_child->integer;
                return newNodeAst;
            case NODE_DIVISION:
                newNodeAst->nodeType = NODE_INTEGER;
                newNodeAst->integer = left_child->integer / right_child->integer;
                return newNodeAst;
            case NODE_MODULUS:
                newNodeAst->nodeType = NODE_INTEGER;
                newNodeAst->integer = left_child->integer % right_child->integer;
                return newNodeAst;
            default:
                /* shall never pass through here */
                return newNodeAst;
        }
    }
}

NodeAST* judgeIF_EXPRESSION(Node_If_AST* node_If_Ast) {

    int judgement;

    judgement = evaluateExpression(node_If_Ast->testChild)->integer;

    if (judgement == -1) {
        return evaluateExpression(node_If_Ast->thenChild);
    }

    /* (judgement == 0) */
    else {
        return evaluateExpression(node_If_Ast->elseChild);
    }
}

NodeAST* handleFUNCTION_CALL(NodeAST* nodeAst) {

    NodeAST* functionReturnNodeAst;
    int numberOfArguments = 0;

    /* anonymous function */
    if (nodeAst->leftChild->nodeType == NODE_FUNCTION) {

        /* create a new function name `-anonymous-X` */
        char* functionName = malloc(20);
        sprintf(functionName, "-anonymous-%d", anonymousFunctionOrder);
        anonymousFunctionOrder++;
        
        /* update the current fuction scope */
        topOfActiveFunctionNameStack++;
        activeFunctionNameStack[topOfActiveFunctionNameStack] = functionName;

        /* bind the function name `-anonymous-X` with the expression `nodeAst->leftChild->rightChild` */
        pushIntoGlobalSymbolTable(functionName, nodeAst->leftChild->rightChild, &globalLinkedList);

        /* push parameters into AddressOfParameterTable */
        traversalPARAMETER(nodeAst->leftChild->leftChild, activeFunctionNameStack[topOfActiveFunctionNameStack], 0);
    }

    /* named function */
    else {

        /* update the current fuction scope */
        topOfActiveFunctionNameStack++;
        activeFunctionNameStack[topOfActiveFunctionNameStack] = nodeAst->leftChild->string;
    }

    /* traversal arguments tree and push arguments into the passed argument stack */
    traversalARGUMENT(nodeAst->rightChild, &numberOfArguments);

    /* push basePtrOfArgument into the passed argument stack */
    stackPtrOfArgument++;
    passedArgumentStack[stackPtrOfArgument] = basePtrOfArgument;
    basePtrOfArgument = stackPtrOfArgument;

    /* call the function */
    functionReturnNodeAst = evaluateExpression(findGlobalSymbolTableNode(activeFunctionNameStack[topOfActiveFunctionNameStack]));

    /* pop basePtrOfArgument from the passed argument stack */
    basePtrOfArgument = passedArgumentStack[stackPtrOfArgument];
    stackPtrOfArgument--;

    /* pop arguments from the passed argument stack */
    stackPtrOfArgument -= numberOfArguments;

    /* return the fuction scope */
    topOfActiveFunctionNameStack--;

    return functionReturnNodeAst;
}

void traversalPARAMETER(NodeAST* nodeAst, char* functionName, int offsetAddress) {

    /* visit part */
    if (!nodeAst) {
        return;
    }
    else if (nodeAst->nodeType == NODE_STRING) {
        pushIntoAddressOfParameterTable(functionName, nodeAst->string, offsetAddress);
    }

    /* recursive part */
    if (nodeAst->leftChild) {
        traversalPARAMETER(nodeAst->leftChild, functionName, offsetAddress + 1);
    }
    if (nodeAst->rightChild) {
        traversalPARAMETER(nodeAst->rightChild, functionName, offsetAddress + 1);
    }
}

void traversalARGUMENT(NodeAST* nodeAst, int* numberOfArguments) {

    /* prevent NULL when visit NODE_ARGUMENT node */
    if (!nodeAst) {
        return;
    }
    else if (nodeAst->nodeType == NODE_ARGUMENT) {
        /* count the number of arguments */
        *numberOfArguments = *numberOfArguments + 1;

        /* push arguments into passed argument stack */
        traversalARGUMENT(nodeAst->rightChild, numberOfArguments);
        stackPtrOfArgument++;
        passedArgumentStack[stackPtrOfArgument] = evaluateExpression(nodeAst->leftChild)->integer;
    }
}

void pushIntoAddressOfParameterTable(char* functionName, char* identifier, int offsetAddress) {
    /* push the parameters into AddressOfParameterTable */
    topOfAddressOfParameterTable++;
    addressOfParameterTable[topOfAddressOfParameterTable].functionName = functionName;
    addressOfParameterTable[topOfAddressOfParameterTable].identifier = identifier;
    addressOfParameterTable[topOfAddressOfParameterTable].offsetAddress = offsetAddress;
}

void pushIntoGlobalSymbolTable(char* identifier, NodeAST* expression, GlobalSymbolTable** head) {

    GlobalSymbolTable* directTable = *head;
    GlobalSymbolTable* previousTable = NULL;

    /* create a new directTable */
    GlobalSymbolTable* newTable = malloc(sizeof(GlobalSymbolTable));
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

NodeAST* findGlobalSymbolTableNode(char* identifier) {

    GlobalSymbolTable* table = globalLinkedList;

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

int findAddressOfParameterTable(char* functionName, char* identifier) {
    for (int i = 0; i <= topOfAddressOfParameterTable; i++) {
        if (strcmp(addressOfParameterTable[i].functionName, functionName) == 0 && strcmp(addressOfParameterTable[i].identifier, identifier) == 0) {
            return addressOfParameterTable[i].offsetAddress;
        }
    }

    /* if didn't find the address */
    return 0;
}


#if defined(DEBUG__PRINT_TREE_STRUCTURE)
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
