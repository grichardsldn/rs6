OBJECTS=notelist.o main.o pattern.o seq.o reader.o

CFLAGS=-g

all: $(OBJECTS) main

main: Makefile notelist.o main.o pattern.o seq.o
	g++ -o main notelist.o main.o pattern.o seq.o

clean:
	rm main $(OBJECTS)

depend:
	makedepend *.cpp

%.o: %.cpp Makefile
	g++ -c $(CFLAGS) $*.cpp

# DO NOT DELETE
