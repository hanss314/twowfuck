# twowfuck
twowfuck programming language

twowfuck is an absolute trash programming language

The sole purpose of twowfuck is for people to incorporate in trash technical restrictions.

Every string is a valid twowfuck program, and there are no errors. 

# Documentation

The interpreter takes two parameters, the program and the input. The input is optional, and will be treated as an empty string. 
The interpreter does not and will not ever read from disk, or even stdin (this might change).

There is an input pointer which starts at the first byte of the input. If the input is empty, it is treated as an empty string.

The memory consists of an infinite tape of integers in both directions, along with a memory pointer.

The program pointer starts at the first byte of the program, and increments by one every instruction, unless a jump is made.

Each byte is interpreted as an instruction based on its value modulo 7.

0 - Move the memory pointer left

1 - Move the memory pointer right

2 - Increment integer at memory pointer

3 - Decrement integer at memory pointer

4 - Set value at memory pointer to value at input pointer. This then increments the input pointer. 
At the end of a string, 0 will be written to memory and the input pointer will return to the start of the input.

5 - Casts the integer at memory pointer to an unsigned char and prints to stdout

6 - If the current memory location stores 0, increment the program pointer by 3 instead of 1. 
Otherwise, it takes the least significant 64 bits of the sha1 digest of the program after the current pointer, 
modulos it by the length of the program, and sets the program pointer to that location.
