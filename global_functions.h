#include "symbol.h"

void addSymbolToExternTable(Symbol** symbolsArray, int* symbols_count, char* symbol_name, char* type, int ic);
void addSymbolToSymbolTable(Symbol** symbolsArray, int* symbols_count, char* symbol_name, char* type, int ic);
char** splitLineBySpace(char* line, int* countOfWords);
char** splitLineBySpaceAndComma(char* line, int* countOfWords);
