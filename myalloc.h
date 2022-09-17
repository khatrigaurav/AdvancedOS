//---------------------------------------------------------------
// File     : myalloc.h
// Purpose  : Header file for the myalloc.c.
// Language : C
// Author   : Gaurav Khatri
// Date     : Sep 14, 2022
//---------------------------------------------------------------

#ifndef MYALLOC_H_
#define MYALLOC_H_

typedef struct node_header{
	int size;	//size of the allocated memory for this node
	int magic;	//variable to hold the integrity

}header_t;

//free_list data structure
typedef struct node_t{
	int size;	                //size of the free memory block in this node
	struct node_t * next;	    //points to the next freelist data structure i.e next freeelist node
	struct node_t * previous;	//points to the previous node if it exists
	int free;	                //too keep track of whether the given node is free or not
}node_t;

void *initialize_memory();
void add_element(int);
void first_free_finder(int, node_t **);
void node_split(int, node_t **, node_t **);
void *custom_malloc(int);
void free_memory(void *);

#endif