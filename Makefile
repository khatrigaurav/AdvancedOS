run: 
	./main

clean:
	rm *.o

library:
	gcc -o myalloc.o -c myalloc.c
	ar rcs myalloc.a myalloc.o 

main:
	gcc -o main main.c myalloc.a