#define SHAPER1_NUM_VALUES (10 )

class Settings;

class DeviceShaper1 : public IDevice
{
	public:

	// IDevice stuff
	virtual void Init( 	IDeviceEvents *event,
				const char *instance_name, 
				int samplerate,
				const char *startup_params );
	virtual bool SetOutput( const char *output_name, int *output_ptr );
	virtual bool SetInput( const char *input_name, int *input_ptr );
	virtual void Clock();
	virtual void MidiNoteOn( int channel, int note, int volume );
	virtual void MidiNoteOff( int channel, int note );
	virtual bool SetMidiInput( const char *input, int channel );
	virtual bool SetMidiOutput( const char *output, int channel );

	DeviceShaper1();

	private:
	Settings *settings;
	int x_setting;
	int y_setting;
	int z_setting;
	int lpf_value_settings[ SHAPER1_NUM_VALUES ];
	int lpf_offset_setting;
	int lpf_range_setting;
	int rate_setting;
};

