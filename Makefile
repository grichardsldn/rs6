ROOT=.

DEVICE_OBJECTS=\
	$(ROOT)/dev_testdevice/testdevice.o\
	$(ROOT)/dev_stepseq/stepseq.o\
	$(ROOT)/dev_seq/seq.o\
	$(ROOT)/dev_seq/notelist.o\
	$(ROOT)/dev_seq/pattern.o\
	$(ROOT)/dev_midirx/midirx.o\
	$(ROOT)/dev_midirx/midi.o\
	$(ROOT)/dev_strings/strings.o\
	$(ROOT)/dev_strings/delay.o\
	$(ROOT)/dev_mixer/mixer.o\
	$(ROOT)/dev_mixer/vol.o\
	$(ROOT)/dev_sub/sub.o\
	$(ROOT)/dev_sub/comp.o\
	$(ROOT)/dev_recorder/recorder.o\
	$(ROOT)/dev_samp/samp.o\
	$(ROOT)/dev_sampdrum/sampdrum.o\
	$(ROOT)/dev_smodel/smodel.o\
	$(ROOT)/dev_sub102/sub102.o\
	$(ROOT)/dev_sub102/sub102comp.o\
	$(ROOT)/dev_reva/reva.o\
	$(ROOT)/dev_dist1/dist1.o\
	$(ROOT)/dev_shaper1/shaper1.o\
	$(ROOT)/ui/Panel.o\
	$(ROOT)/ui/PanelImpl.o\
	$(ROOT)/ui/widget.o

SOURCES=Makefile\
	IDevice.cpp\
	create_device.cpp\
	RS7Mapper.cpp\
	script.cpp\
	settings.cpp\
	dbwidgetset.cpp\
	reader.cpp\
	ALSAWriter.cpp\
	ALSAWriterImpl.cpp\
	main.cpp

OBJECTS=\
	IDevice.o\
	create_device.o\
	RS7Mapper.o\
	setsound.o\
	script.o\
	settings.o\
	dbwidgetset.o\
	reader.o\
	main.o \
	ALSAWriter.o\
	ALSAWriterImpl.o\
	darkbat/dkb_api.o \
	darkbat/UDPSocket.o

LIBS=-lpthread -lasound

TARGETS=main

CFLAGS=-Wall  -O4

%.o: %.cpp Makefile
	gcc -c $(CFLAGS) $*.cpp

all: $(TARGETS)
	
clean:
	rm *.o $(TARGETS)

main: Makefile $(OBJECTS) $(DEVICE_OBJECTS)
	g++ -o main $(OBJECTS) $(DEVICE_OBJECTS) $(LIBS)

depend:
	makedepend $(SOURCES)

# DO NOT DELETE

