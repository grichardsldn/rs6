OBJECTS=UDPSocket.o dkb_api.o

CFLAGS=-g

all: $(OBJECTS) main

carpet: ${OBJECTS} carpet.o
	g++ -o carpet carpet.o ${OBJECTS} -pthread

main: ${OBJECTS} Makefile
	g++ -o main ${OBJECTS} -lpthread

clean:
	rm main $(OBJECTS)

depend:
	makedepend *.cpp

%.o: %.cpp Makefile
	g++ -c $(CFLAGS) $*.cpp

# DO NOT DELETE
