#include "Scanner.h"
#include "Parser.h"

#ifndef SIZE
#define SIZE 1001
#endif


//only global and accessible on main.cpp
static char streams[SIZE] = {'\0'};
static char input[SIZE];


//prototypes, aka declaration
string get(list<string> &myList, size_t listIndex);

int main() {
    static list<string> tokenStreamsType;
    static list<string> tokenStreamsValue;
    size_t index = 0;

    while (cin >> input) {
        strcat(streams, input);
    }

    while (index <= SIZE - 1 && streams[index] != '\0') {
        struct token A_token = scanner(streams, &index);

        tokenStreamsType.push_back(A_token.type);
        tokenStreamsValue.push_back(A_token.value);
    }

    struct token A_end_token = {"$", 0};
    tokenStreamsType.push_back(A_end_token.type);
    tokenStreamsValue.push_back(A_end_token.value);

    bool syntacticValidity = parser(tokenStreamsType);

    if (syntacticValidity) {
        for (size_t i = 0; i < tokenStreamsType.size() - 1; i++) {

            string typePtr = get(tokenStreamsType, i);
            if (strcmp(typePtr.c_str(), "STRLIT") == 0) {
                cout << get(tokenStreamsType, i) << " ";
                cout << "\"" << get(tokenStreamsValue, i) << "\"" << endl;
            }
            else {
                cout << get(tokenStreamsType, i) << " ";
                cout << get(tokenStreamsValue, i) << endl;
            }
        }
    }
    else {
        cout << "invalid input" << endl;
    }

    return 0;
}
