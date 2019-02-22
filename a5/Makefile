all: compile run

compile: a5_bits.c a5.h
	gcc a5_bits.c -o bits
	gcc a5_matrix.c -o matrix -g

run: a5_matrix.c a5.h
	./bits
	./matrix

# This target is used for test cases on all files. It will not work unless
# only the main function in a5_test.c is visible.
#test_all: a5_test.c a5_bits.c a5_matrix.c
#	gcc a5_bits.c a5_matrix.c a5_test.c -o test -g

clean:
	rm *.c~ *.h~ Makefile~
