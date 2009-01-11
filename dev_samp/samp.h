// This is a test device
#define NUM_CHANNELS (8)


class SampleData
{
	public:
	signed short *data;
	int length;
	int start_point;
	int loop_end_point;
	int loop_start_point;
	int release_point;
	int samplerate;
	int note_freq;
	char filename[1024];
	char data_filename[1024];
	bool mono;
	void ReadFile( char *filename );
};

class SampChannel
{
	public:
	SampleData *data;

	int Clock(); // return signed 16 bit data
	void Trigger( SampleData *data, int note, int vol, int samplerate );
	void Release( );
	int getNote();
	SampChannel();
	private:
	float play_point;
	float increment;
	int note;
	int vol;
	int samplerate;
};

class DeviceSamp : public IDevice
{
	public:

	// IDevice stuff
	virtual void Init( 	IDeviceEvents *event,
				char *instance_name, 
				int samplerate,
				char *startup_params );
	virtual bool SetOutput( char *output_name, int *output_ptr );
	virtual void Clock();
	virtual void MidiNoteOn( int channel, int note, int volume );
	virtual void MidiNoteOff( int channel, int note );
	virtual bool SetMidiInput( char *input, int channel );

	DeviceSamp();

	private:
	int midi_channel;
	int *output;
	int next_channel;
	SampChannel channels[NUM_CHANNELS];
	SampleData *data;
	int samplerate;
};

