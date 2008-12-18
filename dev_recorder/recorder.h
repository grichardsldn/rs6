// This is a test device
// requires stdio.h

class DeviceRecorder : public IDevice
{
	public:

	// IDevice stuff
	virtual void Init( 	IDeviceEvents *event,
				char *instance_name, 
				int samplerate,
				char *startup_params );
	virtual bool SetInput( char *input_name, int *input_ptr );
	virtual void Clock();
	virtual bool TransportPlay();
	virtual bool TransportStop();

	DeviceRecorder();

	private:
	char filename[1024];
	bool playing;
	int *input;
	FILE *outfile;
};

