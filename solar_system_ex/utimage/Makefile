#MAKEFILE

# Make Option
CC	 = gcc
GXX	 = g++

CXXFLAGS = -g -Wall -O2 -I./
CFLAGS	 = -g -Wall -O2 -I./

MV = mv
RM = rm -f
CP = cp -f
AR = ar -rs
LN = ln -sf

TARGET = libutimage.a

OBJS = utimage.o sgiimage.o jpegimage.o jpegcodec.o

.c.o:
	${CC} ${CFLAGS} -c $<

.cpp.o:
	${CXX} ${CXXFLAGS} -c $<

all: ${TARGET}

${TARGET}: ${OBJS}
	${AR} ${TARGET} ${OBJS}

clean:
	${RM} ${TARGET} *.o *~ 

