#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "linked_list.h"

/*a function that create the pattern of a word in 24 bits*/


void bin(unsigned x);
void print_bin(int x);

void bit_reg(MachineCode* word, char* first_op, char* second_op);
void address_type(char* command, char* first_op, char* second_op, MachineCode* word);
