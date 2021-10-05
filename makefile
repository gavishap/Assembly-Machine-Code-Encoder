assembler: main.o command_testing.o files.o first_pass.o funct_dictionary.o global_functions.o linked_list.o second_pass.o symbol.o testing.o
	gcc -g -Wall -ansi -pedantic main.o command_testing.o files.o first_pass.o funct_dictionary.o global_functions.o linked_list.o second_pass.o symbol.o testing.o -o assembler

main.o: main.c command_testing.h files.h second_pass.h
	gcc -c main.c -o main.o -Wall -ansi -pedantic

command_testing.o: command_testing.c command_testing.h symbol.h
	gcc -c command_testing.c -o command_testing.o -Wall -ansi -pedantic

files.o: files.c files.h linked_list.h symbol.h
	gcc -c files.c -o files.o -Wall -ansi -pedantic

first_pass.o: first_pass.c command_testing.h first_pass.h funct_dictionary.h testing.h
	gcc -c first_pass.c -o first_pass.o -Wall -ansi -pedantic

funct_dictionary.o: funct_dictionary.c funct_dictionary.h
	gcc -c funct_dictionary.c -o funct_dictionary.o -Wall -ansi -pedantic

global_functions.o: global_functions.c global_functions.h
	gcc -c global_functions.c -o global_functions.o -Wall -ansi -pedantic

linked_list.o: linked_list.c linked_list.h
	gcc -c linked_list.c -o linked_list.o -Wall -ansi -pedantic

second_pass.o: second_pass.c second_pass.h linked_list.h
	gcc -c second_pass.c -o second_pass.o -Wall -ansi -pedantic

symbol.o: symbol.c symbol.h linked_list.h
	gcc -c symbol.c -o symbol.o -Wall -ansi -pedantic

testing.o: testing.c testing.h
	gcc -c testing.c -o testing.o -Wall -ansi -pedantic