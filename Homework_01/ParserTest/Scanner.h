#ifndef COMPILER_V2_SCANNER_H
#define COMPILER_V2_SCANNER_H

#if !defined(SIZE)
#define SIZE 1001
#endif

#include <cstring>
#include <cstddef>


//prototypes, aka declaration
struct token {
    char type[20];
    char value[20];
};

token scanner(char* localStreams, size_t* indexPtr);
token scanID(char* localStreams, size_t* indexPtr);
token scanStringLiteral(char* localStreams, size_t* indexPtr);


#endif //COMPILER_V2_SCANNER_H
