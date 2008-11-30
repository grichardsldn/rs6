// This is a test device

#define NUM_INPUTS (10)

class DeviceMixer : public IDevice
{
	public:

	// IDevice stuff
	virtual void Init( 	IDeviceEvents *event,
				char *instance_name, 
				int samplerate,
				char *startup_params );
	virtual bool SetOutput( char *output_name, int *output_ptr );
	virtual bool SetInput( char *input_name, int *input_ptr );
	virtual void Clock();

	DeviceMixer();

	private:
	int *inputs[NUM_INPUTS];
	int *out_left;
	int *out_right;
};

