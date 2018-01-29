all:
	clear
	gcc -std=gnu99 -Wall -Wpedantic -o df source/df.c
	./df tests/a.png tests/images/

clean:
	rm df
