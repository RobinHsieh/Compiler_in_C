%{
#define MAX_LINE 100

char* lineStream[MAX_LINE];

int indentation = 0;
int countIndentation[MAX_LINE] = {0};
int countLine = 0;
%}
%pointer
%x NO_FRONT_SPACE
%x AFTER_LCB

SPACE " "
LINE  [^\n{}]
LCB   "{"
RCB   "}"
EOL   \n

%%

{SPACE}+                {
                         BEGIN NO_FRONT_SPACE;
}

{LCB}                   {
                         strcat(lineStream[countLine], "{");
                         countIndentation[countLine] = indentation;
                         indentation++;
                         BEGIN AFTER_LCB;
}

{RCB}                   {
                         strcat(lineStream[countLine], "}");
                         indentation--;
                         countIndentation[countLine] = indentation;
                         BEGIN NO_FRONT_SPACE;
}

[^ \n{}]{LINE}*         {
                         strcat(lineStream[countLine], yytext);
                         countIndentation[countLine] = indentation;
                         BEGIN NO_FRONT_SPACE;
}

{EOL}                   {
                         countIndentation[countLine] = indentation;
                         countLine++;
                         BEGIN INITIAL;
}

<NO_FRONT_SPACE>{LCB}   {
                         strcat(lineStream[countLine], "{");
                         countIndentation[countLine] = indentation;
                         indentation++;
                         BEGIN AFTER_LCB;
}

<NO_FRONT_SPACE>{RCB}   {
                         strcat(lineStream[countLine], "}");
                         indentation--;
                         countIndentation[countLine] = indentation;
}

<NO_FRONT_SPACE>{LINE}+ {
                         strcat(lineStream[countLine], yytext);
                         countIndentation[countLine] = indentation;
}

<NO_FRONT_SPACE>{EOL}   {
                         countIndentation[countLine] = indentation;
                         countLine++;
                         BEGIN INITIAL;
}

<AFTER_LCB>{LCB}        {
                         strcat(lineStream[countLine], "{");
                         indentation++;
                         BEGIN AFTER_LCB;
}

<AFTER_LCB>{RCB}        {
                         strcat(lineStream[countLine], "}");
                         indentation--;
}

<AFTER_LCB>{LINE}+      {
                         strcat(lineStream[countLine], yytext);
}

<AFTER_LCB>{EOL}        {
                         countLine++;
                         BEGIN INITIAL;
}

%%

int main(int argc, char *agrv[]) {

    for (size_t i = 0; i < MAX_LINE; i++) {
        lineStream[i] = (char*)malloc(50);
    }

    yylex();

    for (size_t i = 0; i <= countLine; i++) {
        for (size_t j = 0; j < countIndentation[i]; j++) {
            printf("%s", "\t");
        }
        if (i <= countLine - 1) {
            printf("%s\n", lineStream[i]);
        }
        else {
            printf("%s", lineStream[i]);
        }
    }

#if 0
    for (size_t i = 0; i <= countLine; i++) {
        printf("%d\n", countIndentation[i]);
    }
#endif

    return 0;
}
