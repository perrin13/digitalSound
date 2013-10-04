# Makefile for cSound
#

OBJS = main.o \
       sound.o \
       fft.o	 

CFLAGS = -Wall -g -lm

all: a.out

vocode: ${OBJS}
	g++ -o a.out ${CFLAGS} ${OBJS}

${OBJS}: sound.h fft.h

clean:
	rm -f a.out *.o

test:
	./a.out
