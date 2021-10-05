#include "global_functions.h"
#include "funct_dictionary.h"

int command_lengths[100], global_icf, global_dcf;
int first_pass(FILE* file, Symbol** symbolsArray, int* symbols_count);
void handleCommand(command* cmd, char** wordsArray, int wordsArraySize, int* ic, int prefix_label, int only_des);
void resetCommandLengths();
