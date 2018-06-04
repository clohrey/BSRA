.PHONY: run clean

dialog :dialog.c
	gcc -g -ansi -pedantic -Wall dialog.c -o dialog
	
run: dialog
	valgrind --leak-check=yes ./dialog
		
run2: dialog
	nemiver dialog
	
run3: dialog
	./dialog
	
clean:
	rm dialog dialog.o
