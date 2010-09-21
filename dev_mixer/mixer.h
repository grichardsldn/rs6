// This is a test device

#define NUM_INPUTS (7)

class VolPanel;
class Settings;

class Panel;

class DeviceMixer : public IDevice
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

	DeviceMixer();

	private:
	int *inputs[NUM_INPUTS];
	int *out_left;
	int *out_right;
	//VolPanel *vol_panel;
	Settings *settings;
	int volumes[NUM_INPUTS];
	int x;
	int y;
	int z;
	Panel *panel;
	void CreatePanel();
};

