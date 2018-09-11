all:
	clear
	gcc -std=c99 -g -Wall -Wpedantic source/dupe_find.c source/integrity.c -o df

macos: clean all
	./df /Users/alex/Desktop/CYOAs
	# ./df /Users/alex/Desktop/based

windows: clean all 
	./df.exe tests/samples/

clean:
	touch df
	rm -f df
