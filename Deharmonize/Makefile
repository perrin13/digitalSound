# Makefile for cSound
#

OBJS = autoTune.o \
       sound.o \
       fft.o	 

CFLAGS = -Wall -g -lm

all: tune

tune: ${OBJS}
	g++ -o tune ${CFLAGS} ${OBJS}

${OBJS}: sound.h fft.h

clean:
	rm -f tune *.o

test:
	./tune
