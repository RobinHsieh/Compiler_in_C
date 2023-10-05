#ifndef FINAL_PROJECT_CONSTRUCTAST_H
#define FINAL_PROJECT_CONSTRUCTAST_H

//#define DEBUG__PRINT_TREE_STRUCTURE
//#define DEBUG__TURN_OFF_TRAVERSAL
//#define DEBUG__FIX_FUNCTION_CALL

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum nodeType NodeType;
typedef struct globalSymbolTable GlobalSymbolTable;
typedef struct nodeAST NodeAST;
typedef struct node_If_AST Node_If_AST;
typedef struct addressOfParameterTable AddressOfParameterTable;

enum nodeType {
    /* nodeType for leaf nodes */
    NODE_INTEGER,           // 0
    NODE_BOOLEAN,           // 1
    NODE_STRING,            // 2


    /* nodeType for internal nodes */
    NODE_ADDITION,          // 3
    NODE_SUBTRACTION,       // 4
    NODE_MULTIPLICATION,    // 5
    NODE_DIVISION,          // 6
    NODE_MODULUS,           // 7

    NODE_GREATER,           // 8
    NODE_SMALLER,           // 9
    NODE_EQUAL,             // 10
    NODE_AND,               // 11
    NODE_OR,                // 12
    NODE_NOT,               // 13

    NODE_PRINT_NUM,         // 14
    NODE_PRINT_BOOL,        // 15

    NODE_IF_EXPRESSION,     // 16

    NODE_VARIABLE,          // 17
    NODE_DEFINE,            // 18

    NODE_PARAMETER,         // 19
    NODE_ARGUMENT,          // 20
    NODE_FUNCTION,          // 21
    NODE_FUNCTION_CALL,     // 22

    NODE_STATEMENT          // 23
};

struct globalSymbolTable {
    char* identifier;
    struct nodeAST* expression;
    struct globalSymbolTable* nextPtr;
};

struct addressOfParameterTable {
    char* functionName;
    char* identifier;
    int offsetAddress;
};

struct nodeAST {
    NodeType nodeType;
    int integer;
    char* string;
    struct nodeAST* leftChild;
    struct nodeAST* rightChild;
};

struct node_If_AST {
    NodeType nodeType;
    struct nodeAST* testChild;
    struct nodeAST* thenChild;
    struct nodeAST* elseChild;
};

/* construct Abstract Syntax Tree */
NodeAST* addNode(NodeType nodeType, int integer, char* string, NodeAST* leftChild, NodeAST* rightChild);
NodeAST* add_If_Node(NodeType nodeType, NodeAST* testChild, NodeAST* thenChild, NodeAST* elseChild);

/* interpret Abstract Syntax Tree */
void traversalSTATMENT(NodeAST* nodeAst);
NodeAST* evaluateExpression(NodeAST* nodeAst);
NodeAST* judgeIF_EXPRESSION(Node_If_AST* node_If_Ast);
NodeAST* handleFUNCTION_CALL(NodeAST* nodeAst);
void traversalPARAMETER(NodeAST* nodeAst, char* functionName, int parameterOrder);
void traversalARGUMENT(NodeAST* nodeAst, int* numberOfArguments);
#if defined(DEBUG__PRINT_TREE_STRUCTURE)
void traversalAST_preorder(NodeAST* root);
void traversalAST_inorder(NodeAST* root);
void traversalAST_postorder(NodeAST* root);
#endif

/* deal with global symbol table */
void pushIntoGlobalSymbolTable(char* identifier, NodeAST* expression, GlobalSymbolTable** head);
NodeAST* findGlobalSymbolTableNode(char* identifier);
int findAddressOfParameterTable(char* functionName, char* identifier);

/* deal with address of parameter table*/
void pushIntoAddressOfParameterTable(char* functionName, char* identifier, int offsetAddress);

#endif //FINAL_PROJECT_CONSTRUCTAST_H
