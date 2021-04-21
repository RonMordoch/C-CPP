CC= gcc
CFLAGS= -Wextra -Wall -Wvla -std=c99

default: NumericalAnalyzer
all : default

NumericalAnalyzer: NumericalAnalyzer.o libinfi.a
	$(CC) $(CFLAGS) NumericalAnalyzer.o -L. -lm -linfi -o NumericalAnalyzer

NumericalAnalyzer.o: NumericalAnalyzer.c
	$(CC) $(CFLAGS) -c NumericalAnalyzer.c

infi.o: infi.c infi.h
	$(CC) $(CFLAGS) -c infi.c

libinfi.a: infi.o infi.h
	ar rcs libinfi.a infi.o infi.h

clean:
	rm NumericalAnalyzer *.o *.a

.PHONY: all, clean