all: runbenchmark

runslow: buildslow slowsort
	./slowsort

buildslow: slowsort.c
	gcc -o slowsort slowsort.c

runfast: buildfast fastsort
	./fastsort

buildfast: fastsort.c
	gcc -o fastsort fastsort.c

runbenchmark: slowsort fastsort
	./slowsort
	./fastsort

clean: 
	rm *.c~ Makefile~ fastsort slowsort
