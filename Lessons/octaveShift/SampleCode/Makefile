# Makefile for cSound
#

OBJS = octaveShift.o \
       sound.o \
       fft.o	 

CFLAGS = -Wall -g -lm

all: shift

shift: ${OBJS}
	g++ -o shift ${CFLAGS} ${OBJS}

${OBJS}: sound.h fft.h

clean:
	rm -f shift *.o

test:
	./shift
