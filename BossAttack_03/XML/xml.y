%{

#include "xml.tab.h"
#include <stdio.h>

int answer;

int yylex();
void yyerror (const char *message);

%}

%union {

int bitwise;

}

%token<bitwise> AND_START AND_END OR_START OR_END NOT_START NOT_END TRUE FALSE
%type<bitwise> sub_tag and_tail or_tail

%%

sub_tag    : and_tail AND_END {
           $$ = $1 & $2;
//           puts("    reduce and_tail AND_END -> sub_tag");//
           answer = $$;
//           printf("ans: %d\n", answer);
           }
           | or_tail OR_END {
           $$ = $1 | $2;
//           puts("    reduce or_tail OR_END -> sub_tag");//
           answer = $$;
//           printf("ans: %d\n", answer);
           }
           | NOT_START sub_tag NOT_END {
           $$ = ~ $2;
//           puts("    reduce NOT_START sub_tag NOT_END -> sub_tag");//
           answer = $$;
//           printf("ans: %d\n", answer);
           }
           | TRUE {
           $$ = $1;
//           puts("    reduce TRUE -> sub_tag");//
           answer = $$;
//           printf("ans: %d\n", answer);
           }
           | FALSE {
           $$ = $1;
//           puts("    reduce FALSE -> sub_tag");//
           answer = $$;
//           printf("ans: %d\n", answer);
           }
           ;

and_tail   : and_tail sub_tag {
           $$ = $1 & $2;
//           puts("    reduce and_tail sub_tag -> and_tail");//
//           printf("ans: %d\n", $$);
           }
           | AND_START {
           $$ = $1;
//           puts("    reduce AND_START -> and_tail");//
//           printf("ans: %d\n", $$);
           }
           ;

or_tail    : or_tail sub_tag {
           $$ = $1 | $2;
//           puts("    reduce or_tail sub_tag -> or_tail");//
//           printf("ans: %d\n", $$);
           }
           | OR_START {
           $$ = $1;
//           puts("    reduce OR_START -> or_tail");//
//           printf("ans: %d\n", $$);
           }
           ;

%%

void yyerror (const char *message) {
    printf("%s", "Syntax error");
}

int main(int argc, char *argv[]) {
    yyparse();
    if (answer) {
        printf("%s", "true");
    }
    else {
        printf("%s", "false");
    }
    return 0;
}
