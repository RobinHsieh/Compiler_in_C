%{

int nameCount = 0;

%}

NAME          [A-Z]+", "[A-Z][a-z]+"-"[A-Z][a-z]+
ID            [A-Z][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]
EMAIL_HEAD    [0-9]+"@"
EMAIL_TAIL    [a-z]+"."
BIRTHDAY_A_F  ([0-9][0-9]|[0-9][0-9][0-9]|[0-9][0-9][0-9][0-9])"/"
BIRTHDAY_A_M  (0[13578]|10|12)"/"
BIRTHDAY_A_B  0[1-9]|[1-2][0-9]|3[0-1]
BIRTHDAY_B_F  ([0-9][0-9]|[0-9][0-9][0-9]|[0-9][0-9][0-9][0-9])"/"
BIRTHDAY_B_M  (0[2469]|11)"/"
BIRTHDAY_B_B  0[1-9]|[1-2][0-9]|30
PHONE         "+886"[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]

%%

{NAME} {
    if(nameCount == 0) {
        printf("%s\n", yytext);
    }
    else {
        printf("\n%s\n", yytext);
    }
    nameCount++;
}
{ID} {
    printf("identity number: %s\n", yytext);
}
{EMAIL_HEAD}{EMAIL_TAIL}+[a-z]+ {
    printf("email: %s\n", yytext);
}
{BIRTHDAY_A_F}{BIRTHDAY_A_M}{BIRTHDAY_A_B} {
    printf("birthday: %s\n", yytext);
}
{BIRTHDAY_B_F}{BIRTHDAY_B_M}{BIRTHDAY_B_B} {
    printf("birthday: %s\n", yytext);
}
{PHONE} {
    printf("phone: %s\n", yytext);
}

%%

int main(int argc, char *agrv[]) {
    yylex();
    return 0;
}
