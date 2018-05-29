.PHONY: run clean

tiere: tiere.c
	gcc -g -ansi -pedantic -Wall database_handler.c tiere.c -o tiere

run: tiere
	valgrind --leak-check=yes ./tiere

run2: tiere
	nemiver tiere $(args)

run3: tiere
	./tiere $(args)

clean:
	rm tiere tiere.o
