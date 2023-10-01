%option noyywrap
%{

#include "xml.tab.h"

%}

AND_START "<and>"
AND_END   "</and>"
OR_START  "<or>"
OR_END    "</or>"
NOT_START "<not>"
NOT_END   "</not>"
TRUE      "<true/>"
FALSE     "<false/>"

%%

{AND_START} {
             yylval.bitwise = -1;
             return(AND_START);
}
{AND_END}   {
             yylval.bitwise = -1;
             return(AND_END);
}
{OR_START}  {
             yylval.bitwise = 0;
             return(OR_START);
}
{OR_END}    {
             yylval.bitwise = 0;
             return(OR_END);
}
{NOT_START} {
             return(NOT_START);
}

{NOT_END}   {
             return(NOT_END);
}

{TRUE}      {
             yylval.bitwise = -1;
             return(TRUE);
}
{FALSE}     {
             yylval.bitwise = 0;
             return(FALSE);
}

[ \n]       {}

%%