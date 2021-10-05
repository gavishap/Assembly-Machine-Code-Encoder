#include "symbol.h"


int symbolIC(Symbol** symbolsArray, char* symbolName,int symbols_count)
{
	int i;
	for ( i = 0; i < symbols_count; i++) {
		if (strcmp((*symbolsArray)[i].name, symbolName) == 0)
		{
			return (*symbolsArray)[i].ic;
		}
	}
}

char* symbolType(Symbol** symbolsArray, char* symbolName, int symbols_count)
{
	int i;
	for ( i = 0; i < symbols_count; i++) {
		if (strcmp((*symbolsArray)[i].name, symbolName) == 0)
		{
			return (*symbolsArray)[i].type;
		}
	}
	return "";
}
