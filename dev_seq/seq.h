
//#include "pattern.h"
//#include "reader.h"

#define NUM_PATTERNS  (20)
#define NUM_CHANNELS  (10)
#define NUM_SECTIONS (10)

class Pattern;
class Reader;
class NoteList;

class ChannelSection
{
	public:
	char name[1024];
	int midi_channel;
	Pattern* patterns[NUM_PATTERNS];
	ChannelSection( char *name);
	int transposes[NUM_PATTERNS];
		
	void ProcessChannelSection( Reader *reader );
	private:
	void AddPattern( Pattern *a_pattern, int transpose );
};



class Section
{
	public:
	char name[1024];
	ChannelSection *channels[NUM_CHANNELS];
	Section(char *name);
	void AddChannel( ChannelSection *channel );
	void ProcessSection( Reader *reader );
};

class SequenceProcessor
{
	public:
	SequenceProcessor( char *filename, NoteList *notelist, int samplerate, int tempo );

	private:
	void AddSection( Section *a_section );
	Section * sections[NUM_SECTIONS];
	int RunSection( char *name, int startpoint, NoteList *notelist, int samplerate, int tempo  );
};


class NoteList;

class DeviceSeq : public IDevice
{
	public:

	// IDevice stuff
	virtual void Init( 	IDeviceEvents *event,
				char *instance_name, 
				int samplerate,
				char *startup_params );
	virtual void Clock();
	virtual bool TransportPlay();
	virtual bool TransportStop();
	virtual bool TransportReset();
	DeviceSeq();

	private:
	IDeviceEvents *events;
	int samplerate;
	int tempo;
	NoteList *notelist;
	bool running;
};

