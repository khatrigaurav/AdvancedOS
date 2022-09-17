//---------------------------------------------------------------
// File     : myalloc.c
// Purpose  : Implementation of custom memory allocation.
// Language : C
// Author   : Gaurav Khatri
// Date     : Sep 14, 2022
//---------------------------------------------------------------

#include <stdio.h>
#include <sys/mman.h>
#include "myalloc.h"

node_t *head = NULL;
node_t *free_node = NULL;
node_t *new_node = NULL;

// This will initialize a big chunk of requested size i.e. 4KB in this case.
void *initialize_memory()
{
	if (head == NULL)
	{

		head = (node_t*) mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		head->size = 4096 - sizeof(head);
		head->next = NULL;
		head->previous = NULL;
		head->free = 1;
		printf("\nMemory initialization complete. A 4KB block has been set.");
	}
	return head;
}

//Test function that shows the address changes when new elements are added to the head
void add_element(int size)
{
	node_t *temp = head;

	temp->size = size;
	temp->next = head;

	head = temp + size + sizeof(temp);
	
}

//The following function should find a node among all the available nodes that fits the memory requirement.
// Use First fit strategy

void first_free_finder(int size, node_t **free_node)
{
	// node_t *head = initialize_memory();
	// *free_node = NULL;
	printf("\n\tNode finding in progress : ");
	node_t *temp = head;
	while (temp != NULL)
	{
		if (temp->size + sizeof(node_t) >= size + sizeof(header_t))
		{

			*free_node = temp;
			printf("\n\tFirst free node has been found and initialized");
			printf("\n\t\tFree node address : %p", temp);
			printf("\n\t\tFree node size : %d", temp->size);

			break;
		}
		temp = temp->next;
		printf("\n\tSearching in next block...");
	}
	// printf("\n Details of free block");
	// printf("\n head addr : %p",head);
	// printf("\n found addr : %p",found);
	// printf("\n head size : %d",head->size);
}

/*The node split function divides the free_node into new_node and free_node.
free_node is the original huge block of memory that will take the size 's' after this operation 
new_node is the new node created will be transferred to the right i.e free_node->next = new_node */

void node_split(int size, node_t **free_node, node_t **new_node)
{
	printf("\n\t\tProcess initiated to split the found free node.");

	node_t **temp;

	*new_node = *free_node + size;
	(*new_node)->size = (*free_node)->size - size - sizeof(header_t);
	(*new_node)->free = 1;
	(*free_node)->size = size;
	(*free_node)->free = 0;
	(*new_node)->previous = *free_node;
	(*new_node)->next = (*free_node)->next;
	(*free_node)->next = *new_node;
	printf("\n\t\tProcess completed to split the free node.");
	// head= *free_node;

}

void *custom_malloc(int size)
{

	printf("\n\nMalloc initiated for size : %d", size);

	first_free_finder(size, &free_node);

	//Print statements to test the pointer allocation
	// printf("\n Status before allocating");
	// printf("\n Free_node addr : %p",free_node);
	// printf("\n Free_node size : %d",free_node->size);
	// printf("\n Free Node Next : %p",free_node->next);
	// printf("\n Free Node Previous : %p",free_node->previous);

	if (free_node == NULL)
	{

		printf("No available memory \n");
		return NULL;
	}

	node_split(size, &free_node, &new_node);

	printf("\n\nStatus after allocating");
	printf("\n New_Node addr : %p", new_node);
	printf("\n New Node Next : %p", new_node->next);
	printf("\n New_Node size : %d", new_node->size);
	printf("\n New_Node previous : %p", new_node->previous);
	printf("\n Free status : %d", new_node->free);

	return ((char*) free_node) + sizeof(header_t);

}


void free_memory(void *memory_region)
{

	void *exact_location = memory_region - sizeof(header_t);
	printf("\n\nFree operation started for node at location %p", exact_location);
	node_t *corresponding_node = (node_t*)(exact_location);

	corresponding_node->free = 1;	//setting free_flag to 1

	// corresponding_node->next->free = 1;	//used for testing purpose

	if (corresponding_node->next && corresponding_node->next->free == 1)
	{
		//if consecutive locations are free, join them
		printf("\nConsecutive locations are free, coaelescing initiated");
		corresponding_node->size = corresponding_node->size + corresponding_node->next->size;
		corresponding_node->next = corresponding_node->next->next;
		corresponding_node->next->previous = corresponding_node->previous;
	}

	if (corresponding_node->previous && corresponding_node->previous->free == 1)
	{
		//if previous location is free, join them
		printf("\nPrevious location is free, coaelescing initiated");
		corresponding_node->previous->size = corresponding_node->previous->size + corresponding_node->size;
		corresponding_node->previous = corresponding_node->previous->previous;
		corresponding_node->previous->next = corresponding_node->next;
	}

	printf("\nFree operation completed for node at location %p", exact_location);
	printf("\n\tNew node size at same location : %d", corresponding_node->size);
	printf("\n\tNext node address at same location : %p", corresponding_node->next);

}
