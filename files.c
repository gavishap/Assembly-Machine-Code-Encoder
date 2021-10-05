
#include "files.h"
#include "linked_list.h"
#include "symbol.h"

char* decimalToHex(num)
{
    int i = 0, k = 0, rem, j;
    char hex_arr[6], *final_hex;
    final_hex = (char*)malloc(7 * sizeof(char));
    final_hex[6] = '\0';
    while (num != 0)
    {
        rem = num % 16;
        if (rem < 10)
        {
            hex_arr[i++] = 48 + rem;
        }
        else
        {
            hex_arr[i++] = 55 + rem;
        }
        num /= 16;
    }

    for (k = 6 - i - 1; k >= 0; k--)/*print zeros before the hex number*/
    {
        final_hex[k] = '0';
    }

    for (j = i - 1, k = 6 - i; j >= 0; j--, k++)
    {

        final_hex[k] = hex_arr[j];
    }

    return final_hex;
}


void write_to_ob(char* Filename, int global_dcf, int global_icf)
{
	int ic = 100;
	node* curr = find(ic);
	char tmp[50];
	strcpy(tmp, Filename);
	strcat(tmp, ".ob");

	FILE* obFile = fopen(tmp, "w");
	fprintf(obFile, "     %d %d\n", global_icf, global_dcf);
	if (obFile != NULL)
	{
		while (curr != NULL)
		{
			char* wordInHex = decimalToHex(curr->mc.word);
			fprintf(obFile, "%07d ", ic);
			fprintf(obFile, "%s\n", wordInHex);
			curr = find(++ic);

		}
	}

	fclose(obFile);

}


void write_to_ent(char* Filename, Symbol** symbolsArray, int symbols_count)
{
	char tmp[50];
	strcpy(tmp, Filename);
	strcat(tmp, ".ent");

	FILE* entFile = fopen(tmp, "w");
	if (entFile != NULL)
	{
		int i = 0;
		while (i<symbols_count)
		{
			if (strstr((*symbolsArray)[i].type, "entry") != NULL) {
				fprintf(entFile, "%s ", (*symbolsArray)[i].name);
				fprintf(entFile, " %07d\n", (*symbolsArray)[i].ic);
			}
			i++;
		}
	}

	fclose(entFile);

}

void write_to_ext(char* Filename, Symbol** symbolsArray, int symbols_count)
{
	char tmp[50];
	strcpy(tmp, Filename);
	strcat(tmp, ".ext");

	FILE* extFile = fopen(tmp, "w");
	if (extFile != NULL)
	{
		int i = 0;
		while (i < symbols_count)
		{
			fprintf(extFile, "%s ", (*symbolsArray)[i].name);
			fprintf(extFile, " %07d\n", (*symbolsArray)[i].ic);
			i++;
		}
	}

	fclose(extFile);

}
