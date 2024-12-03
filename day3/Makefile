exec:
	gcc -Wall -g -o day3 day3.c
	./day3
	more output.out

check:
	gcc -Wall -g -o day3 day3.c
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all ./day3
	more output.out
