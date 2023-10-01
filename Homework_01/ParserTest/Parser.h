#ifndef COMPILER_V2_PARSER_H
#define COMPILER_V2_PARSER_H

#include <iostream>
#include <list>
#include <cstring>

using namespace std;


//prototypes, aka declaration
bool parser(list<string> &tokenList);
void stmts(list<string> &tokenList, list<string>::iterator* tokenIndexPtr, bool* syntacticValidityPtr);
void stmt(list<string> &tokenList, list<string>::iterator* tokenIndexPtr, bool* syntacticValidityPtr);
void primary(list<string> &tokenList, list<string>::iterator* tokenIndexPtr, bool* syntacticValidityPtr);
void primaryTail(list<string> &tokenList, list<string>::iterator* tokenIndexPtr, bool* syntacticValidityPtr);
void match(list<string>::iterator* tokenIndexPtr, const char* type, bool* syntacticValidityPtr);


#endif //COMPILER_V2_PARSER_H
