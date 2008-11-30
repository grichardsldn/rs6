// This is a test device


class DeviceTestDevice : public IDevice
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

	DeviceTestDevice();

	private:
	// local stuff
	int val;
	int count;
	int *outptr;
	int current_note;
	float increment;
	float through;
	int samplerate;
	int midi_channel;
};

