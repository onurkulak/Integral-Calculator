
all: integral tintegral

function.o: function.c
	gcc -c -g -Wall -o function.o function.c  -lm

integral: integral.c function.o
	gcc -g -Wall -o integral function.o integral.c -lm

tintegral: tintegral.c function.o
	gcc -g -Wall -o tintegral function.o tintegral.c -lpthread -lm

clean:
	/bin/rm -fr *~ *.o integral tintegral
