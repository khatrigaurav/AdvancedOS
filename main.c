//---------------------------------------------------------------
// File     : main.c
// Purpose  : Driver program for custom memory allocation.
// Language : C
// Author   : Gaurav Khatri
// Date     : Sep 14, 2022
//---------------------------------------------------------------
#include <stdio.h>
#include "myalloc.h"

int main()

{
	initialize_memory();            //initializes the memory block.

	void *r1 = custom_malloc(2);    //Calling custom malloc for size 2.
	void *r2 = custom_malloc(30);
	void *r3 = custom_malloc(70);
	void *r4 = custom_malloc(190);

	free_memory(r2);                //releasing the memory associated with region 1.
	free_memory(r1);	            //This should show increased size from 30 to 32, demonstrating coalescing of memory blocks.

}