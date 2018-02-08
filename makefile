CC=cc
CFLAGS=-g

main: main.o print_signals.o
	$(CC) $(CFLAGS) main.o print_signals.o -lc -o main

clean:
	-@rm -f main
	-@rm -f main.o 
