%{

#include "pc.tab.h"
#include <stdio.h>

int answer;
int valid = 1;

int yylex();
void semanticError();
void yyerror (const char *message);
int factorial(int count);
int permutation(int m, int n);
int combination(int m, int n);

%}

%token NUMBER PERMUTATION COMBINATION

%%

start  : term {
       answer = $1;
       }
       ;

term   : term '+' factor {
       $$ = $1 + $3;
       }
       | term '-' factor {
       $$ = $1 - $3;
       }
       | factor {
       $$ = $1;
       }
       ;

factor : PERMUTATION NUMBER NUMBER {
       if ($2 < $3 || $2 > 12 || $3 > 12) {
           semanticError();
           return 0;
       }
       $$ = permutation($2, $3);
       }
       | COMBINATION NUMBER NUMBER {
       if ($2 < $3 || $2 > 12 || $3 > 12) {
           semanticError();
           return 0;
       }
       $$ = combination($2, $3);
       }
       | number {
       $$ = $1;
       }
       ;

number : NUMBER {
       $$ = $1;
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

int main(int argc, char *argv[]) {
    yyparse();
    if (valid) {
        printf("%d", answer);
    }
    return 0;
}