# Makefile for cSound
#

OBJS = vocode.o \
       sound.o  

CFLAGS = -Wall -g -lm

all: vocode

vocode: ${OBJS}
	g++ -o vocode ${CFLAGS} ${OBJS}

${OBJS}: sound.h

clean:
	rm -f vocode *.o

test:
	./vocode
