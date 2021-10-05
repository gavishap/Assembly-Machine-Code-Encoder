#include <stdio.h>
typedef struct Symbol
{
	char name[50];
	int ic;
	char type[50];
}Symbol;

int symbolIC(Symbol** symbolsArray, char* symbolName, int symbols_count);
char* symbolType(Symbol** symbolsArray, char* symbolName, int symbols_count);

