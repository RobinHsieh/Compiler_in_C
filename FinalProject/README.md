# Compiler final project

This project involves implementing an interpreter for variations of Scheme, a dialect of Lisp. However, the grammar of this language deviates slightly, and the functions implemented represent a subset of Scheme.

## Getting Started
You can compile it by two ways.
### 1. Shell script
```bash
bison -d -v -o mini_LISP.tab.c mini_LISP.y
gcc -c -g -I.. mini_LISP.tab.c
flex -o mini_LISP.yy.c mini_LISP.l
gcc -c -g -I.. mini_LISP.yy.c
gcc -c constructAST.c
gcc -o mini_LISP mini_LISP.tab.o mini_LISP.yy.o constructAST.o -ll
./mini_LISP < input.scm
```
### 2. CMake
```CMake
cmake_minimum_required(VERSION 3.23)
project(final_project C)

set(CMAKE_C_STANDARD 99)

# Find required packages
find_package(BISON)
find_package(FLEX)

# Generate parser and lexer files
BISON_TARGET(
        MyParser mini_LISP.y ${CMAKE_CURRENT_BINARY_DIR}/mini_LISP.tab.c
        VERBOSE mini_LISP.y.output
)
FLEX_TARGET(
        MyScanner mini_LISP.l  ${CMAKE_CURRENT_BINARY_DIR}/mini_LISP.yy.c
)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

# Include directories
include_directories(${CMAKE_CURRENT_BINARY_DIR} .)

# Compile the generated and other source files
add_executable(
        mini_LISP
        ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyScanner_OUTPUTS}
        constructAST.c
)

# Link against the 'l' library
target_link_libraries(mini_LISP l)
```

## Status of Feature Implementation

### Basic Features

| No. | ~~~~~ Feature ~~~~~ | ~~~~~~~~~~~~~~~~~ Description ~~~~~~~~~~~~~~~~~~ | Points | Test Case |
|-----|---------------------|--------------------------------------------------|--------|-----------|
| 1.  | Syntax Validation   | Print “syntax error” when parsing invalid syntax | 10     |:o:        |
| 2.  | Print               | Implement print-num statement                    | 10     |:o:        |
| 3.  | Numerical Operations| Implement all numerical operations               | 25     |:o:        |
| 4.  | Logical Operations  | Implement all logical operations                 | 25     |:o:        |
| 5.  | if Expression       | Implement if expression                          | 8      |:o:        |
| 6.  | Variable Definition | Able to define a variable                        | 8      |:o:        |
| 7.  | Function            | Able to declare and call an anonymous function   | 8      |:o:        |
| 8.  | Named Function      | Able to declare and call a named function        | 6      |:o:        |

### Bonus Features

| No. | ~~~~~ Feature ~~~~~ | ~~~~~~~~~~~~~~~~~ Description ~~~~~~~~~~~~~~~~~~ | Points | Test Case |
|-----|---------------------|--------------------------------------------------|--------|-----------|
| 1.  | Recursion           | Support recursive function call                  | 5      |:o:        |
| 2.  | Type Checking       | Print error messages for type errors             | 5      |:lock:     |
| 3.  | Nested Function     | Nested function (static scope)                   | 5      |:lock:     |
| 4.  | First-class Function| Able to pass functions, support closure          | 5      |:lock:     |
