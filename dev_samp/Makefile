OBJECTS=samp.o

CFLAGS=-g

all: $(OBJECTS)

clean: 
	rm $(OBJECTS)

depend:
	makedepend *.cpp

%.o: %.cpp Makefile
	gcc -c $(CFLAGS) $*.cpp

