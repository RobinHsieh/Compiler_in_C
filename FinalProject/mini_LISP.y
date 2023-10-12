%{
/* Here is the Prologue section,
 * that contains macro definitions and declarations of functions
 * and variables that are used in the actions in the grammar rules.
 */

/* Q: Should be placed before include "mini_LISP.tab.h", why?
 * A: Because in "mini_LISP.tab.h", there have some data structure need to include "constructAST.h" first */
#include "constructAST.h"
#include "mini_LISP.tab.h"


NodeAST* root;

int yylex();
void semanticError();
void yyerror (const char *message);

%}

%union{
/* 1. Create an union structure to exchange multiple value types in the semantic value stack.
 *
 * 2. `%union` keyword identifies the yacc semantic value stack and `yyval` as a union.
 * 
 * 3. `yylval` is the semantic value of the look-ahead symbol, it will be pushed(shifted) into,
 *    or popped(reduced) from the yacc semantic value stack( , which defined as: YYSTYPE yyvsa[YYINITDEPTH] ).
 * 
 * 4. `YYSTYPE` is a macro or alias defined by `%union`,
 *    used to define the value type of the semantic value stack(`yyvsa[]`) and `yyval`.
 */
NodeAST* nodeASTptr;
int integer;
char* string;

}

%token '(' ')' PRINT_NUM PRINT_BOOL
%token '+' '-' '*' '/'
%token '>' '<' '='
%token MOD AND OR NOT FUNCTION DEFINE IF

%token<integer> BOOL_VAL NUMBER
%token<string>  ID

%type<nodeASTptr> stmt stmts print_stmt def_stmt
%type<nodeASTptr> exp variable ids params
%type<nodeASTptr> num_op plus plus_parts plus_part minus multiply multiply_parts multiply_part divide modulus greater smaller equal equal_parts equal_part
%type<nodeASTptr> logical_op and_op and_parts and_part or_op or_parts or_part not_op
%type<nodeASTptr> fun_exp fun_ids fun_body fun_call fun_name /* fun_stmts fun_stmt */
%type<nodeASTptr> if_exp test_exp then_exp else_exp

%%

program        : stmts {
               root = $1;
               }
               ;

stmts          : stmts stmt {
               $$ = addNode(NODE_STATEMENT, 0, NULL, $1, $2);
               }
               | stmt
               ;

stmt           : exp | def_stmt | print_stmt
               ;

print_stmt     : '(' PRINT_NUM exp ')' {
               $$ = addNode(NODE_PRINT_NUM, 0, NULL, $3, NULL);
               }
               | '(' PRINT_BOOL exp ')' {
               $$ = addNode(NODE_PRINT_BOOL, 0, NULL, $3, NULL);
               }
               ;

exp            : BOOL_VAL {
               $$ = addNode(NODE_BOOLEAN, $1, NULL, NULL, NULL);
               }
               | NUMBER {
               $$ = addNode(NODE_INTEGER, $1, NULL, NULL, NULL);
               }
               | variable {
               $$ = addNode(NODE_VARIABLE, 0, NULL, $1, NULL);
               }
               | num_op | logical_op
               | fun_exp | fun_call | if_exp
               ;

num_op         : plus | minus | multiply | divide | modulus | greater | smaller | equal
               ;

plus           : '(' plus_parts ')' {
               $$ = $2;
               }
               ;

plus_parts     : plus_parts exp {
               $$ = addNode(NODE_ADDITION, 0, NULL, $1, $2);
               }
               | plus_part
               ;

plus_part      : '+' exp exp {
               $$ = addNode(NODE_ADDITION, 0, NULL, $2, $3);
               }
               ;

minus          : '(' '-' exp exp ')' {
               $$ = addNode(NODE_SUBTRACTION, 0, NULL, $3, $4);
               }
               ;

multiply       : '(' multiply_parts ')' {
               $$ = $2;
               }
               ;

multiply_parts : multiply_parts exp {
               $$ = addNode(NODE_MULTIPLICATION, 0, NULL, $1, $2);
               }
               | multiply_part
               ;

multiply_part  : '*' exp exp {
               $$ = addNode(NODE_MULTIPLICATION, 0, NULL, $2, $3);
               }
               ;

divide         : '(' '/' exp exp ')' {
               $$ = addNode(NODE_DIVISION, 0, NULL, $3, $4);
               }
               ;

modulus        : '(' MOD exp exp ')' {
               $$ = addNode(NODE_MODULUS, 0, NULL, $3, $4);
               }
               ;

greater        : '(' '>' exp exp ')' {
               $$ = addNode(NODE_GREATER, 0, NULL, $3, $4);
               }
               ;

smaller        : '(' '<' exp exp ')' {
               $$ = addNode(NODE_SMALLER, 0, NULL, $3, $4);
               }
               ;

equal          : '(' equal_parts ')' {
               $$ = $2;
               }
               ;

equal_parts    : equal_parts exp {
               $$ = addNode(NODE_EQUAL, 0, NULL, $1, $2);
               }
               | equal_part
               ;

equal_part     : '=' exp exp {
               $$ = addNode(NODE_EQUAL, 0, NULL, $2, $3);
               }
               ;

logical_op     : and_op | or_op | not_op
               ;

and_op         : '(' and_parts ')' {
               $$ = $2;
               }
               ;

and_parts      : and_parts exp {
               $$ = addNode(NODE_AND, 0, NULL, $1, $2);
               }
               | and_part
               ;

and_part       : AND exp exp {
               $$ = addNode(NODE_AND, 0, NULL, $2, $3);
               }
               ;

or_op          : '(' or_parts ')' {
               $$ = $2;
               }
               ;

or_parts       : or_parts exp {
               $$ = addNode(NODE_OR, 0, NULL, $1, $2);
               }
               | or_part
               ;

or_part        : OR exp exp {
               $$ = addNode(NODE_OR, 0, NULL, $2, $3);
               }
               ;

not_op         : '(' NOT exp ')' {
               $$ = addNode(NODE_NOT, 0, NULL, $3, NULL);
               }
               ;

def_stmt       : '(' DEFINE variable exp ')' {
               $$ = addNode(NODE_DEFINE, 0, NULL, $3, $4);
               }
               ;

variable       : ID {
               $$ = addNode(NODE_STRING, 0, $1, NULL, NULL);
               }
               ;

fun_exp        : '(' FUNCTION fun_ids fun_body ')' {
               $$ = addNode(NODE_FUNCTION_CALLEE, 0, NULL, $3, $4);
               }
               ;

fun_ids        : '(' ids ')' {
               $$ = $2;
               }
               ;

ids            : variable ids {
               $$ = addNode(NODE_PARAMETER, 0, NULL, $1, $2);
               }
               | {
               $$ = NULL;
               }
               ;

fun_body       : exp
               ;

fun_call       : '(' fun_exp params ')' {
               $$ = addNode(NODE_FUNCTION_CALLER, 0, NULL, $2, $3);
               }
               | '(' fun_name params ')' {
               $$ = addNode(NODE_FUNCTION_CALLER, 0, NULL, $2, $3);
               }
               ;

params         : exp params {
               $$ = addNode(NODE_ARGUMENT, 0, NULL, $1, $2);
               }
               | {
               $$ = NULL;
               }
               ;

fun_name       : ID {
               $$ = addNode(NODE_STRING, 0, $1, NULL, NULL);
               }
               ;

if_exp         : '(' IF test_exp then_exp else_exp ')' {
               $$ = add_If_Node(NODE_IF_EXPRESSION, $3, $4, $5);
               }
               ;

test_exp       : exp
               ;

then_exp       : exp
               ;

else_exp       : exp
               ;

%%

void yyerror(const char* message) {
    printf("%s\n", "syntax error");
}

int main(int argc, char *argv[]) {
    yyparse();
#if defined(DEBUG__PRINT_TREE_STRUCTURE)
    traversalAST_preorder(root);
    puts("-------------");
    traversalAST_inorder(root);
    puts("-------------");
    traversalAST_postorder(root);
    puts("-------------");
#endif
    activeFunctionNameStack[0] = "-none";
#if !defined(DEBUG__TURN_OFF_TRAVERSAL)
    traversalSTATMENT(root);
#endif
#if defined(DEBUG__TURN_OFF_TRAVERSAL)
    puts("Parsing success!");
#endif
    return 0;
}
