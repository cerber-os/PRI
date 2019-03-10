CC=gcc
CFLAGS=-O2 -Wall --std=c99

MatrixCalculator : MatrixCalculator.o MatrixGUI.o SquareMatrix.o
	$(CC) $(CFLAGS) -o MatrixCalculator MatrixCalculator.o MatrixGUI.o SquareMatrix.o
SquareMatrix.o : SquareMatrix.c
	$(CC) $(CFLAGS) -c SquareMatrix.c
MatrixGUI.o : MatrixGUI.c
	$(CC) $(CFLAGS) -c MatrixGUI.c
MatrixCalculator.o : MatrixCalculator.c
	$(CC) $(CFLAGS) -c MatrixCalculator.c

.PHONY : clean
clean :
	rm -f MatrixCalculator MatrixCalculator.o MatrixGUI.o SquareMatrix.o
