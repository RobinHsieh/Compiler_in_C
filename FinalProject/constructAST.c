#include "constructAST.h"


GlobalSymbolTable* globalLinkedList;
AddressOfParameterTable addressOfParameterTable[2048];
int topOfAddressOfParameterTable = -1;

char* activeFunctionNameStack[2048];
int topOfActiveFunctionNameStack = 0;

int anonymousFunctionOrder = 1;

NodeAST* passedArgumentStack[2048];
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
    newNode->conditionChild = testChild;
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
                if (nodeAst->rightChild->nodeType != NODE_FUNCTION_CALLEE) {
                    /* bind the variable name `nodeAst->leftChild->string` with the expression `nodeAst->rightChild` */
                    pushIntoGlobalSymbolTable(nodeAst->leftChild->string, SYMBOL_VARIABLE, nodeAst->rightChild, &globalLinkedList);
                }

                /* going to define a function */
                else {
                    /* bind the function name `nodeAst->leftChild->string` with the expression `nodeAst->rightChild->rightChild` */
                    pushIntoGlobalSymbolTable(nodeAst->leftChild->string, SYMBOL_FUNCTION_NAME, nodeAst->rightChild->rightChild, &globalLinkedList);

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

    /* Kindly reminder
     *
     * The methods of evaluating expression below can be roughly classified into three types:
     * 1. Traverse downwards, use Inherited attribute: IF_EXPRESSION, FUNCTION_CALLER, VARIABLE
     * 2. Visit leaf nodes, get the semantic value and start to return: INTEGER, BOOLEAN
     * 3. Return upwards, use Synthesized attribute: GREATER, SMALLER, AND, OR, NOT, EQUAL, ADDITION, SUBTRACTION, MULTIPLICATION, DIVISION, MODULUS
     */


    /* Type 1: Traverse downwards, use Inherited attribute */
    if (nodeAst->nodeType == NODE_VARIABLE) {

        NodeAST* newExpressionNodeAst = NULL;
        NodeAST* newLeafNodeAst;
        SymbolType symbolType;

        /* deal with variable */
        if (strcmp(activeFunctionNameStack[topOfActiveFunctionNameStack], "-none") == 0) {
            newExpressionNodeAst = findGlobalSymbolTableNode(nodeAst->leftChild->string, &symbolType);
        }

        /* deal with parameter */
        else {
            int address = findAddressOfParameterTable(activeFunctionNameStack[topOfActiveFunctionNameStack], nodeAst->leftChild->string);
            if (address != 0) {
                /* parameter is a expression except function expression */
                if (passedArgumentStack[basePtrOfArgument - address]->nodeType == NODE_INTEGER || passedArgumentStack[basePtrOfArgument - address]->nodeType == NODE_BOOLEAN) {
                    newLeafNodeAst = addNode(passedArgumentStack[basePtrOfArgument - address]->nodeType, passedArgumentStack[basePtrOfArgument - address]->integer, NULL, NULL, NULL);
                }
                /* parameter is a function expression */
                else {
                    newLeafNodeAst = passedArgumentStack[basePtrOfArgument - address];
                }
            }
            else {
                newExpressionNodeAst = findGlobalSymbolTableNode(nodeAst->leftChild->string, &symbolType);
            }
        }

        /* don't need to evaluate the function name when traversing the arguments */
        if (newExpressionNodeAst) {
            if (symbolType == SYMBOL_FUNCTION_NAME) {
                newLeafNodeAst = nodeAst->leftChild;
            }
            else {
                newLeafNodeAst = evaluateExpression(newExpressionNodeAst);
            }
        }
        return newLeafNodeAst;
    }

    /* Type 1: Traverse downwards, use Inherited attribute */
    else if (nodeAst->nodeType == NODE_IF_EXPRESSION) {
        return judgeIF_EXPRESSION((Node_If_AST*)nodeAst);
    }

    /* Type 1: Traverse downwards, use Inherited attribute */
    else if (nodeAst->nodeType == NODE_FUNCTION_CALLER) {

        /* handle the function call */
        return handleFUNCTION_CALLER(nodeAst);
    }

    /* Type 2: Visit leaf nodes, get the semantic value and start to return */
    else if (nodeAst->nodeType == NODE_INTEGER || nodeAst->nodeType == NODE_BOOLEAN) {
        NodeAST* newLeafNodeAst = malloc(sizeof(NodeAST));
        newLeafNodeAst->nodeType = nodeAst->nodeType;
        newLeafNodeAst->integer = nodeAst->integer;
        return newLeafNodeAst;
    }

    /* Type 3: Return upwards, use Synthesized attribute */
    else {
        NodeAST* left_child = evaluateExpression(nodeAst->leftChild);
        NodeAST* right_child;

        /* prevent NULL when visit NODE_NOT node */
        if (nodeAst->rightChild != NULL) {
            right_child = evaluateExpression(nodeAst->rightChild);
        }
        NodeAST* newNodeAst = malloc(sizeof(NodeAST));

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

    judgement = evaluateExpression(node_If_Ast->conditionChild)->integer;

    if (judgement == -1) {
        return evaluateExpression(node_If_Ast->thenChild);
    }

    /* (judgement == 0) */
    else {
        return evaluateExpression(node_If_Ast->elseChild);
    }
}

NodeAST* handleFUNCTION_CALLER(NodeAST* nodeAst) {

    NodeAST* functionReturnNodeAst;
    NodeAST* functionExpressionNodeAst;
    SymbolType symbolType;
    int numberOfArguments = 0;
    char* functionName = malloc(20);
    int address;

    /* note: need to traversal arguments before update the current function scope,
     * because if argument are parameters, these parameters are belong to old function scope instead of new. */
    /* traversal arguments tree and push arguments into the passed argument stack */
    traversalARGUMENT(nodeAst->rightChild, &numberOfArguments);

    /* anonymous function */
    if (nodeAst->leftChild->nodeType == NODE_FUNCTION_CALLEE) {

        /* create a new function name `-anonymous-X` */
        sprintf(functionName, "-anonymous-%d", anonymousFunctionOrder);
        anonymousFunctionOrder++;

        /* bind the function name `-anonymous-X` with the expression `nodeAst->leftChild->rightChild` */
        pushIntoGlobalSymbolTable(functionName, SYMBOL_FUNCTION_NAME, nodeAst->leftChild->rightChild, &globalLinkedList);

        /* push parameters into AddressOfParameterTable */
        traversalPARAMETER(nodeAst->leftChild->leftChild, functionName, 0);
    }
    /* named function of variable */
    else {
        strcpy(functionName, nodeAst->leftChild->string);
    }

    /* find the bound function expression from the function name */
    address = findAddressOfParameterTable(activeFunctionNameStack[topOfActiveFunctionNameStack], functionName);

    /* function is passed as an argument */
    if (strcmp(activeFunctionNameStack[topOfActiveFunctionNameStack], "-none") != 0 && address != 0) {

        /* argument is function expression */
        if (passedArgumentStack[basePtrOfArgument - address]->nodeType == NODE_FUNCTION_CALLEE) {

            /* create a new function name `-anonymous-X` */
            sprintf(functionName, "-anonymous-%d", anonymousFunctionOrder);
            anonymousFunctionOrder++;

            /* push parameters into AddressOfParameterTable */
            traversalPARAMETER(passedArgumentStack[basePtrOfArgument - address]->leftChild, functionName, 0);

            /* evaluate the function's expression */
            functionExpressionNodeAst = passedArgumentStack[basePtrOfArgument - address]->rightChild;
        }
        /* argument is function name */
        else {
            strcpy(functionName, passedArgumentStack[basePtrOfArgument - address]->string);
            functionExpressionNodeAst = findGlobalSymbolTableNode(functionName, &symbolType);
        }
    }
    /* function is a variable */
    else {
        functionExpressionNodeAst = findGlobalSymbolTableNode(functionName, &symbolType);
    }

    /* update the current function scope */
    topOfActiveFunctionNameStack++;
    activeFunctionNameStack[topOfActiveFunctionNameStack] = functionName;

    /* push basePtrOfArgument into the passed argument stack */
    stackPtrOfArgument++;
    passedArgumentStack[stackPtrOfArgument] = addNode(NODE_INTEGER, basePtrOfArgument, NULL, NULL, NULL);
    basePtrOfArgument = stackPtrOfArgument;

    /* call the function */
    functionReturnNodeAst = evaluateExpression(functionExpressionNodeAst);

    /* pop basePtrOfArgument from the passed argument stack */
    basePtrOfArgument = passedArgumentStack[stackPtrOfArgument]->integer;
    stackPtrOfArgument--;

    /* pop arguments from the passed argument stack */
    stackPtrOfArgument -= numberOfArguments;

    /* return the function scope */
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

        /* don't need to evaluate the function expression when traversing the arguments */
        if (nodeAst->leftChild->nodeType == NODE_FUNCTION_CALLEE) {
            passedArgumentStack[stackPtrOfArgument] = nodeAst->leftChild;
        }
        else {
            passedArgumentStack[stackPtrOfArgument] = evaluateExpression(nodeAst->leftChild);
        }
    }
}

void pushIntoAddressOfParameterTable(char* functionName, char* identifier, int offsetAddress) {
    /* push the parameters into AddressOfParameterTable */
    topOfAddressOfParameterTable++;
    addressOfParameterTable[topOfAddressOfParameterTable].functionName = functionName;
    addressOfParameterTable[topOfAddressOfParameterTable].identifier = identifier;
    addressOfParameterTable[topOfAddressOfParameterTable].offsetAddress = offsetAddress;
}

void pushIntoGlobalSymbolTable(char* identifier, SymbolType symbolType, NodeAST* expression, GlobalSymbolTable** head) {

    GlobalSymbolTable* directTable = *head;
    GlobalSymbolTable* previousTable = NULL;

    /* create a new directTable */
    GlobalSymbolTable* newTable = malloc(sizeof(GlobalSymbolTable));
    newTable->identifier = identifier;
    newTable->symbolType = symbolType;
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

NodeAST* findGlobalSymbolTableNode(char* identifier, SymbolType* symbolType) {

    GlobalSymbolTable* table = globalLinkedList;

    /* iteration */
    while (table != NULL) {
        if (strcmp(table->identifier, identifier) == 0) {
            *symbolType = table->symbolType;
            return table->expression;
        }
        table = table->nextPtr;
    }

    /* shall never pass through here */
    *symbolType = table->symbolType;
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
