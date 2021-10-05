#include "global_functions.h"
#include <string.h>
#include <stdlib.h>

void addSymbolToExternTable(Symbol** symbolsArray, int* symbols_count, char* symbol_name, char* type, int ic)
{
	(*symbols_count)++;
	*symbolsArray = (Symbol*)realloc(*symbolsArray, sizeof(Symbol) * (*symbols_count));
	strcpy((*symbolsArray)[*symbols_count - 1].name, symbol_name);
	(*symbolsArray)[*symbols_count - 1].ic = ic;
	strcpy((*symbolsArray)[*symbols_count - 1].type, type);
}
void addSymbolToSymbolTable(Symbol** symbolsArray, int* symbols_count, char* symbol_name, char* type, int ic)
{
	int symbol_in_table = 0, i;
	if ((*symbols_count) != 0) {
		for (i = 0; i < *symbols_count; i++)
		{
			if (strcmp((*symbolsArray)[i].name, symbol_name) == 0)
			{
				printf("The label name %s already exists in the label chart.\n", symbol_name);
				symbol_in_table = 1;
			}
		}
	}
	if (symbol_in_table == 1)
		return;
	else {
		(*symbols_count)++;
		*symbolsArray = (Symbol*)realloc(*symbolsArray, sizeof(Symbol) * (*symbols_count));
		strcpy((*symbolsArray)[*symbols_count - 1].name, symbol_name);
		(*symbolsArray)[*symbols_count - 1].ic = ic;
		strcpy((*symbolsArray)[*symbols_count - 1].type, type);
		return;
	}
}



/*function to split a row into seperate words by space*/
char** splitLineBySpace(char* line, int* countOfWords)
{
	char* tempLine = strdup(line);
	*countOfWords = 0;
	char** wordsArray = NULL; /*array of strings to hold the words*/
	char* temp = strtok(tempLine, " \n\t"); /*save first word*/
	if (temp != NULL) {
		wordsArray = (char**)malloc(sizeof(char*));
		(wordsArray)[0] = temp;
		(*countOfWords)++;
	}

	while (temp = strtok(NULL, " \n\t\r"))
	{
		(*countOfWords)++;
		wordsArray = (char**)realloc(wordsArray, sizeof(char*) * (*countOfWords));
		wordsArray[(*countOfWords) - 1] = temp;
	}
	return wordsArray;
}

/*function to split a row into seperate words by space and comma*/
char** splitLineBySpaceAndComma(char* line, int* countOfWords)
{
	char* tempLine = strdup(line);
	*countOfWords = 0;
	char** wordsArray = NULL; /*array of strings to hold the words*/
	char* temp = strtok(tempLine, ", \n\t"); /*save first word*/
	if (temp != NULL) {
		wordsArray = (char**)malloc(sizeof(char*));
		(wordsArray)[0] = temp;
		(*countOfWords)++;
	}

	while (temp = strtok(NULL, ", \n\t\r"))
	{
		(*countOfWords)++;
		wordsArray = (char**)realloc(wordsArray, sizeof(char*) * (*countOfWords));
		wordsArray[(*countOfWords) - 1] = temp;
	}
	return wordsArray;
}
