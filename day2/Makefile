exec:
	gcc -Wall -g -o day2 day2.c
	./day2

check:
	gcc -Wall -g -o day2 day2.c
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all ./day2
