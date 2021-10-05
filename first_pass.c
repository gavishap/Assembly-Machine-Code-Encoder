/*fisrt pass through the code lines to save the ic and to evaluate what type of line it is (not finished)*/
#include "first_pass.h"
#include "command_testing.h"
#include "testing.h"



void handleCommand(command* cmd, char** wordsArray, int wordsArraySize, int* ic, int prefix_label, int only_des) {
	MachineCode word;
	word.word = 0;
	int extra_words;
	extra_words = 0;
	word.fields.opcode = cmd->opcode;
	word.fields.funct = cmd->funct_id;

	/*if there is 2 operands with a label*/
	if (wordsArraySize == 4) {
		address_type(cmd->name, wordsArray[1 + prefix_label], wordsArray[2 + prefix_label], &word);
		bit_reg(&word, wordsArray[1 + prefix_label], wordsArray[2 + prefix_label]);
	}
	/*if there is 1 operand plus a label or 2 operands without a label*/
	else if (wordsArraySize == 3) {
		/*2 operands without a label */
		if (prefix_label == 0) {
			address_type(cmd->name, wordsArray[1 + prefix_label], wordsArray[2 + prefix_label], &word);
			bit_reg(&word, wordsArray[1 + prefix_label], wordsArray[2 + prefix_label]);
		}
		/*if there is 1 operand plus a label*/
		else {
			address_type(cmd->name, NULL, wordsArray[1 + prefix_label], &word);
			bit_reg(&word, NULL, wordsArray[1 + prefix_label]);
		}
	}
	else if (wordsArraySize == 2) {
		/*if there is 1 operand without a label*/
		if (prefix_label == 0) {
			address_type(cmd->name, NULL, wordsArray[1 + prefix_label], &word);
			bit_reg(&word, NULL, wordsArray[1 + prefix_label]);
		}
		/*zero operands with a label*/
		else {
			address_type(cmd->name, NULL, NULL, &word);
			bit_reg(&word, NULL, NULL);
		}
	}
	/*if there is 0 operands without a label*/
	else if (wordsArraySize == 1) {
		address_type(cmd->name, NULL, NULL, &word);
		bit_reg(&word, NULL, NULL);
	}

	insertFirst((*ic)++, word);

	/*checking how many extra words we need*/

	if (word.fields.add_src < 3 && only_des == 0)
	{
		char* temp;
		int num = strtod(wordsArray[1 + prefix_label] + 1, &temp);
		MachineCode extra_num;
		extra_words++;
		if (word.fields.add_src == 0)
			extra_num.word = (num  << 3) + 4;
		else /* add_src = 1 or 2 */
			extra_num.word = 0;
		insertFirst((*ic)++, extra_num);
	}
	if (word.fields.add_des < 3 && only_des == 0)
	{
		char* temp;
		int num = strtod(wordsArray[2 + prefix_label] + 1, &temp);
		MachineCode extra_num;
		extra_words++;
		if (word.fields.add_des == 0)
			extra_num.word = (num << 3) + 4;
		else /* add_des = 1 or 2 */
			extra_num.word = 0;
		insertFirst((*ic)++, extra_num);
	}
	else if (word.fields.add_des < 3 && only_des == 1)
	{
		char* temp;
		int num = strtod(wordsArray[1 + prefix_label] + 1, &temp);
		MachineCode extra_num;
		extra_words++;
		if (word.fields.add_des == 0)
			extra_num.word = (num << 3) + 4;
		else /* add_src = 1 or 2 */
			extra_num.word = 0;
		insertFirst((*ic)++, extra_num);
	}
}


int first_pass(FILE* file, Symbol** symbolsArray, int* symbols_count)
{
	int ic = 100, dc = 0, label_line = 1, has_error = 0;
	char line[50];
	char* symbol;
	int i = 0;
	commands_dictionary dictionary = build_dictionary();  /*importing the dictionary we made to compare the command names to*/
	
	while (fgets(line, 50, file))
	{
		int  label_command = 0, only_des = 0; /*flags for different commands we find, only_des is a flag if there is only one operand*/
		int begin_ic = ic;

		if (line == NULL || line[0] == '\n' || line[0] == ';')
		{
			continue;
		}
				
		int wordsArraySize;
		wordsArraySize = 0;
		char** wordsArray = splitLineBySpaceAndComma(line, &wordsArraySize);
		if (wordsArraySize == 0)
			continue;

		/*if the first word has a parenthesis then its a label*/
		if (wordsArray[0][strlen(wordsArray[0]) - 1] == ':')
		{
			/* Label */
			
			symbol = strtok(wordsArray[0], ":");

			/*if there's a label with a command after it*/
			if (wordsArray[1][0] != '.')
			{
				label_command = 1;
				if (wordsArraySize == 3)
					only_des = 1;/*the first operand should be treated as a destination operand*/
				if (wordsArraySize == 2)
					only_des = 2;/*there is no operands in this case*/
			}

			/*after knowing that we have a label without command, we check if its .data or .string*/
			if (label_command == 0)
			{
				int i;
				for ( i = 0; i < strlen(line) - 1; i++)
				{
					if (isspace(line[i]) || line[i] != '.')
						continue;
					/*.string*/
					else if (strncmp(line + i, ".string", 7) == 0)
					{
						

						/* check string legal */
						if (check_guidance_sentence(line, 1, 0, 1, 0, 0, ic) == 0)
						    has_error = 1;

						/*adding a label to the label table*/
						addSymbolToSymbolTable(symbolsArray, symbols_count, symbol, "data", ic);
						char* label_string_begin = strchr(line, '"');
						char* label_string_end = strrchr(line, '"');
						char* tav;
						for (tav = label_string_begin + 1; tav < label_string_end; tav++)
						{
							/* conevrt decimal to binary */
							MachineCode m = decimal_2_binary(*tav);
							insertFirst(ic, m);
							ic++;
							dc++;
						}
						/* add '\0' */
						MachineCode m;
						m.word = 0;
						insertFirst(ic, m);
						ic++;
						dc++;

					}
					/*.data*/
					else if (strncmp(line + i, ".data", 5) == 0)
					{
						int p;
						

						/* check data legal */
						if (check_guidance_sentence(line, 1, 1, 0, 0, 0, ic) == 0)
						    has_error = 1;

						/*adding a label to the label table*/
						addSymbolToSymbolTable(symbolsArray, symbols_count, symbol, "data", ic);
						wordsArray = splitLineBySpaceAndComma(line, &wordsArraySize);
						for ( p = 2; p < wordsArraySize; p++)
						{
							int num = atoi(wordsArray[p]);
							/* convert decimal to binary */
							MachineCode m = decimal_2_binary(num);
							insertFirst(ic, m);
							ic++;
							dc++;
						}

					}
				}
			}
			/*command inside a label*/
			else
			{
				/* Search command */
				command* cmd = findCommand(dictionary, wordsArray[1]);
				if (cmd == NULL)
				{
					/* handle error */
					/*printf("Command %s not exists\n", wordsArray[1]);*/
				}
				else
				{
					if (cmd_err_first_pass(ic, wordsArray, 1, wordsArraySize, symbolsArray, symbols_count,line,1) == 0)
					    has_error = 1;
					addSymbolToSymbolTable(symbolsArray, symbols_count, symbol, "code", ic);
					handleCommand(cmd, wordsArray, wordsArraySize, &ic, 1, only_des);
				}
			}
			if (strcmp(wordsArray[1], ".extern") == 0)
			{
				/* Extern */
				
				symbol = wordsArray[2];

				/* check extern legal */
				if (check_guidance_sentence(line, 1, 0, 0, 0, 1, ic) == 0)
				    has_error = 1;

				/*adding a label to the label table*/
				addSymbolToSymbolTable(symbolsArray, symbols_count, symbol, "external", 0);
				only_des = 0;
			}
			else if (strcmp(wordsArray[1], ".entry") == 0)
			{
				/* Entry */
				
				only_des = 0;

				/* check entry legal */
				if (check_guidance_sentence(line, 1, 0, 0, 1, 0, ic) == 0)
				    has_error = 1;
			}
		}
		/*when there is no label*/
		else {
			if (wordsArraySize == 2)
				only_des = 1;
			if (strcmp(wordsArray[0], ".extern") == 0)
			{
				/* Extern */
				
				symbol = wordsArray[1];

				/* check extern legal */
				if (check_guidance_sentence(line, 0, 0, 0, 0, 1, ic) == 0)
				    has_error = 1;

				/*adding a label to the label table*/
				addSymbolToSymbolTable(symbolsArray, symbols_count, symbol, "external", 0);
				only_des = 0;
			}
			else if (strcmp(wordsArray[0], ".entry") == 0)
			{
				/* Entry */
				
				only_des = 0;

				/* check entry legal */
				if (check_guidance_sentence(line, 0, 0, 0, 1, 0, ic) == 0)
				    has_error = 1;
			}
			else if (strcmp(wordsArray[0], ".string") == 0)
			{
				/* String if not in a label*/
				
				only_des = 0;

				/* check string legal */
				if (check_guidance_sentence(line, 0, 0, 1, 0, 0, ic) == 0)
				    has_error = 1;

				char* label_string_begin = strchr(line,'"');
				char* label_string_end = strrchr(line, '"');
				char* tav;
				for ( tav = label_string_begin+1; tav < label_string_end; tav++)
				{
					/* conevrt decimal to binary */
					MachineCode m = decimal_2_binary(*tav);
					insertFirst(ic, m);
					ic++;
					dc++;
				}
				/* add '\0' */
				MachineCode m;
				m.word = 0;
				insertFirst(ic, m);
				ic++;
				dc++;
			}
			else if (strcmp(wordsArray[0], ".data") == 0)
			{
				int p;
				/* Data if not in a label*/
				
				only_des = 0;

				/* check data legal */
				if (check_guidance_sentence(line, 0, 1, 0, 0, 0, ic) == 0)
                    has_error = 1;
				wordsArray = splitLineBySpaceAndComma(line, &wordsArraySize);
				for ( p = 1; p < wordsArraySize; p++)
				{
					int num = atoi(wordsArray[p]);
					/* convert decimal to binary */
					MachineCode m = decimal_2_binary(num);
					insertFirst(ic, m);
					ic++;
					dc++;
				}
			}
			/*command that is not inside a label\symbol*/
			else
			{
				/* Search command */
				command* cmd = findCommand(dictionary, wordsArray[0]);
				if (cmd == NULL)
				{
					/* handle error */
					/*printf("Command %s not exists\n", wordsArray[0]);*/
				}
				else
				{
					if (cmd_err_first_pass(ic, wordsArray, 0, wordsArraySize, symbolsArray, symbols_count, line,1) == 0)
					    has_error = 1;
					handleCommand(cmd, wordsArray, wordsArraySize, &ic, 0, only_des);
				}
			}
		}
		int diff_ic = ic - begin_ic;
		command_lengths[i] = diff_ic;
		i++;
	}
	global_dcf = dc;
	global_icf = ic - 100 - dc;
	return has_error;
}


void resetCommandLengths()
{
	int i;
	for (i = 0; i< 100; i++)
	{
		command_lengths[i] = 0;
	}
}
