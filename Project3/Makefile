CC=gcc
CFLAGS=-O2 -Wall --std=c99 -lm -D_POSIX_C_SOURCE=200809L -g

Calc : Calc.o Calculator.o Stack.o Strings.o Variables.o
	$(CC) $(CFLAGS) -o Calc Calc.o Calculator.o Stack.o Strings.o Variables.o
Calc.o : Calc.c
	$(CC) $(CFLAGS) -c Calc.c
Calculator.o : Calculator.c
	$(CC) $(CFLAGS) -c Calculator.c
Stack.o : Stack.c
	$(CC) $(CFLAGS) -c Stack.c
Strings.o : Strings.c
	$(CC) $(CFLAGS) -c Strings.c
Variables.o : Variables.c
	$(CC) $(CFLAGS) -c Variables.c

.PHONY : clean
clean :
	rm -f Calc Calc.o Calculator.o Stack.o Strings.o Variables.o
