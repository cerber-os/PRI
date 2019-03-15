# Project 3

Create program computing arithmetic operations, such as:
  - addition
  - subtraction
  - multiplication
  - division
  - exponentation
  - calculation of logarithmic and exponental function

It should also allows to save results in variables and use them in later computations.
Example desired program inputs and outputs:
```$ a = -2 + 3 * (3 - 4 / (2 - 0)) + log2(4)**exp2(2) + sqrt(9)
$ a
20
$ b = 7 - 2 / a
$ b
6.9
$ c = a + b
$ c
26.9
```

**Build:**
```sh
$ make
```
or
```sh
$ gcc -o Calc -O2 -Wall --std=c99 $(ls *.c)
```

**Note:** Entry point of program is located in file ```Calc.c```
