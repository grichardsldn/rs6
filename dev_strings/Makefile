OBJECTS=strings.o delay.o

CFLAGS=-g

all: $(OBJECTS)

clean: 
	rm $(OBJECTS)

depend:
	makedepend *.cpp

%.o: %.cpp Makefile
	g++ -c $(CFLAGS) $*.cpp

