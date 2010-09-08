OBJECTS=notelist.o main.o pattern.o seq.o reader.o

CFLAGS=-g

all: $(OBJECTS)

main: Makefile notelist.o main.o pattern.o seq.o
	g++ -o main notelist.o main.o pattern.o seq.o

clean:
	rm main $(OBJECTS)

depend:
	makedepend *.cpp

%.o: %.cpp Makefile
	gcc -c $(CFLAGS) $*.cpp

# DO NOT DELETE
