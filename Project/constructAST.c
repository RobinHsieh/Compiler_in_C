#include "constructAST.h"


/* testData segment */
GlobalSymbolTable* globalLinkedList;
AddressOfParameterTable addressOfParameterTable[2048];
int topOfAddressOfParameterTable = -1;

char* currentFunctionNameBeingVisited;
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
                    /* nodeAst->leftChild->nodeType == NODE_STRING */
                    /* bind the variable name `nodeAst->leftChild->string` with the expression `nodeAst->rightChild` */
                    pushIntoGlobalSymbolTable(nodeAst->leftChild->string, nodeAst->rightChild, &globalLinkedList);
                }

                /* going to define a function */
                else {
                    /* bind the function name `nodeAst->leftChild->string` with the expression `nodeAst->rightChild->rightChild` */
//                    puts("hi0!, push function name"); // debug
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

        NodeAST* newNodeAst = malloc(sizeof(NodeAST));

        /* deal with variable */
        if (strcmp(currentFunctionNameBeingVisited, "-none") == 0) {
            newNodeAst = evaluateExpression(findGlobalSymbolTableNode(nodeAst->leftChild->string));
        }

        else {
            int address = findAddressOfParameterTable(currentFunctionNameBeingVisited, nodeAst->leftChild->string);
            if (address != 0) {
                newNodeAst->nodeType = NODE_INTEGER; // It can also be NODE_BOOLEAN, later we will deal with it.
//                printf("hi4!, stack frame       ,  basePtrOfArgument: %d, address of variable: %d\n", basePtrOfArgument, address); // debug
                newNodeAst->integer = passedArgumentStack[basePtrOfArgument - address];
//                printf("hi5!, find result       ,  variable = %d\n", newNodeAst->integer); // debug
            }
            else {
                newNodeAst = evaluateExpression(findGlobalSymbolTableNode(nodeAst->leftChild->string));
            }
        }
        return newNodeAst;
    }

    else if (nodeAst->nodeType == NODE_INTEGER || nodeAst->nodeType == NODE_BOOLEAN) {
        return nodeAst;
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

        // prevent NULL when visit NODE_NOT node
        if (nodeAst->rightChild != NULL) {
            right_child = evaluateExpression(nodeAst->rightChild);
        }

        NodeAST* leafNodeAst = malloc(sizeof(NodeAST));

        /* return part: internal node */
        switch (nodeAst->nodeType) {
            case NODE_GREATER:
                leafNodeAst->nodeType = NODE_BOOLEAN;
                leafNodeAst->integer = -(left_child->integer > right_child->integer);
                return leafNodeAst;
            case NODE_SMALLER:
                leafNodeAst->nodeType = NODE_BOOLEAN;
                leafNodeAst->integer = -(left_child->integer < right_child->integer);
                return leafNodeAst;
            case NODE_AND:
                leafNodeAst->nodeType = NODE_BOOLEAN;
                leafNodeAst->integer = left_child->integer & right_child->integer;
                return leafNodeAst;
            case NODE_OR:
                leafNodeAst->nodeType = NODE_BOOLEAN;
                leafNodeAst->integer = left_child->integer | right_child->integer;
                return leafNodeAst;
            case NODE_NOT:
                leafNodeAst->nodeType = NODE_BOOLEAN;
                leafNodeAst->integer = ~ left_child->integer;
                return leafNodeAst;
            case NODE_EQUAL:
                leafNodeAst->nodeType = NODE_BOOLEAN;
                /* nodeAst->leftChild is also the "expression: (exp exp)" with nodeAst in "equal: ( = (exp exp) exp ......)" */
                if (left_child->nodeType == NODE_BOOLEAN) {
                    if (left_child->integer == 0) {
                        leafNodeAst->integer = 0;
                    }
                    else if (left_child->integer == -1) {
                        leafNodeAst->integer = -(left_child->rightChild->integer == right_child->integer);
                    }
                }
                else {
                    leafNodeAst->integer = -(left_child->integer == right_child->integer);
                }
                return leafNodeAst;
            case NODE_ADDITION:
                leafNodeAst->nodeType = NODE_INTEGER;
                leafNodeAst->integer = left_child->integer + right_child->integer;
                return leafNodeAst;
            case NODE_SUBTRACTION:
                leafNodeAst->nodeType = NODE_INTEGER;
                leafNodeAst->integer = left_child->integer - right_child->integer;
                return leafNodeAst;
            case NODE_MULTIPLICATION:
                leafNodeAst->nodeType = NODE_INTEGER;
                leafNodeAst->integer = left_child->integer * right_child->integer;
//                printf("hi multiple!, %d\n", leafNodeAst->integer); // debug
                return leafNodeAst;
            case NODE_DIVISION:
                leafNodeAst->nodeType = NODE_INTEGER;
                leafNodeAst->integer = left_child->integer / right_child->integer;
                return leafNodeAst;
            case NODE_MODULUS:
                leafNodeAst->nodeType = NODE_INTEGER;
                leafNodeAst->integer = left_child->integer % right_child->integer;
                return leafNodeAst;
            default:
                /* shall never pass through here */
                return leafNodeAst;
        }
    }
}

NodeAST* judgeIF_EXPRESSION(Node_If_AST* node_If_Ast) {

    int judgement;

//    puts("test"); // debug
    judgement = evaluateExpression(node_If_Ast->testChild)->integer;

    if (judgement == -1) {
//        puts("then"); // debug
        return evaluateExpression(node_If_Ast->thenChild);
    }

    else if (judgement == 0) {
//        puts("else"); // debug
        return evaluateExpression(node_If_Ast->elseChild);
    }

    else {
        /* shall never pass through here */
        return (NodeAST*)node_If_Ast;
    }
}

NodeAST* handleFUNCTION_CALL(NodeAST* nodeAst) {

    NodeAST* functionReturnNodeAst;
    int numberOfArguments = 0;

    /* anonymous function */
    if (nodeAst->leftChild->nodeType == NODE_FUNCTION) {

        char* functionName = malloc(30);
        sprintf(functionName, "-anonymous-%d", anonymousFunctionOrder);
        anonymousFunctionOrder++;
        currentFunctionNameBeingVisited = functionName;

        /* bind the function name `-anonymous-X` with the expression `nodeAst->leftChild->rightChild` */
        pushIntoGlobalSymbolTable(functionName, nodeAst->leftChild->rightChild, &globalLinkedList);

        /* push the parameters into AddressOfParameterTable */
        traversalPARAMETER(nodeAst->leftChild->leftChild, currentFunctionNameBeingVisited, 0);
    }

    /* named function */
    else {
        currentFunctionNameBeingVisited = nodeAst->leftChild->string;
    }

    /* traversal arguments tree and push arguments into passed argument stack */
    traversalARGUMENT(nodeAst->rightChild, &numberOfArguments);
//    printf("hi2!, meet function call,  numberOfArguments = %d\n", numberOfArguments); // debug

    /* push basePtrOfArgument into passed argument stack */
    stackPtrOfArgument++;
    passedArgumentStack[stackPtrOfArgument] = basePtrOfArgument;
    basePtrOfArgument = stackPtrOfArgument;

    /* call the function */
    functionReturnNodeAst = evaluateExpression(findGlobalSymbolTableNode(currentFunctionNameBeingVisited));

    /* pop basePtrOfArgument from passed argument stack */
    basePtrOfArgument = passedArgumentStack[stackPtrOfArgument];
    stackPtrOfArgument--;

    /* pop the arguments from passed argument stack */
    stackPtrOfArgument -= numberOfArguments;

    /* return to global scope */
    currentFunctionNameBeingVisited = "-none";

    return functionReturnNodeAst;
}

void traversalPARAMETER(NodeAST* nodeAst, char* functionName, int offsetAddress) {

    /* visit part */
    if (nodeAst->nodeType == NODE_STRING) {
        pushIntoAddressOfParameterTable(functionName, nodeAst->string, offsetAddress);
//        printf("hi1!, push parameter    ,  node->string: %s, offset: %d\n", nodeAst->string, offsetAddress); // debug
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
    if (nodeAst == NULL) {
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
//    printf("hi3!, find variable     ,  functionName: %s, variable: %s\n", functionName, identifier); // debug
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
