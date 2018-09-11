all:
	clear
	gcc -std=c99 -g -Wall -Wpedantic source/dupe_find.c source/integrity.c -o df

macos: clean all
	./df tests/test1 tests/test1/subtest1

windows: clean all 
	./df.exe tests/samples/

clean:
	touch df
	rm -f df
