all: compileP1 runP1 compileP2 runP2

compileP1: a5_bits.c a5.h
	gcc a5_bits.c -o bits

compileP2: a5_matrix.c a5_list.c a5.h
	gcc a5_matrix.c -o matrix
	gcc a5_list.c -o list

runP1: bits
	./bits

runP2: matrix list
	./matrix
	./list

# This target is used for test cases on all files. It will not work unless
# only the main function in a5_test.c is visible.
#test_all: a5_test.c a5_bits.c a5_matrix.c
#	gcc a5_bits.c a5_matrix.c a5_test.c -o test -g

clean:
	rm *.c~ *.h~ Makefile~
