# FIFO-API
Example of a Circular FIFO which can hold a large chunk of memory and manage pushing and popping bytes from it . 


This example allows you to  push and Pop random amount of bytes into the circular buffer FIFO .

TO build and RUN it >

1. gcc -0 fifo fifo.c

2. ./fifo

Future Work. 

I would like to add a High Watermark and low Watermark to the buffer so that it can notify the caller when the critical limits reach .This has apractical use in streaming applciations where buffer is being fed from one end and consumed from other end .
