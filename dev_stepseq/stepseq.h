// This is a test device


class DeviceStepSeq : public IDevice
{
	public:

	// IDevice stuff
	virtual void Init( 	IDeviceEvents *event,
				char *instance_name, 
				int samplerate,
				char *startup_params );
	virtual bool SetOutput( char *output_name, int *output_ptr );
	virtual void Clock();

	DeviceStepSeq();

	private:
	IDeviceEvents *events;

	int subdiv;  // which sub-division
	int div_len;  // length of a sub-division
	int div_left;  // how much sub-division is left
	int msr; // num subdivision on out of 16

	int array[64]; // the note data
	int pos; // how far through the array we are 
	int num_notes;  // number of notes in the array
	int current_note ;  // the current note value

	int clock_count; // not needed
};

