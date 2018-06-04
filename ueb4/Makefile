.PHONY: run clean

filereader: filereader.c
	gcc -g -ansi -pedantic -Wall filereader.c -o filereader

run: filereader
	valgrind --leak-check=yes ./filereader

run2: filereader
	nemiver filereader

run3: filereader
	./filereader

clean:
	rm filereader filereader.o
