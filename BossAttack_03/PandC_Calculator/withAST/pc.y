%{

#include "constructAST.h" // shoud be placed before include "pc.tab.h", why?
#include "pc.tab.h"
#include <stdio.h>


NodeAST* root;
int valid = 1;

int yylex();
void semanticError();
void yyerror (const char *message);

%}

%union{

NodeAST* nodeASTptr;
int value;

}

%token<value> NUMBER
%token PERMUTATION COMBINATION
%type<nodeASTptr> start term factor number

%%

start  : term {
       root = $1;
       }
       ;

term   : term '+' factor {
       $$ = addNode(NODE_ADDITION, 0, $1, $3);
       }
       | term '-' factor {
       $$ = addNode(NODE_SUBTRACTION, 0, $1, $3);
       }
       | factor {
       $$ = $1;
       }
       ;

factor : PERMUTATION number number {
       if ($2->value < $3->value || $2->value > 12 || $3->value > 12) {
           semanticError();
           return 0;
       }
       $$ = addNode(NODE_PERMUTATION, 0, $2, $3);
       }
       | COMBINATION number number {
       if ($2->value < $3->value || $2->value > 12 || $3->value > 12) {
           semanticError();
           return 0;
       }
       $$ = addNode(NODE_COMBINATION, 0, $2, $3);
       }
       | number {
       $$ = $1;
       }
       ;

number : NUMBER {
       $$ = addNode(NODE_VALUE, $1, NULL, NULL);
       }
       ;

%%

void semanticError() {
    printf("%s", "Wrong Formula");
}

void yyerror(const char* message) {
    valid = 0;
    printf("%s", "Wrong Formula");
}

int main(int argc, char *argv[]) {
    yyparse();
    if (valid) {
        int interpretedAnswer = interpretAST(root);
        printf("%d", interpretedAnswer);
    }
    return 0;
}