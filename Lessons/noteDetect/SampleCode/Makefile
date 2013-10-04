# Makefile for cSound
#

OBJS = noteDetect.o \
       sound.o \
       fft.o	 

CFLAGS = -Wall -g -lm

all: note

note: ${OBJS}
	g++ -o note -lm ${CFLAGS} ${OBJS}

${OBJS}: sound.h fft.h

clean:
	rm -f note *.o

test:
	./note
