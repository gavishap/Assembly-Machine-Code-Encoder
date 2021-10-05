#include "second_pass.h"
#include "linked_list.h"
#include "testing.h" 

void fillZeroWord(node* currentNode, Symbol** symbolsArray, char* wordsArray, int symbols_count, Symbol** externSymbolsArray, int* externSymbolsAmount, int ic)
{
	currentNode->mc.word = (symbolIC(symbolsArray, wordsArray, symbols_count) << 3);
	if (strcmp(symbolType(symbolsArray, wordsArray, symbols_count), "external") == 0) {
		currentNode->mc.fields.E = 1;
		addSymbolToExternTable(externSymbolsArray, externSymbolsAmount, wordsArray, "external", ic);
	}
	else /*if not external then has to be relocatable*/
		currentNode->mc.fields.R = 1;
}

int second_pass(FILE* file, Symbol** symbolsArray, Symbol** externSymbolsArray, int symbols_count, int* externSymbolsAmount)
{
	int ic = 100, dc = 0, label_line = 1, i = 0, has_error = 0;
	char line[50];
	fseek(file, 0, SEEK_SET);
	int k = 0;
	while (fgets(line, 50, file))
	{
		int is_symbol = 0;
		/*int symbols_count = first_pass(file, &symbolsArray);*/
		if (line == NULL || line[0] == '\n' || line[0] == ';')
			continue;
		int wordsArraySize = 0;
		char** wordsArray = splitLineBySpaceAndComma(line, &wordsArraySize);
		if (wordsArraySize == 0)
			continue;
		if (wordsArray[0][strlen(wordsArray[0]) - 1] == ':')
			is_symbol = 1;
		if (strcmp(wordsArray[0 + is_symbol], ".extern") == 0 || strcmp(wordsArray[0 + is_symbol], ".string") == 0 || strcmp(wordsArray[0 + is_symbol], ".data") == 0) {
			ic = ic + command_lengths[k];
			k++;
			continue;
		}
		if (strcmp(wordsArray[0], ".entry") == 0)/*adding ",entry" to the symbol type*/
		{
			char* symbol = wordsArray[1];
			int symbol_in_table = 0;
			for (i = 0; i < symbols_count; i++)
			{
				if (strcmp((*symbolsArray)[i].name, symbol) == 0)
				{
					strcat((*symbolsArray)[i].type, ", entry");
					symbol_in_table = 1;
				}
			}
			if (symbol_in_table == 0)
				printf("The .entry label %s is not in the lable table.\n", symbol);
			k++;
			continue;
		}
		if (cmd_err_first_pass(ic, wordsArray, is_symbol, wordsArraySize, symbolsArray, &symbols_count, line, 0) == 0)
		    has_error = 1;
		node* currentNode, * commandNode, * firstExtraNode;
		int j = 0;
		while (j < command_lengths[k])
		{
			currentNode = find(ic);
			if (currentNode == NULL)
			{
				printf("Error: unexist ic\n");
				return 0;
			}
			if (currentNode->mc.word == 0)/*if there's an extra word*/
			{
				if (command_lengths[k] == 3 && j == 1 && find(ic - 1)->mc.fields.add_des == 1 && find(ic - 1)->mc.fields.add_src == 1)/*if theres 3 words with 2 labels for the command and the last ic is the command, jump to the next zero word*/
				{
					j++;
					ic++;
					continue;
				}
				commandNode = find(ic - 1);
				firstExtraNode = currentNode;
				if (j == 2) {
					firstExtraNode = commandNode;
					commandNode = find(ic - 2);
				}
				if (commandNode->mc.fields.opcode >= 0 && commandNode->mc.fields.opcode <= 4)/*command with 2 operands*/
				{
					if (commandNode->mc.fields.add_src == 1 && commandNode->mc.fields.add_des != 1) /*if first operand has a label and second doesnt*/
						fillZeroWord(currentNode, symbolsArray, wordsArray[1 + is_symbol], symbols_count, externSymbolsArray, externSymbolsAmount, ic);

					if (commandNode->mc.fields.add_des == 1 && commandNode->mc.fields.add_src != 1) /*if second operand has a label and first doesnt*/
						fillZeroWord(currentNode, symbolsArray, wordsArray[2 + is_symbol], symbols_count, externSymbolsArray, externSymbolsAmount, ic);

					if (commandNode->mc.fields.add_src == 1 && commandNode->mc.fields.add_des == 1)/*if first and second operands are labels*/
					{
						fillZeroWord(firstExtraNode, symbolsArray, wordsArray[1 + is_symbol], symbols_count, externSymbolsArray, externSymbolsAmount, ic - 1);
						fillZeroWord(currentNode, symbolsArray, wordsArray[2 + is_symbol], symbols_count, externSymbolsArray, externSymbolsAmount, ic);
					}
				}

				else if (commandNode->mc.fields.opcode == 5 || commandNode->mc.fields.opcode == 12 || commandNode->mc.fields.opcode == 13 && commandNode->mc.fields.add_des == 1)
					fillZeroWord(currentNode, symbolsArray, wordsArray[1 + is_symbol], symbols_count, externSymbolsArray, externSymbolsAmount, ic);

				else if (commandNode->mc.fields.opcode == 9 && commandNode->mc.fields.add_des == 2) {
					currentNode->mc.word = ((symbolIC(symbolsArray, wordsArray[1 + is_symbol] + 1, symbols_count) - (commandNode->ic)) << 3);
					currentNode->mc.fields.A = 1;
				}

				else if (commandNode->mc.fields.opcode == 9 && commandNode->mc.fields.add_des == 1)
					fillZeroWord(currentNode, symbolsArray, wordsArray[1 + is_symbol], symbols_count, externSymbolsArray, externSymbolsAmount, ic);

			}
			ic++;
			j++;
		}
		k++;
	}

	return has_error;
}

