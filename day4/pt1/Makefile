exec:
	gcc -Wall -g -o day4 day4.c
	./day4
	more output.out

check:
	gcc -Wall -g -o day4 day4.c
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all ./day4
	more output.out
