#include <stdio.h>
#include <string.h>
#include "command_testing.h"
#include "second_pass.h"
#include "testing.h"

int main(int argc, char const *argv[]) 
{
	int argIndex;

	for (argIndex = 1; argIndex < argc; argIndex++)
	{
		FILE* file;
		int symbols_amount = 0, externSymbols_amount = 0,i=0, has_error = 0;
		Symbol* symbolsArray = NULL, *externSymbolsArray=NULL;
		char* filename[50], filenamePrefix[50];
		strcpy(filenamePrefix, argv[argIndex]);
		strcpy(filename, argv[argIndex]);
		strcat(filename, ".as");
		
		file = fopen(filename, "r");
		if (file == NULL)
		{
			printf("Cannot open file text '%s'\n", filename);
			continue;
		}
		has_error = first_pass(file, &symbolsArray, &symbols_amount);

		if (!has_error) {

            has_error = second_pass(file, &symbolsArray, &externSymbolsArray, symbols_amount, &externSymbols_amount);
            if (!has_error) {

                write_to_ent(filenamePrefix, &symbolsArray, symbols_amount);
                write_to_ob(filenamePrefix, global_dcf, global_icf);
                write_to_ext(filenamePrefix, &externSymbolsArray, externSymbols_amount);
            }

        }

		clean(); /* clean list of machine codes */

	}
	
	
	return 0;

}
