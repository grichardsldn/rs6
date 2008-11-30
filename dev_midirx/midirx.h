
class NoteList;


class MIDIReader;

class DeviceMidiRX : public IDevice
{
	public:

	// IDevice stuff
	virtual void Init( 	IDeviceEvents *event,
				char *instance_name, 
				int samplerate,
				char *startup_params );
	virtual void Clock();

	DeviceMidiRX();

	private:
	IDeviceEvents *events;
	char instance_name[1024];

	MIDIReader *midi_reader;
	int midi_in;
};

