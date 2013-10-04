# Makefile for cSound
#

OBJS = cSound.o \
       sound.o  

CFLAGS = -Wall -g -lm

all: cPlay

cPlay: ${OBJS}
	g++ -o cPlay ${CFLAGS} ${OBJS}

${OBJS}: sound.h

clean:
	rm -f cPlay *.o

test:
	./cPlay
