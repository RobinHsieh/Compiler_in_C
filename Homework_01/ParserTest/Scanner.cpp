#include <cstdio>
#include "Scanner.h"


token scanner(char localStreams[SIZE], size_t* indexPtr) {
    struct token Token = {};

    if ('A' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 'Z' ||
        'a' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 'z' ||
        localStreams[*indexPtr] == '_') {

        Token = scanID(localStreams, indexPtr);
    }

    else if (localStreams[*indexPtr] == '\"') {
        Token = scanStringLiteral(localStreams, indexPtr);
    }

    else {

        switch (localStreams[*indexPtr]) {
            case '(':
                strcpy(Token.type, "LBR");
                strcpy(Token.value, "(");
                *indexPtr += 1;
                break;
            case ')':
                strcpy(Token.type, "RBR");
                strcpy(Token.value, ")");
                *indexPtr += 1;
                break;
            case '.':
                strcpy(Token.type, "DOT");
                strcpy(Token.value, ".");
                *indexPtr += 1;
                break;
        }
    }

    return Token ;
}


token scanID(char* localStreams, size_t* indexPtr) {
    struct token Token = { "ID", 0};

    const char* sPtr;
    bool statementJudging;

    //[A-Za-z_]
    statementJudging = ('A' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 'Z') ||
                       ('a' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 'z') ||
                       localStreams[*indexPtr] == '_';
    if (statementJudging && *indexPtr <= strlen(localStreams) - 1) {
        strncat(Token.value, localStreams + *indexPtr, 1);
        *indexPtr += 1;
    }
    else {
        strcpy(Token.type, "InCorrect");
        *indexPtr += 1;

        //puts("hi");
    }

    //[0-9A-Za-z_]*
    statementJudging = ('0' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= '9') ||
                       ('A' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 'Z') ||
                       ('a' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 'z') ||
                       localStreams[*indexPtr] == '_';
    while (statementJudging && *indexPtr <= strlen(localStreams) - 1) {
        strncat(Token.value, localStreams + *indexPtr, 1);
        *indexPtr += 1;
        statementJudging = ('0' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= '9') ||
                           ('A' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 'Z') ||
                           ('a' <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 'z') ||
                           localStreams[*indexPtr] == '_';
    }

    sPtr = strchr(" \"().", localStreams[*indexPtr]);
    //Incorrect
    if (sPtr == nullptr) {
        strcpy(Token.type, "InCorrect");
        *indexPtr += 1;
    }
        //Correct
    else {
        //do nothing
    }

    return Token;
}


token scanStringLiteral(char* localStreams, size_t* indexPtr) {
    struct token Token = { "STRLIT", 0};

    //boolean = have_value_in_STRLIT_and_find_the_end_Quotation_mark
    bool statementJudging = false;

    //Start with "
    if (localStreams[*indexPtr] == '\"') {
        *indexPtr += 1;
    }

    //End with " but no value in STRLIT
    if (localStreams[*indexPtr] == '\"') {
        strcpy(Token.type, "InCorrect");
        *indexPtr += 1;
    }
    else {

        //Scan the value
        while (!statementJudging && *indexPtr <= strlen(localStreams) - 1) {

            //Read value
            if (localStreams[*indexPtr] != '\"') {
                strncat(Token.value, localStreams + *indexPtr, 1);
                *indexPtr += 1;
            }

                //Find the end Quotation mark
            else {
                statementJudging = true;
                *indexPtr += 1;
            }
        }

        //Incorrect
        if (!statementJudging) {
            strcpy(Token.type, "InCorrect");
        }
            //Correct
        else {
            //do nothing
        }
    }

    return Token;
}

