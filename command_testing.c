#include "command_testing.h"
#include "symbol.h"




/*determine the type of the addresing according of the operands we get*/

void address_type(char* command, char* first_op, char* second_op, MachineCode* word) { /*filling bits for (16-17)add_src&(11-12)add_des: by using first and second operand from the code*/

	/* (2) relative addressing - defining the bits 11-12 in case number 2 of addressing*/
	/*single operand with jump to an address*/
	if ((strcmp(command, "jmp") == 0) || (strcmp(command, "bne") == 0) || (strcmp(command, "jsr") == 0)) {

		if ((second_op[0] == '&') && (first_op == NULL)) {
			word->fields.A = 1;
			word->fields.R = 0;
			word->fields.E = 0;
			word->fields.add_src = 0;
			word->fields.add_des = 2;

		}
		if ((second_op[0] != '&') && (isalpha(second_op[0]) != 0) && (first_op == NULL)) {
			word->fields.A = 1;
			word->fields.add_src = 0;
			word->fields.add_des = 1;

		}

	}
	/*commands that dont have operands*/
	else if ((strcmp(command, "rts") == 0) || (strcmp(command, "stop") == 0)) {
		word->fields.add_src = 0;
		word->fields.add_des = 0;
		word->fields.A = 1;
		word->fields.R = 0;
		word->fields.E = 0;
	}
	/*single operand without jump to an address*/
	else if ((strcmp(command, "clr") == 0) || (strcmp(command, "not") == 0) || (strcmp(command, "inc") == 0) || (strcmp(command, "dec") == 0) || (strcmp(command, "red") == 0) || (strcmp(command, "prn") == 0)) {
		if ((first_op == NULL) && (second_op[0] == 'r')) {
			/*register*/
			word->fields.add_src = 0;
			word->fields.add_des = 3;
			word->fields.A = 1;
			word->fields.R = 0;
			word->fields.E = 0;
		}
		else if ((first_op == NULL) && isdigit(second_op[1])) { /*immediate value*/
			word->fields.add_src = 0;
			word->fields.add_des = 0;
			word->fields.A = 1;
		}
		else {/*label*/
			word->fields.add_src = 0;
			word->fields.add_des = 1;
			word->fields.A = 1;
		}
	}

	/*commands with 2 operands*/
	else
	{
		word->fields.A = 1;
		word->fields.R = 0;
		word->fields.E = 0;

		if (first_op[0] == '#') {/*(0) immidiate addressing - in case the operand is a number and start with '#'*/
			word->fields.add_src = 0;
		}
		else if (first_op[0] == 'r') {/* (3) direct register addresing - the operand is a register*/
			word->fields.add_src = 3;
		}
		else {
			word->fields.add_src = 1;      /* (1) direct addressing -in case the operand is located in the memory*/
		}

		if (second_op[0] == '#') {/*(0) immidiate addressing - in case the operand is a number and start with '#'*/
			word->fields.add_des = 0;
		}
		else if (second_op[0] == 'r') {/* (3) direct register addresing - the operand is a register*/
			word->fields.add_des = 3;
		}
		else {
			word->fields.add_des = 1;      /* (1) direct addressing -in case the operand is located in the memory*/
		}



	}
}



/*function that determine the bits for both operand source(13-15 reg_src) & destination(8-10 reg_des) */

void bit_reg(MachineCode* word, char* first_op, char* second_op) {
	word->fields.reg_src = 0;
	word->fields.reg_des = 0;
	if (first_op==NULL)
		word->fields.reg_src = 0;
	else if ((first_op[0] == 'r'))/*if the source operand is register */
		word->fields.reg_src = atoi(first_op + 1);
	if (second_op == NULL)
		word->fields.reg_des = 0;
	else if ((second_op[0] == 'r'))/*if the source operand is register */
		word->fields.reg_des =  atoi(second_op + 1);
}


	
			
