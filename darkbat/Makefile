OBJECTS=UDPSocket.o dkb_api.o dkbtest.o

CFLAGS=-g

all: $(OBJECTS) main

main: ${OBJECTS} Makefile
	g++ -o main ${OBJECTS} -lpthread
clean:
	rm main $(OBJECTS)

depend:
	makedepend *.cpp

%.o: %.cpp Makefile
	g++ -c $(CFLAGS) $*.cpp

# DO NOT DELETE
