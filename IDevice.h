
// IDeviceEvents: interface for signalling back to the mapper.
class IDeviceEvents
{
	public:
	virtual void MidiNoteOn( int channel, int note, int volume );
	virtual void MidiNoteOff( int chanel, int note );
};

// IDevice: interface for a device in the RS7 plumber
class IDevice
{
	public:
	virtual void Init( 	IDeviceEvents *event,
				const char *instance_name, 
				int samplerate,
				const char *startup_params );

	// plumbing
	virtual bool SetInput( const char *input_name, int *input_ptr );
	virtual bool SetOutput( const char *output_name, int *output_ptr );

	virtual bool SetMidiInput( const char *input_name, int channel );

	virtual void MidiNoteOn( int channel, int note, int volume );
	virtual void MidiNoteOff( int channel, int note );

	// management
	virtual int ReceiveCommand( 	const char *command,
					int command_length,
					char *response_buffer,
					int response_buffer_length );

	// transport
	virtual bool TransportPlay();
	virtual bool TransportStop();
	virtual bool TransportReset();

	// operation
	virtual void Clock();
};
	

