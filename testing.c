#include "funct_dictionary.h"
#include "global_functions.h"
#include "testing.h"

int check_guidance_sentence(char* line, int is_symbol,int data, int string, int entry, int external, int ic) /*checking validity of a guidance sentence*/
{
	if (entry == 1 || external == 1)
	{
		int arrayCount = 0;
		char** wordsArray = splitLineBySpace(line, &arrayCount);
		if (arrayCount > 2 + is_symbol)
		{
			error_print(TOO_MANY_OPS_IN_GUIDANCE,ic);
			return 0;
		}
		else if (arrayCount < 2 + is_symbol)
		{
			error_print(SCARCE_OPS_IN_GUIDANCE,ic);
			return 0;
		}
		return 1;
	}
	
	if (string == 1)
	{
		int i = strlen(line) -1;
		while (i >= 0 && (line[i] == '\0' || isspace(line[i])))
		{
			i--;
		}
		if (line[i] != '"')
		{
			error_print(MISSING_QUOTATION_MARK, ic);
			return 0;
		}

		char* label_string_begin = strchr(line, '"');
		char* label_string_end = strrchr(line, '"');
		if (label_string_begin == NULL || label_string_end == NULL || label_string_begin == label_string_end) {
			error_print(MISSING_QUOTATION_MARK, ic);
			return 0;
		}

		return 1;
	}

	if (data == 1)
	{
		int i = strlen(line) - 1;
		while (i >= 0 && (line[i] == '\0' || isspace(line[i])))/*removing all the whitespaces or \0 from the end of the string*/
		{
			i--;
		}

		if (line[i] < '0' || line[i] > '9')/*if the last character isnt a number*/
		{
			error_print(END_DATA_WITHOUT_NUMBER, ic);
			return 0;
		}


		char* ptr = strstr(line, ".data") + 5;/*giving a pointer to the spot in the string after the ".data." */
		while (*ptr != '\0')
		{
			if (isspace(*ptr)) {
				ptr++;
				continue;
			}

			if (*ptr == '-' || *ptr == '+')
				ptr++;

			int digitsCount = 0;
			while (*ptr >= '0' && *ptr <= '9')
			{
				ptr++;
				digitsCount++;
			}

			if (digitsCount == 0)
			{
				error_print(MISSING_NUMBER, ic);
				return 0;
			}

			if (*ptr == '\0')
				break;

			while (isspace(*ptr)) {
				ptr++;
			}

			if (*ptr == '\0')
				break;

			if (*ptr != ',')
			{
				error_print(MISSING_COMMA, ic);
				return 0;
			}
			else {
				ptr++;
			}
		}

		return 1;

	}
}

int check_label_presence(char* symbol_name, Symbol** symbolsArray, int* symbols_count, int ic)/*if a label inside a command is defined(second pass)*/
{
	int symbol_in_table = 0, i;

	if ((*symbols_count) != 0) {
		for (i = 0; i < *symbols_count; i++)
		{
			if (strcmp((*symbolsArray)[i].name, symbol_name) == 0) {
                symbol_in_table = 1;
                break;
			}
		}
	}
	else {
		error_print(UNDEFINED_LABEL, ic);
		return 0;
	}
	if (symbol_in_table == 0){
		error_print(UNDEFINED_LABEL, ic);
		return 0;
	}
	else 
		return 1;
}


int grammar_check(int ic, char* line, int is_symbol, int right_operand_count)
{
	/* split by spaces*/
	int wordsArraySize, j;
	char** wordsArray = splitLineBySpace(line, &wordsArraySize);
	int first_op = 0;
	if (is_symbol)/*if theres a comma before the label*/
	{
		if (wordsArray[0][0] == ',')
		{
			error_print(ILLEGAL_COMMA, ic);
			return 0;
		}
	}

	if (wordsArray[0 + is_symbol][0] == ',')/*if a comma is chained to the beginning of the command or if theres a comma before the command*/
	{
		error_print(ILLEGAL_COMMA, ic);
		return 0;
	}

	if (wordsArray[0 + is_symbol][strlen(wordsArray[0 + is_symbol]) - 1] == ',')/*if a comma is chained to the end of the command*/
	{
		error_print(ILLEGAL_COMMA, ic);
		return 0;
	}

	if (wordsArraySize > 1 + is_symbol && right_operand_count == 0)
	{
		error_print(ILLEGAL_COMMA, ic);
		return 0;
	}
	else if (wordsArraySize == 1 + is_symbol && right_operand_count == 0)
	{
		if (wordsArray[0 + is_symbol][strlen(wordsArray[0 + is_symbol]) - 1] == ',')/*if a comma is chained to the end of the command*/
		{
			error_print(ILLEGAL_COMMA, ic);
			return 0;
		}
		return 1;
	}

	if (wordsArray[1 + is_symbol][0] == ',')/*if theres a comma between the command and first operand or chained to the beginning of first operand*/
	{
		error_print(ILLEGAL_COMMA, ic);
		return 0;
	}


	if (wordsArraySize > 2 + is_symbol && right_operand_count == 1)
	{
		error_print(ILLEGAL_COMMA, ic);
		return 0;
	}
	else if (wordsArraySize == 2 + is_symbol && right_operand_count == 1)/* In case there is more operands or in case we want check the last word in the array */
	{
		if (wordsArray[1 + is_symbol][strlen(wordsArray[1 + is_symbol]) - 1] == ',')/*if a comma is chained to the end of the first operand*/
		{
			error_print(ILLEGAL_COMMA, ic);
			return 0;
		}
		return 1;
	}

	int countOfCommas = 0;
	if (wordsArray[1 + is_symbol][strlen(wordsArray[0 + is_symbol]) - 1] == ',')/*if a comma is chained to the end of the first operand*/
	{
		countOfCommas++;
	}
	for (j = 0; j < strlen(wordsArray[2 + is_symbol]); j++)
	{
		if (wordsArray[2 + is_symbol][j] == ',')
		{
			countOfCommas++;
		}
		else
		{
			if (countOfCommas > 1)
			{
				error_print(ILLEGAL_COMMA, ic);
				return 0;
			}
			else if (countOfCommas == 0)
			{
				error_print(MISSING_COMMA, ic);
				return 0;
			}
			else
			{
				if (wordsArray[2 + is_symbol][strlen(wordsArray[0 + is_symbol]) - 1] == ',')/*if a comma is chained to the end of the second operand*/
				{
					error_print(ILLEGAL_COMMA, ic);
					return 0;
				}
				return 1;
			}
		}
	}

	if (wordsArray[3 + is_symbol][0] == ',')/*if theres a comma between the command and first operand or chained to the beginning of second operand*/
	{
		countOfCommas++;
	}


	if (wordsArray[3 + is_symbol][strlen(wordsArray[0 + is_symbol]) - 1] == ',')/*if a comma is chained to the end of the second operand*/
	{
		error_print(ILLEGAL_COMMA, ic);
		return 0;
	}

	if (countOfCommas > 1)
	{
		error_print(ILLEGAL_COMMA, ic);
		return 0;
	}
	else if (countOfCommas == 0)
	{
		error_print(MISSING_COMMA, ic);
		return 0;
	}
	else
	{
		if (wordsArraySize > 3 + is_symbol)
		{
			error_print(ILLEGAL_COMMA, ic);
			return 0;
		}
		return 1;
	}

}

/* address2 : 1 - means address method (2), 0 - address method (1)*/
int labelCheck(char* operand, int address2, int ic) /*checks for a valid label name*/
{
	commands_dictionary dictionary = build_dictionary();
	command* cmd = findCommand(dictionary, operand);
	if (cmd != NULL)
		error_print(ILLEGAL_LABEL_NAME, ic);
	/*int i;
	while (i >= 0 && (operand[i] == '\0' || isspace(operand[i])))
	{
		i--;
	}*/
	int i, right_label=1;					/*flag if the label is a correct addressing 2 method lable name*/
	if ((operand + 0 + address2) == NULL || isalpha(operand[0+address2]) == 0) {				/*if second/first charachter isnt a letter or null*/
		right_label = 0;
	}
	else  {
		
			for (i = 0+address2; i < strlen(operand) - address2; i++)		/*if rest of characters are letters or numbers*/
				if ((isalpha(operand[i]) == 0) && (isdigit(operand[i]) == 0) && (isspace(operand[i]) == 0)) {
					right_label = 0;
				}
	}
	if (right_label == 0) {/*if a regular label or addressing 2 label has an illegal name*/
		error_print(ILLEGAL_LABEL_NAME, ic);
		return 0;
	}
	else if (strlen(operand) - address2 > 31) {  /*if the label is longer than the max value*/
		error_print(WRONG_LABEL_LEN, ic);
		return 0;
	}
	return 1;
}

/* Return 0 if there is error */
int cmd_err_first_pass(int ic, char** wordsArray, int is_symbol, int wordsArraySize,Symbol** symbolsArray, int* symbols_count, char* line, int first_pass)
{
	if ((wordsArraySize - is_symbol) > 3) {/*command with more than 2 operands*/
		error_print(TOO_MANY_OP, ic);
		return 0;
	}
	if ((wordsArraySize - is_symbol) == 3)/*command with 2 operands*/
	{
		/*grammar_check(ic, line, is_symbol, 2);*/
		int immediate_first = 0, immediate_second = 0;

		char* command = wordsArray[0 + is_symbol];
		char* firstOp = wordsArray[1 + is_symbol];
		char* secondOp = wordsArray[2 + is_symbol];
		/*??check for commas and spaces in the right spot with a function??*/
		/*comma_check()*/
		if (first_pass == 1)
		{
			if ((strcmp(command, "mov") != 0) && (strcmp(command, "cmp") != 0) && (strcmp(command, "add") != 0) && (strcmp(command, "sub") != 0) && (strcmp(command, "lea") != 0)) {/*2 operands with valid command name but wrong command*/
				error_print(TOO_MANY_OP, ic);
				return 0;
			}


			/*firstOp checks*/
			if (firstOp[0] == '&') {/*if an operand is addressing method 2*/
				error_print(ILLEGAL_SRC_OP, ic);
				return 0;
			}
			if (strcmp(command, "lea") == 0)
				if (firstOp[0] == 'r' || firstOp[0] == '#' || firstOp[0] == '&') {
					error_print(ILLEGAL_SRC_OP, ic);
					return 0;
				}
			/*secondOp checks*/
			if (secondOp[0] == '&') {
				error_print(ILLEGAL_DST_OP, ic);
				return 0;
			}
			if (((strcmp(command, "mov") == 0) || (strcmp(command, "add") == 0) || (strcmp(command, "sub") == 0) || (strcmp(command, "lea") == 0)) && secondOp[0] == '#')
			{
				error_print(ILLEGAL_DST_OP, ic);
				return 0;
			}
			/*after we know that the command has the right type of opperands*/
			/*checking first operand validity*/
			if (firstOp[0] == '#' ) {/*checking if its a number*/
				if (check_immediate(firstOp, ic) == 0)
					immediate_first = 1;
					return 0;
			}
		}
		if (firstOp[0] == 'r' && immediate_first == 0) { /*if its a register*/
            if (first_pass == 1 && check_reg_in_first(firstOp, ic, symbolsArray, symbols_count) == 0)
                return 0;

			if (first_pass == 0 && check_register(firstOp, ic, symbolsArray, symbols_count) == 0)
				return 0;
		}

		else if(immediate_first == 0 ){/*if its a label*/
			if (first_pass == 1 && labelCheck(firstOp, 0, ic) == 0 )
				return 0;
			if (first_pass == 0 && check_label_presence(firstOp, symbolsArray, symbols_count, ic) == 0)
				return 0;
		}
		/*checking second operand validity*/
		if (secondOp[0] == '#' ) { /*checking if its a number*/
			immediate_second = 1;
			if (first_pass == 1 && check_immediate(secondOp, ic) == 0)
				return 0;
		}

		else if (secondOp[0] == 'r' && immediate_second == 0 && first_pass == 0) { /*if its a register*/

            if (first_pass == 1 && check_reg_in_first(firstOp, ic, symbolsArray, symbols_count) == 0)
                return 0;

			if (first_pass == 0 && check_register(secondOp, ic, symbolsArray, symbols_count) == 0)
				return 0;
		}
		else if(immediate_second == 0 && first_pass == 0){
			if (labelCheck(secondOp, 0, ic) == 0)/*if its a label*/
				return 0;
			if (first_pass == 0 && check_label_presence(secondOp, symbolsArray, symbols_count, ic) == 0)
				return 0;
		}
	}
	else if ((wordsArraySize - is_symbol) == 2)/*command with 1 operand*/
	{
		/*grammar_check(ic, line, is_symbol, 1);*/
		int immediate_first = 0;
		char* command = wordsArray[0 + is_symbol];
		char* firstOp = wordsArray[1 + is_symbol];
		if (first_pass == 1) {
			if ((strcmp(command, "mov") == 0) || ((strcmp(command, "cmp") == 0)) || (strcmp(command, "add") == 0) || (strcmp(command, "sub") == 0) || (strcmp(command, "lea") == 0)) {/*meaning there isnt enough operands for the command*/
				error_print(SCARCE_OPS, ic);
				return 0;
			}
			else if ((strcmp(command, "rts") == 0) || (strcmp(command, "stop") == 0)) {/*too many operands for these commands*/
				error_print(TOO_MANY_OP, ic);
				return 0;
			}

			/*firstOp checks*/
			if (firstOp[0] == '#' && (strcmp(command, "prn") != 0)) {/*if the operand is addressing method 0*/
				error_print(ILLEGAL_DST_OP, ic);
				return 0;
			}
			if ((strcmp(command, "jmp") != 0) && (strcmp(command, "bne") != 0) && (strcmp(command, "jsr") != 0) && firstOp[0] == '&') {/*if the command isnt a jump command and theres an address for a label*/
				error_print(ILLEGAL_DST_OP, ic);
				return 0;
			}
			if (((strcmp(command, "jmp") == 0) || (strcmp(command, "bne") == 0) || (strcmp(command, "jsr") == 0)) && firstOp[0] == 'r') {/*if the command is a jump command with a register operand*/
				error_print(ILLEGAL_DST_OP, ic);
				return 0;
			}
			/*after we know that the command has the right type of opperands*/
			/*checking operand validity*/
			if (firstOp[0] == '#') { /*checking if its a number*/
				immediate_first = 1;
				if (check_immediate(firstOp, ic) == 0)
					return 0;
			}
		}

		if (firstOp[0] == 'r' && immediate_first==0) { /*if its a register*/
			if (first_pass == 1 && check_reg_in_first(firstOp, ic, symbolsArray, symbols_count) == 0)
			    return 0;

		    if (first_pass == 0 && check_register(firstOp, ic, symbolsArray, symbols_count) == 0)
				return 0;
		}

		else if (firstOp[0] == '&' && immediate_first == 0) {/*if its an address of a label*/
			if (labelCheck(firstOp, 1, ic) == 0)
				return 0;
			if (first_pass == 0 && check_label_presence(firstOp+1, symbolsArray, symbols_count, ic) == 0)
				return 0;
		}
		else if(immediate_first == 0 && first_pass==1){
			if (labelCheck(firstOp, 0, ic) == 0)/*if its a label*/
				return 0;
			if (first_pass == 0 && check_label_presence(firstOp , symbolsArray, symbols_count, ic) == 0)
				return 0;
		}

	}
	else if ((wordsArraySize - is_symbol) == 1)/*command with 0 operand */
	{
		/*grammar_check(ic, line, is_symbol, 0);*/
		if (first_pass == 1) {
			char* command = wordsArray[0 + is_symbol];
			if ((strcmp(command, "rts") != 0) && (strcmp(command, "stop") != 0)) {
				error_print(SCARCE_OPS, ic);
				return 0;
			}
		}
	}
	else if ((wordsArraySize - is_symbol) == 0)/* just a label*/
	{
		if (first_pass == 1) {
			error_print(LABEL_WITHOUT_COMMAND, ic);
			return 0;
		}
	}
	return 1;
}

int error_print(enum ErrorType type, int ic)
{
	switch (type) {
	case TOO_MANY_OP:
		printf("Error in line %d: Too many operands for command\n", ic);
		break;
	case ILLEGAL_SRC_OP:
		printf("Error in line %d: illegal source operand\n", ic);
		break;
	case ILLEGAL_DST_OP:
		printf("Error in line %d: illegal destination operand\n", ic);
		break;
	case INVALID_IMMEDIATE_VALUE:
		printf("Error in line %d: Invalid immediate value\n", ic);
		break;
	case INVALID_REGISTER:
		printf("Error in line:%d  Contains invalid register\n", ic);
		break;
	case ILLEGAL_LABEL_NAME:
		printf("Error in line %d: illegal label name\n", ic);
		break;
	case WRONG_LABEL_LEN:
		printf("Error in line %d:The length of the label is more than 31 chars\n", ic);
		break;
	case SCARCE_OPS:
		printf("Error in line %d: Not enough operands for command\n", ic);
		break;
	case LABEL_WITHOUT_COMMAND:
		printf("Error in line %d: Label without command\n", ic);
		break;
	case	ILLEGAL_COMMA:
		printf("Error in line %d: Illegal comma\n", ic);
		break;
	case	MISSING_COMMA:
		printf("Error in line %d: Missing comma\n", ic);
		break;
	case UNDEFINED_LABEL:
		printf("Error in line %d: Undefined label \n", ic);
		break;
	case SCARCE_OPS_IN_GUIDANCE:
		printf("Error in line %d: Scarce number of operands in guidance sentence \n", ic);
		break;
	case TOO_MANY_OPS_IN_GUIDANCE:
		printf("Error in line %d: Too many operands in guidance sentence \n", ic);
		break;
	case MISSING_QUOTATION_MARK:
		printf("Error in line %d: Missing quotation mark \n", ic);
		break;
	case END_DATA_WITHOUT_NUMBER:
		printf("Error in line %d: End of data without number \n", ic);
		break;
	case MISSING_NUMBER:
		printf("Error in line %d: Missing a number \n", ic);
		break;
		
	}
	
	return 0;
}

int check_immediate(char* operand, int ic)
{
	char* temp;
	double num = strtod(operand + 1, &temp);
	if (num == 0) {
		error_print(INVALID_IMMEDIATE_VALUE, ic);
		return 0;
	}
	if (strlen(temp) > 0 && !isspace(temp[0])){
		error_print(INVALID_IMMEDIATE_VALUE, ic);
		return 0;
	}
	return 1;
}

int check_reg_in_first(char* operand, int ic, Symbol** symbolsArray, int* symbols_count)
{
    char* temp;

    if (strtod(operand + 1, &temp) == 0 || (strtod(operand + 1, &temp)>7 && strlen(temp) ==0) || (strtod(operand + 1, &temp) <0 && strlen(temp) == 0)) {/*if theres nothing after the 'r' or a number over 7 or under 0*/
        error_print(INVALID_REGISTER, ic);
        return 0;
    }
    if (strlen(temp) > 0 && temp[0] != ' ') /*if this goes through, we are talking about a label that starts with 'r'*/
    {
        error_print(INVALID_REGISTER, ic);
        return 0;
    }
    return 1;

}


int check_register(char* operand, int ic, Symbol** symbolsArray, int* symbols_count)
{
	char* temp;

	if (strtod(operand + 1, &temp) == 0 || (strtod(operand + 1, &temp)>7 && strlen(temp) ==0) || (strtod(operand + 1, &temp) <0 && strlen(temp) == 0)) {/*if theres nothing after the 'r' or a number over 7 or under 0*/
		if (check_label_presence(operand, symbolsArray, symbols_count, ic) == 0) {
			return 0;
		}
		else
			return 0;
	}
	if (strlen(temp) > 0 && temp[0] != ' ') /*if this goes through, we are talking about a label that starts with 'r'*/
		if ( check_label_presence(operand, symbolsArray, symbols_count, ic) == 0)
			return 0;
	return 1;
}
  