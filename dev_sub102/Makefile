OBJECTS=sub102.o sub102comp.o

CFLAGS=-g

all: $(OBJECTS)

clean: 
	rm $(OBJECTS)

depend:
	makedepend *.cpp

%.o: %.cpp Makefile
	g++ -c $(CFLAGS) $*.cpp

