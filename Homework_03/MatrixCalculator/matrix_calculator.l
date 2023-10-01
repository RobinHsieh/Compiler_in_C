%option noyywrap
%{

#include "matrix_calculator.tab.h"
int errorIndex = 0;

%}

%%

\n      {;}

[ \t]+  {
         errorIndex += yyleng;
}

[+-]    {
         errorIndex += yyleng;
         printf("%s ", yytext);//
         yylval.loc = errorIndex;
         return yytext[0];
}

"*"     {
         errorIndex += yyleng;
         printf("%s ", yytext);//
         yylval.loc = errorIndex;
         return yytext[0];
}

","     {
         errorIndex += yyleng;
         printf("%s ", ",");//
         return yytext[0];
}

"["     {
         errorIndex += yyleng;
         printf("%s ", "[");//
         return yytext[0];
}  // Left Square BraCket

"]"     {
         errorIndex += yyleng;
         printf("%s ", "]");//
         return yytext[0];
}  // Right Square BraCket

"("     {
         errorIndex += yyleng;
         printf("%s ", "(");//
         return yytext[0];
}   // Left BraCket

")"     {
         errorIndex += yyleng;
         printf("%s ", ")");//
         return yytext[0];
}   // Right BraCket

"^T"    {
         errorIndex += yyleng;
         printf("%s ", yytext);//
         return TRANS;
}    // TransPose

[0-9]+  {
         errorIndex += yyleng;
         printf("%d ", atoi(yytext));
         yylval.ival = atoi(yytext);
         return NUM;
}

.       {;}

%%