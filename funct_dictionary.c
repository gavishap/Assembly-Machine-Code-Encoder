#include "funct_dictionary.h"

void fill_command(command* commnadPtr, char* name, int funct_id, int opcode)
{
	strcpy(commnadPtr->name, name);
	commnadPtr->funct_id = funct_id;
	commnadPtr->opcode = opcode;
}

commands_dictionary build_dictionary()
{
	commands_dictionary dictionary = (commands_dictionary)malloc(sizeof(command) * COMMAND_COUNT);
	fill_command(dictionary, "mov", 0, 0 );
	fill_command(dictionary + 1 , "cmp", 0, 1);
	fill_command(dictionary + 2, "add", 1, 2);
	fill_command(dictionary + 3, "sub", 2, 2);
	fill_command(dictionary + 4, "lea", 0, 4);
	fill_command(dictionary + 5, "clr", 1, 5);
	fill_command(dictionary + 6, "not", 2, 5);
	fill_command(dictionary + 7, "inc", 3, 5);
	fill_command(dictionary + 8, "dec", 4, 5);
	fill_command(dictionary + 9, "jmp", 1, 9);
	fill_command(dictionary + 10, "bne", 2, 9);
	fill_command(dictionary + 11, "jsr", 3, 9);
	fill_command(dictionary + 12, "red", 0, 12);
	fill_command(dictionary + 13, "prn", 0, 13);
	fill_command(dictionary + 14, "rts", 0, 14);
	fill_command(dictionary + 15, "stop", 0, 15);

	return dictionary;
}
/*find a command in the dictionary and rerturn it*/
command* findCommand(commands_dictionary dictionary, char* name)
{
	int i;
	for ( i = 0; i < COMMAND_COUNT; i++)
	{/*ask about comparison*/
		if (strcmp(dictionary[i].name, name) == 0)
			return dictionary + i;
	}
	return NULL;
}


