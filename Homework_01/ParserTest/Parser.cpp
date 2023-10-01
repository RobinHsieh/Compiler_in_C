#include "Parser.h"


bool parser(list<string> &tokenList) {
    list<string>::iterator tokenIndex = tokenList.begin();
    bool syntacticValidity = true;

    stmts(tokenList, &tokenIndex, &syntacticValidity);

    return syntacticValidity;
}

// NOLINTNEXTLINE(misc-no-recursion)
void stmts(list<string> &tokenList, list<string>::iterator* tokenIndexPtr, bool* syntacticValidityPtr) {

    if (strcmp((*tokenIndexPtr)->c_str(), "STRLIT") == 0 || strcmp((*tokenIndexPtr)->c_str(), "ID") == 0) {
        stmt(tokenList, tokenIndexPtr, syntacticValidityPtr);
        stmts(tokenList, tokenIndexPtr, syntacticValidityPtr);
    }
    else if(strcmp((*tokenIndexPtr)->c_str(), "$") == 0){
        //do nothing
    }
    else {
        *syntacticValidityPtr = false;
    }
}

// NOLINTNEXTLINE(misc-no-recursion)
void stmt(list<string> &tokenList, list<string>::iterator* tokenIndexPtr, bool* syntacticValidityPtr) {

    if (strcmp((*tokenIndexPtr)->c_str(), "ID") == 0) {
        primary(tokenList, tokenIndexPtr, syntacticValidityPtr);
    }
    else if (strcmp((*tokenIndexPtr)->c_str(), "STRLIT") == 0) {
        match(tokenIndexPtr, "STRLIT", syntacticValidityPtr);
    }
    else if (strcmp((*tokenIndexPtr)->c_str(), "RBR") == 0) {
        //do nothing
    }
    else {
        *syntacticValidityPtr = false;
    }
}

// NOLINTNEXTLINE(misc-no-recursion)
void primary(list<string> &tokenList, list<string>::iterator* tokenIndexPtr, bool* syntacticValidityPtr) {

    if (strcmp((*tokenIndexPtr)->c_str(), "ID") == 0) {

        match(tokenIndexPtr, "ID", syntacticValidityPtr);
        primaryTail(tokenList, tokenIndexPtr, syntacticValidityPtr);
    }
    else if(strcmp((*tokenIndexPtr)->c_str(), "$") == 0) {
        //do nothing
    }
    else {
        *syntacticValidityPtr = false;
    }
}

// NOLINTNEXTLINE(misc-no-recursion)
void primaryTail(list<string> &tokenList, list<string>::iterator* tokenIndexPtr, bool* syntacticValidityPtr) {

    if (strcmp((*tokenIndexPtr)->c_str(), "DOT") == 0) {

        match(tokenIndexPtr, "DOT", syntacticValidityPtr);
        match(tokenIndexPtr, "ID", syntacticValidityPtr);
        primaryTail(tokenList, tokenIndexPtr, syntacticValidityPtr);
    }
    else if(strcmp((*tokenIndexPtr)->c_str(), "LBR") == 0) {

        match(tokenIndexPtr, "LBR", syntacticValidityPtr);
        stmt(tokenList, tokenIndexPtr, syntacticValidityPtr);
        match(tokenIndexPtr, "RBR", syntacticValidityPtr);
        primaryTail(tokenList, tokenIndexPtr, syntacticValidityPtr);
    }
    else if(strcmp((*tokenIndexPtr)->c_str(), "$") == 0 ||
    strcmp((*tokenIndexPtr)->c_str(), "STRLIT") == 0 ||
    strcmp((*tokenIndexPtr)->c_str(), "ID") == 0 ||
    strcmp((*tokenIndexPtr)->c_str(), "RBR") == 0) {
        //do nothing
    }
    else {
        *syntacticValidityPtr = false;
    }
}


void match(list<string>::iterator* tokenIndexPtr, const char* type, bool* syntacticValidityPtr) {// Use pointers to const when referring to string literals

    if (strcmp((*tokenIndexPtr)->c_str(), "$") == 0) {
        *syntacticValidityPtr = false;
        //do not plus tokenIndex
    }
    else {
        if (strcmp((*tokenIndexPtr)->c_str(), type) != 0) {
            *syntacticValidityPtr = false;
            (*tokenIndexPtr)++;
        }
        else {
            (*tokenIndexPtr)++;
        }
    }
}


string get(list<string> &myList, size_t listIndex) {
    list<string>::iterator it = myList.begin();
    for (size_t i = 0; i < listIndex; i++) {
        it++;
    }
    return *it;
}

