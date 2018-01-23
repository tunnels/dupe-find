all:
	gcc -std=gnu99 -Wall -Wpedantic -o df source/df.c 
	echo "" && ./df

clean:
	rm df
