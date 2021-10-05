

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct command
{
	char name[10];
	int funct_id;
	int opcode;
}command, * commands_dictionary;

#define COMMAND_COUNT 16

void fill_command(command* commnadPtr, char* name, int funct_id, int opcode);

commands_dictionary build_dictionary();

command* findCommand(commands_dictionary dictionary, char* name);
