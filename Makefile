all:
	clear
	gcc -std=c99 -g -Wall -Wpedantic source/df.c -o df

macos: clean all
	./df tests/images/

windows: clean all 
	./df.exe tests/samples/

clean:
	touch df
	rm -f df
