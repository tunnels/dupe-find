all:
	gcc -std=gnu99 -Wall -Wpedantic -o df main.c 
	echo "" && ./df

clean:
	rm df
