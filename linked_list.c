#include "linked_list.h"


struct node* head = NULL;
struct node* current = NULL;

MachineCode decimal_2_binary(int num)
{
    MachineCode m;
    m.word = num;
    return m;
}

void print_binary(MachineCode m)
{
    int bits[24] = { 0 }, i;
    unsigned int num = m.word;

    for (i = 0; num > 0; i++)
    {
        bits[i] = num % 2;
        num = num / 2;
    }

    for (i = 23; i >= 0; i--)
    {
        printf("%d", bits[i]);
    }
    printf("\n");
}



void printList() {
    struct node* ptr = head;

   
    while (ptr != NULL) {
        printf("ic = %d,    ", ptr->ic);
        print_binary(ptr->mc);
        ptr = ptr->next;
    }
}


void insertFirst(int key, MachineCode data) {
 
    struct node* link = (struct node*) malloc(sizeof(struct node));

    link->ic = key;
    link->mc = data;

 
    link->next = head;

  
    head = link;
}


struct node* deleteFirst() {


    struct node* tempLink = head;


    head = head->next;


    return tempLink;
}


bool isEmpty() {
    return head == NULL;
}

int length() {
    int length = 0;
    struct node* current;

    for (current = head; current != NULL; current = current->next) {
        length++;
    }

    return length;
}


struct node* find(int key) {

  
    struct node* current = head;

  
    if (head == NULL) {
        return NULL;
    }

 
    while (current->ic != key) {

 
        if (current->next == NULL) {
            return NULL;
        }
        else {
   
            current = current->next;
        }
    }

  
    return current;
}


struct node* delete(int key) {

  
    struct node* current = head;
    struct node* previous = NULL;

  
    if (head == NULL) {
        return NULL;
    }

   
    while (current->ic != key) {

    
        if (current->next == NULL) {
            return NULL;
        }
        else {
     
            previous = current;
       
            current = current->next;
        }
    }

  
    if (current == head) {
 
        head = head->next;
    }
    else {
   
        previous->next = current->next;
    }

    return current;
}

void reverse(struct node** head_ref) {
    struct node* prev = NULL;
    struct node* current = *head_ref;
    struct node* next;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    *head_ref = prev;
}

void clean()
{
	while (!isEmpty())
	{
		deleteFirst();
	}
}
