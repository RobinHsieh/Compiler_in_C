%option noyywrap
%{

#include "constructAST.h"
#include "pc.tab.h"

%}

NUMBER      [0-9]+
PERMUTATION [Pp]
COMBINATION [Cc]

%%

{PERMUTATION} {
               return(PERMUTATION);
}

{COMBINATION} {
               return(COMBINATION);
}

"+"           {
               return(yytext[0]);
}

"-"           {
               return(yytext[0]);
}

{NUMBER}      {
               yylval.value = atoi(yytext);
               return(NUMBER);
}

[ ]           {;}

%%