# Makefile for cSound
#

OBJS = deHarmo.o \
       sound.o \
       fft.o	 

CFLAGS = -Wall -g -lm

all: deHar

deHar: ${OBJS}
	g++ -o deHar ${CFLAGS} ${OBJS}

${OBJS}: sound.h fft.h

clean:
	rm -f deHar *.o

test:
	./deHar
