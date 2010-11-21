OBJECTS=notelist.o main.o pattern.o seq.o reader.o seq_timemap.o

CFLAGS=-g

all: $(OBJECTS)

main: Makefile notelist.o main.o pattern.o seq.o seq_timemap.o
	g++ -o main notelist.o main.o pattern.o seq.o seq_timemap.o

clean:
	rm main $(OBJECTS)

depend:
	makedepend *.cpp

%.o: %.cpp Makefile
	gcc -c $(CFLAGS) $*.cpp

# DO NOT DELETE
