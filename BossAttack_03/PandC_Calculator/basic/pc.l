%option noyywrap
%{

#include "pc.tab.h"

%}

NUMBER      [0-9]+
PERMUTATION [Pp]
COMBINATION [Cc]

%%

{PERMUTATION} {
               return(PERMUTA);
}

{COMBINATION} {
               return(COMBINA);
}

"+"           {
               return(yytext[0]);
}

"-"           {
               return(yytext[0]);
}

{NUMBER}      {
               yylval = atoi(yytext);
               return(NUMBER);
}

[ ]           {;}

%%