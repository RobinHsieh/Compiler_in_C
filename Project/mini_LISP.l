%option noyywrap
%{

#include "constructAST.h"
#include "mini_LISP.tab.h"

%}

/* exclusive state */
%x IGNORE_COMMENT_STATE

/* preliminary definitions */

SEPARATOR     [ \t\n\r]
LETTER        [a-z]
DIGIT         [0-9]

/* token definitions, upper case */

NUMBER        0|[1-9]{DIGIT}*|-[1-9]{DIGIT}*
ID            {LETTER}({LETTER}|{DIGIT}|-)*
BOOL_VAL      "#t"|"#f"

%%

"//"          {
              BEGIN IGNORE_COMMENT_STATE;
              }

<IGNORE_COMMENT_STATE>.  {
                         /* ignore comment */
                         }

<IGNORE_COMMENT_STATE>\n {
                         BEGIN INITIAL;
                         }

{SEPARATOR}   {
              /* do nothing */
              }


{NUMBER}      {
              yylval.integer = atoi(yytext);
              return NUMBER;
              }
{BOOL_VAL}    {
              yylval.integer = (yytext[1] == 't') ? -1 : 0 ;
              return BOOL_VAL;
              }


[()\+\-*/\>\<=] {
              // Both versions `[()\+\-*/><=]` and `[()\+\-*/\>\<=]` are correct.
              return yytext[0];
              }
"mod"         {
              return MOD;
              }


"and"         {
              return AND;
              }
"or"          {
              return OR;
              }
"not"         {
              return NOT;
              }


"define"      {
              return DEFINE;
              }
"fun"         {
              return FUNCTION;
              }
"if"          {
              return IF;
              }


"print-num"   {
              return PRINT_NUM;
              }
"print-bool"  {
              return PRINT_BOOL;
              }

{ID}          {
              yylval.string = malloc(sizeof(yyleng + 1));
              strcpy(yylval.string, yytext);
              return ID;
              }

%%