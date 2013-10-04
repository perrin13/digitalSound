# Makefile for cSound
#

OBJS = wavPlay.o \
       sound.o  

CFLAGS = -Wall -g -lm

all: wPlay

wPlay: ${OBJS}
	g++ -o wPlay ${CFLAGS} ${OBJS}

${OBJS}: sound.h

clean:
	rm -f wPlay *.o

test:
	./wPlay
