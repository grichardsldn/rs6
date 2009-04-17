class ToneGen;
class LFO;
class ADSR;
class Slewer;

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
	Slewer *slewer;
	int midi_channel;
	int note;
	int *output;
	int current_note;
	float pwm;
	float pitch_mod;
	float filt_level;
	float filt_env;
	float filt_lfo;

	bool running;

	void CopyParams();
	VolWidget *amp_attack_widget;
	VolWidget *amp_decay_widget;
	VolWidget *amp_sustain_widget;
	VolWidget *amp_release_widget;
	VolWidget *pitch_mod_widget;
	VolWidget *pwm_widget;

	VolWidget *filt_level_widget;
	VolWidget *filt_env_widget;
	VolWidget *filt_lfo_widget;
	
	
	WidgetPanel *panel;
};

