all:
	clear
	gcc -std=gnu99 -Wall -Wpedantic -o df source/df.c
	./df tests/base.png tests/images/

clean:
	rm df
