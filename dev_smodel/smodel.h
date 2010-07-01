
#define SMODEL_NUM_NODES (10)

class DeviceSModel : public IDevice
{
	public:

	// IDevice stuff
	virtual void Init( 	IDeviceEvents *event,
				const char *instance_name, 
				int samplerate,
				const char *startup_params );
	virtual bool SetOutput( const char *output_name, int *output_ptr );
	virtual void Clock();
	virtual void MidiNoteOn( int channel, int note, int volume );
	virtual void MidiNoteOff( int channel, int note );
	virtual bool SetMidiInput( const char *input, int channel );

	DeviceSModel();

	private:
	bool running;
	int midi_channel;
	int * output;
	double pos[SMODEL_NUM_NODES];	
	double vel[SMODEL_NUM_NODES];	
};

