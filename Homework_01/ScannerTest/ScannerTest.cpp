#include <iostream>
using namespace std;
#include <list>
#include <cstring>
#define SIZE 1001


struct token {
    char type[20];
    char value[20];
};

//prototypes, aka declaration
token scanner(char *localStreams, size_t* indexPtr);
token scanDigits(char *localStreams, size_t* indexPtr);
//token scanner(char localStreams[SIZE], size_t* indexPtr);
//token scanDigits(char localStreams[SIZE], size_t* index);


char streams[SIZE] = {'\0'};
char input[SIZE];

int main() {
    static list<string> tokenStreams;
    static list<string> tokenStreamsValue;
    size_t index = 0;

    while (cin >> input) {
        //printf("%s", input);
        strcat(streams, input);
    }
    //puts("");
    while (index <= SIZE - 1 && streams[index] != '\0') {
        struct token A_token = scanner(streams, &index);

        cout << A_token.type;
        if (strcmp(A_token.type, "NUM") == 0) {
            cout << " " << A_token.value << endl;
        } else {
            cout << endl;
        }
    }


    //cout << endl;
    return 0;
}

//token scanner(char localStreams[SIZE], size_t* indexPtr) {
token scanner(char *localStreams, size_t* indexPtr) {
    struct token Token = {};

    // NUM: unsigned integer
    //if (strchr("0123456789", localStreams[*indexPtr]) != nullptr) {
    if (localStreams[*indexPtr] >= '0' && localStreams[*indexPtr] <= '9') {
        //puts("->hi3");
        Token = scanDigits(localStreams, indexPtr);
    }
    else {
        //puts("->hi4");
        switch (localStreams[*indexPtr]) {
            // PLUS: +
            case '+':
                strcpy(Token.type, "PLUS");
                *indexPtr += 1;
                break;
            case '-':
                strcpy(Token.type, "MINUS");
                *indexPtr += 1;
                break;
            case '*':
                strcpy(Token.type, "MUL");
                *indexPtr += 1;
                break;
            case '/':
                strcpy(Token.type, "DIV");
                *indexPtr += 1;
                break;
            case '(':
                strcpy(Token.type, "LPR");
                *indexPtr += 1;
                break;
            case ')':
                strcpy(Token.type, "RPR");
                *indexPtr += 1;
                break;
        }
    }
    //puts("one turn");
    return Token ;
}

//token scanDigits(char localStreams[SIZE], size_t* index) {
token scanDigits(char *localStreams, size_t* indexPtr) {
    struct token Token = { "NUM", 0};

    const char* sPtr;

    // when number == 0
    sPtr = strchr("0", localStreams[*indexPtr]);
    if (sPtr != nullptr) {
        strncat(Token.value, sPtr, 1);// strncat(Token.value, "0");
        *indexPtr += 1;
        sPtr = strchr("0123456789", localStreams[*indexPtr]);
        if (sPtr != nullptr) {
            // invalid input
        }
        //puts("->hi5");
    }
    else {

        // when number != 0
        bool statementJudging;

        statementJudging = 49 <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 57;
        while (statementJudging && *indexPtr <= strlen(localStreams) - 1) {
            strncat(Token.value, localStreams + *indexPtr, 1);
            *indexPtr += 1;
            statementJudging = 48 <= localStreams[*indexPtr] && localStreams[*indexPtr] <= 57;
            //puts("->hi6");
        }
    }

    strcpy(Token.type, "NUM");
    return Token;
}
