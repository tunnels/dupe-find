all:
	clear
	gcc -std=gnu99 -Wall -Wpedantic -o df source/df.c

macos: clean all
	./df tests/images/

windows: clean all 
	./df.exe tests/images/

clean:
	rm df -f
