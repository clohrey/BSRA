.PHONY: run clean

database_handler :database_handler.c
	gcc -g -ansi -pedantic -Wall database_handler.c -o database_handler
	
run: database_handler
	valgrind --leak-check=yes ./database_handler
		
run2: database_handler
	nemiver database_handler
	
run3: database_handler
	./database_handler
	
clean:
	rm database_handler database_handler.o
