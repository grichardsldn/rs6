class ToneGen;
class LFO;
class ADSR;
class LinearSlewer;

class VolWidget;
class WidgetPanel;

class DeviceSub : public IDevice
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

	DeviceSub();

	private:
	ToneGen *tonegen;
	ToneGen *mod_tonegen;
	ADSR *amp_adsr;
	LFO	*pwm_lfo;
	LinearSlewer *slewer;
	int midi_channel;
	int note;
	int *output;
	int current_note;
	float pwm;
	float pitch_mod;
	bool running;

	void CopyParams();
	VolWidget *amp_attack_widget;
	VolWidget *amp_decay_widget;
	VolWidget *amp_sustain_widget;
	VolWidget *amp_release_widget;
	VolWidget *pitch_mod_widget;
	VolWidget *pwm_widget;
	
	
	WidgetPanel *panel;
};

