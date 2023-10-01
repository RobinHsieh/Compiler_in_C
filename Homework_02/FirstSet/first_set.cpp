#include <iostream>
#include <vector>
#include <sstream>
//#define DEBUG
#define INPUT_NUM 20
#define PRODUCT_MAX_SIZE 10
using namespace std;

// self-defined type declaration ---------------------------------
typedef struct {
    char symbol;
    bool visitFirst;
    bool derivesEmpty;
} Non_Terminal;

// variable definition -------------------------------------------
string inputArray[INPUT_NUM];
string input;
string productArray[PRODUCT_MAX_SIZE][PRODUCT_MAX_SIZE];
vector<Non_Terminal> nonTerminalList;
bool addLambdaInFirstSet;

// function declaration, aka prototype ---------------------------
vector<string> split(const string &str, const char &delimiter);
void saveToProductArray(string inputArray_local[], int numbersOfGrammar);
bool visitedFirst(char thisNonTerminal);
void changeSymbolDerivesEmpty(char thisNonTerminal, bool status);
bool symbolDerivesEmpty(char thisNonTerminal);
void initialVisit();
vector<char> first(char lhs, string &alpha);
void internalFirst(char lhs, string &alpha, vector<char> &result, size_t Xn, size_t xn);


int main() {
    // standard input
    int j = 0;
    while(cin >> input) {
        if (input == "END_OF_GRAMMAR") {
            break;
        }
        inputArray[j] = input;
        j++;
    }
    // j/2 means how many rules of grammar
    j /= 2;

    saveToProductArray(inputArray, j);


#if defined(DEBUG)
    // check productArray
    for (int row = 0; row < 10; row++) {
        for (int column = 0; column < 10; column++) {
            cout << productArray[row][column] << " ";
        }
        cout << endl;
    }
    cout << "------------------------------" << endl;
#endif

#if defined(DEBUG)
    // check productArray
    for (int row = 0; row < 10; row++) {
        for (int column = 0; column < 10; column++) {
            int b = productArray[row][column].compare("");
            cout << b << " ";
        }
        cout << endl;
    }
    cout << "------------------------------" << endl;
#endif

    // create the non-terminal list
    for (size_t p_row = 0; p_row < PRODUCT_MAX_SIZE; p_row++) {
        if (!productArray[p_row][0].empty()) {
            Non_Terminal non_terminal = {};
            non_terminal.symbol = productArray[p_row][0].at(0);
            // initial them as being visited first
            non_terminal.visitFirst = true;
            // initial them as won't derive empty
            non_terminal.derivesEmpty = false;
            nonTerminalList.push_back(non_terminal);
        }
    }

#if defined(DEBUG)
    // check non-terminalList
    for (int i = 0; i < nonTerminalList.size(); i++) {
        cout << nonTerminalList[i].symbol << endl;
    }
    cout << "------------------------------" << endl;
#endif

    for (size_t p_row = 0; p_row < PRODUCT_MAX_SIZE; p_row++) {
        if (!productArray[p_row][0].empty()) {

            cout << productArray[p_row][0] << " ";

            vector<char> firstSet;

            initialVisit();

            /* productArray[p_row][0] is LHS in a grammar
             * productArray[p_row][1] is the first choose RHS in a grammar */
            int p_column = 1;

            // while break when finish an iteration of a set of choices of RHS in a grammar
            while (!productArray[p_row][p_column].empty()) {
                vector<char> temp = first(productArray[p_row][0].at(0) ,productArray[p_row][p_column]);
                firstSet.insert(firstSet.end(), temp.begin(), temp.end());
                p_column++;
            }

            // check the LHS of grammar derive empty or not
            if (symbolDerivesEmpty(productArray[p_row][0].at(0))) {
                firstSet.push_back(';');
            }

            sort(firstSet.begin(), firstSet.end());

            for (size_t i = 0; i < firstSet.size(); ++i) {
                cout << firstSet[i];
            }
            cout << endl;
        }
    }
    cout << "END_OF_FIRST" << endl;

    return 0;
}



vector<string> split(const string &str, const char &delimiter) {
    vector<string> result;
    stringstream ss(str);
    string tok;
    while (getline(ss, tok, delimiter)) {
        result.push_back(tok);
    }
    return result;
}


void saveToProductArray(string inputArray_local[], int numbersOfGrammar) {
    // save LHS from inputArray to productArray
    for (int p_row = 0, i_index = 0; p_row < numbersOfGrammar; p_row++, i_index += 2) {
        productArray[p_row][0] = inputArray_local[i_index];
    }

    // save RHS from inputArray to productArray
    size_t p_column;
    for (int p_row = 0, i_index = 1; p_row <= numbersOfGrammar; p_row++, i_index += 2) { // productArray's p_row, inputArray_local's i_index
        p_column = 1;
        vector<string> tmp = split(inputArray_local[i_index], '|');
        for (int i = 0; i < tmp.size(); i++) {
            productArray[p_row][p_column] = tmp[i];
            p_column++;
        }
    }

    // bubble sort for LHS of productArray
    bool swap;
    string tmp;
    for (int p_row_i = PRODUCT_MAX_SIZE - 1; p_row_i >= 1; p_row_i--) {
        swap = false;
        for (int p_row_j = 0; p_row_j <= p_row_i - 1; p_row_j++) {
            // compare ASCII code of LHS(must be a non-terminal) in the first column of productArray
            if (*(productArray[p_row_j][0].c_str()) > *(productArray[p_row_j + 1][0].c_str())) {
                swap = true;
                for (int p_column_2 = 0; p_column_2 < PRODUCT_MAX_SIZE; p_column_2++) {
                    tmp = productArray[p_row_j][p_column_2];
                    productArray[p_row_j][p_column_2] = productArray[p_row_j + 1][p_column_2];
                    productArray[p_row_j + 1][p_column_2] = tmp;
                }
            }
        }
        if (!swap) {
            break;
        }
    }
}


bool visitedFirst(char thisNonTerminal) {
    for (size_t i = 0; i < nonTerminalList.size(); ++i) {
        if (nonTerminalList[i].symbol == thisNonTerminal) {
            if (nonTerminalList[i].visitFirst) {
                nonTerminalList[i].visitFirst = false;
                return true;
            }
            else {
                return nonTerminalList[i].visitFirst;
            }
        }
    }
    return false;
}


void changeSymbolDerivesEmpty(char thisNonTerminal, bool status) {
    for (size_t i = 0; i < nonTerminalList.size(); ++i) {
        if (nonTerminalList[i].symbol == thisNonTerminal) {
            nonTerminalList[i].derivesEmpty = status;
            break;
        }
    }
}


bool symbolDerivesEmpty(char thisNonTerminal) {
    if (thisNonTerminal == ';') {
        return true;
    }
    for (size_t i = 0; i < nonTerminalList.size(); ++i) {
        if (nonTerminalList[i].symbol == thisNonTerminal) {
            return nonTerminalList[i].derivesEmpty;
        }
    }
    return false;
}


void initialVisit() {
    for (size_t i = 0; i < nonTerminalList.size(); i++) {
        nonTerminalList[i].visitFirst = true;
    }
}


vector<char> first(char lhs, string &alpha) {
    vector<char> result;

    // how many X do it have in RHS in a grammar Y -> X0 X1 X2 ... Xn
    size_t Xn = alpha.length();

    // check the first set of first(alpha), the X numbers is Xn, now we are checking X0 first
    internalFirst(lhs, alpha, result, Xn, 0);
    return result;
}


void internalFirst(char lhs, string &alpha, vector<char> &result, size_t Xn, size_t xn) { //Xn is the numbers of RHS: X0 X1 X2 ... Xn, xn is the count

    if (xn < Xn) {

        // RHS meets lambda, which is represented by ';'
        if (alpha[xn] == ';') {

            // rule 1 for judge whether Y of grammar: Y -> X0 X1 ... Xn will derives empty
            changeSymbolDerivesEmpty(lhs, true);
            return;
        }
        // RHS meets terminal symbol
        else if ((97 <= alpha[xn] && alpha[xn] <= 122) || alpha[xn] == 36) {

            result.push_back(alpha[xn]);
            return;
        }
        // RHS meets non-terminal symbol
        else if (65 <= alpha[xn] && alpha[xn] <= 90) {

            if (visitedFirst(alpha[xn])) {

                for (size_t p_row = 0; p_row < PRODUCT_MAX_SIZE; p_row++) {

                    // find the grammar in productArray that LHS == alpha[xn]
                    if (!productArray[p_row][0].empty()) {
                        if (productArray[p_row][0].at(0) == alpha[xn]) {

                            size_t p_column = 1;
                            while (!productArray[p_row][p_column].empty()) {

                                internalFirst(productArray[p_row][0].at(0), productArray[p_row][p_column], result, productArray[p_row][p_column].length(), 0);
                                p_column++;
                            }
                            break;
                        }
                    }
                }
            }
            if (symbolDerivesEmpty(alpha[xn])) {
                internalFirst(lhs, alpha, result, Xn, xn + 1);
            }
        }
    }
    // rule 2 for judge whether Y of grammar: Y -> X0 X1 ... Xn will derives empty
    else if (xn == Xn) {
        addLambdaInFirstSet = true;
        for (size_t i = 0; i < alpha.length(); i++) {
            if (!symbolDerivesEmpty(alpha[i])) {
                addLambdaInFirstSet = false;
            }
        }
        if (addLambdaInFirstSet) {
            changeSymbolDerivesEmpty(lhs, true);
        }
    }
}