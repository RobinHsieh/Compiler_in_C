cmake_minimum_required(VERSION 3.23)
project(compiler_HW3 C)

set(CMAKE_C_STANDARD 99)

# Find required packages
find_package(BISON)
find_package(FLEX)

# Generate parser and lexer files
BISON_TARGET(
        MyParser matrix_calculator.y ${CMAKE_CURRENT_BINARY_DIR}/matrix_calculator.tab.c
        VERBOSE matrix_calculator.y.output
)
FLEX_TARGET(
        MyScanner matrix_calculator.l  ${CMAKE_CURRENT_BINARY_DIR}/matrix_calculator.yy.c
)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

# Include directories
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Compile the generated and other source files
add_executable(
        matrix_calculator
        ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyScanner_OUTPUTS}
)

# Link against the 'l' library
target_link_libraries(matrix_calculator l)
