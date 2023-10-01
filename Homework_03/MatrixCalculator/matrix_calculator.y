%{

#include "matrix_calculator.tab.h"
#include <stdio.h>

int yylex();
void sematicerror(int errorIndex);
void yyerror (const char *message);

struct matrixSize {
    int row;
    int column;
};

%}

%union
{
    int ival;
    int loc;
    struct matrixSize* MatrixSize;
}

%token<ival> NUM
%token<loc> '-' '+' '*'
%token TRANS
%token END

%type<MatrixSize> expr term factor matrix

%%

start: term {
      printf("Accepted\n");
      puts("    reduce  term -> start");//
      return 0;
      }
      ;

term  : term '+' factor {
      if ($1->row == $3->row && $1->column == $3->column) {
          printf("sizeof=%ld", sizeof($$));
          $$ = malloc(sizeof(struct matrixSize));

          $$->row = $1->row;
          $$->column = $1->column;
          puts("    reduce term '+' factor -> term");//
      }
      else {
          sematicerror($2);
          return 0;
      }
      }
      | factor {

      printf("sizeof=%ld", sizeof($$));
      $$ = malloc(sizeof(struct matrixSize));

      $$->row = $1->row;
      $$->column = $1->column;
      puts("    reduce factor -> term");//
      }
      ;

factor: factor '*' expr {
      if ($1->column == $3->row) {
          printf("sizeof=%ld", sizeof($$));
          $$ = malloc(sizeof(struct matrixSize));

          $$->row = $1->row;
          $$->column = $3->column;
          puts("    reduce factor '*' expr -> factor");//
      }
      else {
          sematicerror($2);
          return 0;
      }
      }
      | expr {
      printf("sizeof=%ld", sizeof($$));
      $$ = malloc(sizeof(struct matrixSize));

      $$->row = $1->row;
      $$->column = $1->column;
      puts("    reduce expr -> factor");//
      }
      ;

expr  : expr TRANS {
      printf("sizeof=%ld", sizeof($$));
      $$ = malloc(sizeof(struct matrixSize));

      $$->row = $1->column;
      $$->column = $1->row;
      puts("    reduce expr TRANS -> expr");//
      }
      | '(' term ')' {
      puts("    reduce '(' term ')' -> expr");//
      printf("sizeof=%ld", sizeof($$));
      $$ = malloc(sizeof(struct matrixSize));

      $$->row = $2->row;
      $$->column = $2->column;
      }
      | matrix {
      printf("sizeof=%ld", sizeof($$));
      $$ = malloc(sizeof(struct matrixSize));

      $$->row = $1->row;
      puts("    reduce matrix -> expr");//
      $$->column = $1->column;
      }
      ;

matrix: '[' NUM ',' NUM ']' {
      printf("sizeof=%ld", sizeof($$));
      printf("sizeof$2=%ld", sizeof($2));
      $$ = malloc(sizeof(struct matrixSize));// important!!!!!!

      $$->row = $2;
      $$->column = $4;
      puts("    reduce '[' NUM ',' NUM ']' -> matrix");//
      }
      ;

%%

void sematicerror(int errorIndex) {
    printf("Semantic error on col %ld\n", errorIndex);
}

void yyerror(const char* message) {
    printf("%s\n", "Syntax error");
}

int main(int argc, char *argv[]) {
    yyparse();
    return(0);
}
