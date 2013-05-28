#
# Makefile for OpenGL program
#

SRCS = $(shell ls *.cpp)
OBJS = $(SRCS:.cpp=.o)
PROGRAM = product
RM = rm -f

CXX = g++
CXXFLAGS = -g -Wall $(pkg-config gl --cflags) $(pkg-config glew --cflags) 
LDFLAGS = -lGL -lGLU -lglut 

.PHONY: all depend clean
.SUFFIXES: .cpp .o

all: depend $(PROGRAM)
	

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) 

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

clean: 
	$(RM) $(OBJS) $(PROGRAM) depend.inc

depend: $(SRCS)
	-@ $(RM) depend.inc
	-@ for i in $^; do\
		cpp -MM $$i | sed "s/\ [_a-zA-Z0-9][_a-zA-Z0-9]*\.cpp//g" >> depend.inc;\
	done

-include depend.inc
