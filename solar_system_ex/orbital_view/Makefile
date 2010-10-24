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

TARGET = orbital_view
HEADERS = 
SOLAR_OBJS = planet.o planet_solar_system.o sun.o planet_earth.o planet_mercury.o planet_venus.o planet_mars.o planet_jupiter.o planet_saturn.o planet_uranus.o planet_neptune.o planet_pluto.o
OBJS = ${SOLAR_OBJS} get_clock_now.o main.o

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


