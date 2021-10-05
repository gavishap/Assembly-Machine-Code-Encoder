#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef union {
    struct {
        unsigned int E : 1; /*define the '0' bit =E*/
        unsigned int R : 1; /*define the '1' bit =R*/
        unsigned int A : 1; /*define the '2' bit =A*/
        unsigned int funct : 5; /*define the 3-7 bits =funct*/
        unsigned int reg_des : 3; /*define 8-10 bits for the register destination */
        unsigned int add_des : 2; /*define 11-12 bits for the type of the addressing to destination*/
        unsigned int reg_src : 3; /*define 13-15 bits for the source register*/
        unsigned int add_src : 2; /*define 16-17 bits for the type of the addressing to source*/
        unsigned int opcode : 6; /*define 18-23 bits for the opcode*/
    }fields;
    unsigned int word : 24;
}
MachineCode;


typedef struct node {
    MachineCode mc; /* the data (mc = machine code)*/
    int ic; /* the search key */
    struct node* next;
}node;

MachineCode decimal_2_binary(int num);
void print_binary(MachineCode m);
void printList();
void insertFirst(int key, MachineCode data);
struct node* deleteFirst();
bool isEmpty();
int length();
struct node* find(int key);
struct node* delete(int key);
void reverse(struct node** head_ref);
void clean();
