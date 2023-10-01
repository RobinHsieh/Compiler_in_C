%{

// #define DEBUG
#define MAX_LINE 100

int single_comment[MAX_LINE] = {0};
int multi_comment_f[MAX_LINE] = {0};
int multi_comment_b[MAX_LINE] = {0};
int print_statement[MAX_LINE] = {0};
int if_statement[MAX_LINE] = {0};
int tab[MAX_LINE] = {0};
int if_b[MAX_LINE] = {0};

char* lineStream[MAX_LINE];
int countLine = 0;

%}

%pointer
%x SINGLE_COM_STATE
%x MULTI_COM_STATE
%x PRINT_STATE
%x IF_CONDITION_STATE
%x IF_STATE

SINGLE_COM  "// "
MULTI_COM_F "/*"
MULTI_COM_B "*/"
PRINT_F     "cout << "
PRINT_B     " << endl;"
IF_F        "if("
IF_M        ") {"
IF_B        "}"
TAB         "    "
EOL         \n

IN_SINGLE_COM [^\n]*
IN_MULTI_COM  .
IN_PRINT      (["][^"]*["])|[0-9]+
IN_IF         (true)|(false)

%%

{SINGLE_COM}    {
                 single_comment[countLine] = 1;
                 BEGIN SINGLE_COM_STATE;
}
{MULTI_COM_F}   {
                 multi_comment_f[countLine] = 1;
                 BEGIN MULTI_COM_STATE;
}
{PRINT_F}       {
                 print_statement[countLine] = 1;
                 BEGIN PRINT_STATE;
}
{PRINT_B}       {
                 // do nothing
}
{IF_F}          {
                 if_statement[countLine] = 1;
                 BEGIN IF_CONDITION_STATE;
}
{IF_M}          {
                 // do nothing
}
{IF_B}          {
                 if_b[countLine] = 1;
}
{TAB}           {
                 tab[countLine] = 1;
}
{EOL}           {
                 countLine++;
}



<SINGLE_COM_STATE>{IN_SINGLE_COM} {
                 strcpy(lineStream[countLine], yytext);
                 BEGIN INITIAL;
}



<MULTI_COM_STATE>{MULTI_COM_B}    {
                 multi_comment_b[countLine] = 1;
                 BEGIN INITIAL;
}
<MULTI_COM_STATE>{IN_MULTI_COM}   {
                 strcat(lineStream[countLine], yytext);
}
<MULTI_COM_STATE>{EOL}            {
                 countLine++;
}



<PRINT_STATE>{IN_PRINT}     {
                 strcpy(lineStream[countLine], yytext);
                 BEGIN INITIAL;
}



<IF_CONDITION_STATE>{IN_IF} {
                 if (strcmp(yytext, "true") == 0) {
                     strcpy(lineStream[countLine], "True");
                 }
                 else if (strcmp(yytext, "false") == 0) {
                     strcpy(lineStream[countLine], "False");
                 }
                 BEGIN INITIAL;
}

%%

int main(int argc, char *argv[]) {

    for (size_t i = 0; i < MAX_LINE; i++) {
        lineStream[i] = (char*)malloc(50);
    }

    yylex();

#if !defined(DEBUG)
    //puts("------------------------------------");
    for (size_t i = 0; i <= countLine; i++) {

        if (tab[i] == 1) {
            printf("%s", "    ");
        }

        if (single_comment[i] == 1) {
            printf("%s", "# ");
            printf("%s\n", lineStream[i]);
        }
        else if (multi_comment_f[i] == 1) {
            printf("%s", "\"\"\"");
            printf("%s\n", lineStream[i]);
        }
        else if (multi_comment_b[i] == 1) {
            printf("%s", lineStream[i]);
            printf("%s\n", "\"\"\"");
        }
        else if (print_statement[i] == 1) {
            printf("print(%s)\n", lineStream[i]);
        }
        else if (if_statement[i] == 1) {
            printf("if %s:\n", lineStream[i]);
        }
        else if (if_b[i] == 1) {
            // do nothing
        }
        else {
            printf("%s\n", lineStream[i]);
        }

    }
#endif
#if defined(DEBUG)
    puts("------------------------------------");
    printf("%s\n", "s m m p i t");
    for (size_t i = 0; i <= countLine; i++) {

        printf("%d %d %d %d %d %d %s\n", single_comment[i], multi_comment_f[i], multi_comment_b[i], print_statement[i], if_statement[i], tab[i], lineStream[i]);

    }
    puts("------------------------------------");
#endif
    return 0;
}
