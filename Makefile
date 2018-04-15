CC = gcc

readIR:	readIR.c
	$(CC) -o $@ -lwiringPi readIR.c -Wall