#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>


enum ErrorType { TOO_MANY_OP = 1, ILLEGAL_SRC_OP, ILLEGAL_DST_OP, INVALID_IMMEDIATE_VALUE,INVALID_REGISTER, ILLEGAL_LABEL_NAME, WRONG_LABEL_LEN,SCARCE_OPS, LABEL_WITHOUT_COMMAND, ILLEGAL_COMMA, MISSING_COMMA, UNDEFINED_LABEL, SCARCE_OPS_IN_GUIDANCE, TOO_MANY_OPS_IN_GUIDANCE, MISSING_QUOTATION_MARK, END_DATA_WITHOUT_NUMBER, MISSING_NUMBER};
int labelCheck(char* operand, int add2, int ic);
int cmd_err_first_pass(int ic, char** wordsArray, int is_symbol, int wordsArraySize, Symbol** symbolsArray, int* symbols_count, char* line, int first_pass);
int error_print(enum ErrorType type, int ic);
int check_immediate(char* operand, int ic);
int check_reg_in_first(char* operand, int ic, Symbol** symbolsArray, int* symbols_count);
int check_register(char* operand, int ic, Symbol** symbolsArray, int* symbols_count);
int grammar_check(int ic, char* line, int is_symbol, int right_operand_count);
int check_guidance_sentence(char* line, int is_symbol, int data, int string, int entry, int external, int ic);
