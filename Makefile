all: hangman

hangman: hangman.c
	gcc -Wall hangman.c -o hangman

.PHONY: clean
clean:
	rm -f hangman
