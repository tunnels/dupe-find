all:
	gcc -std=gnu99 -Wall -Wpedantic -o df main.c 
	echo "\n\n\n" && ./df

clean:
	rm df
