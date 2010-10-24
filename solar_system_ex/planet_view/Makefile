#MAKEFILE

# Make Option
CC	 = gcc
GXX	 = g++
LD       = ld

INCDIR = -I./ -I../utimage

CXXFLAGS = -g -Wall -O2 ${INCDIR}
CFLAGS	 = -g -Wall -O2 ${INCDIR}

MV = mv
RM = rm -f
CP = cp -f
AR = ar -rs
LN = ln -sf

TARGET = planet
HEADERS = 
OBJS = planet.o planet_custom.o main.o

LDFLAGS = -L../utimage -lutimage -lGL -lGLU -lglut -lglui -lm -ljpeg

.c.o:
	${CC} ${CFLAGS} -c $<

.cpp.o:
	${CXX} ${CXXFLAGS} -c $<

all:${TARGET}

${TARGET}:${OBJS}
	${CC} -o ${TARGET} ${OBJS} ${LDFLAGS}

clean:
	${RM} ${TARGET} *.o *~ 


