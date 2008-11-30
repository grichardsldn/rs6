
// requires IManage.h
// required IDevice.h

class RS7Mapper;

class RS7Manage : public IManage
{
	public:

	RS7Manage( RS7Mapper *mapper );

	int CountBusses( );
	int CreateDevice(	char *device_type, 
				char *instance_name, 
				char *params );
	
	int FindDevice( char *device_type, char *instance_name );
	bool DeleteDevice( int device_id );

	bool SetDeviceInput( int device_id, char *input_name, int bus );
	bool SetDeviceMidiInput( int device_id, char *input_name, int channel );
	bool SetDeviceOutput( int device_id, char *output_name, int bus );

	void Monitor( int left_device_id, int right_device_id );

	void TransportPlay();
	void TransportStop();
	void TransportReset();	
	private:
	RS7Mapper *mapper;
};

class Bus
{
	public:
	Bus();
	int reference_count;
	int current_value;
};

#define NUM_BUSSES (64)
#define NUM_DEVICES (32)

class Device;
class RS7Mapper;

class DeviceEventReceiver : public IDeviceEvents
{
	public:
	void MidiNoteOn( int channel, int note, int volume );
	void MidiNoteOff( int channel, int note);

	Device *device;
	RS7Mapper *mapper;
};

struct Device
{
	public:
	Device();
	void Set( IDevice *device_interface,
		char *device_type,
		char *name );
	void Clear();

	public:
	IDevice *device_interface;
	char device_type[1024];
	char name[1024];
	DeviceEventReceiver *event_receiver;  // this stays throught the life of the object
};

class RS7Mapper
{
	public:
	RS7Mapper( int samplerate, char *audio_device );

	IManage *GetControl();

	void BroadcastMidiNoteOn( int channel, int note, int volume );
	void BroadcastMidiNoteOff( int channel, int note );
	
	RS7Manage *manage_interface;
	int samplerate;
	Bus busses[NUM_BUSSES];
	Device devices[NUM_DEVICES];
	FILE *audio_output;
	int left_monitor_bus;
	int right_monitor_bus;

	pthread_t run_thread;
	static void *run_thread_start( void *arg );
	void RunThread();
	void DoFrame(signed short *data_left, signed short *data_right);
};
	
	
