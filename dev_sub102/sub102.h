class ToneGen;
class LFO;
class ADSR;
class Slewer;

class DeviceSub102 : public IDevice
{
	public:

	// IDevice stuff
	virtual void Init( 	IDeviceEvents *event,
				const char *instance_name, 
				int samplerate,
				const char *startup_params );
	virtual bool SetOutput( const char *output_name, int *output_ptr );
	virtual void Clock();
	virtual void MidiNoteOn( int channel, int note, int volume );
	virtual void MidiNoteOff( int channel, int note );
	virtual bool SetMidiInput( const char *input, int channel );

	DeviceSub102();

	private:
	Settings* settings;
	Sub102::ToneGen *tonegen;
	Sub102::ToneGen *sub_tonegen;
	Sub102::ToneGen *noise_tonegen;
	Sub102::ADSR *amp_adsr;
	Sub102::LFO	*pwm_lfo;
	Sub102::Slewer *slewer;
	int midi_channel;
	int note;
	int *output;
	int current_note;
	double pwm;
	double pitch_mod;
	double filt_level;
	double filt_env;
	double filt_lfo;
	double sub_vol;	
	double noise_vol;

	bool running;

	int octave_adjust;
	void CopyParams();
	int amp_attack_setting;
	int amp_decay_setting;
	int amp_sustain_setting;
	int amp_release_setting;
	int pitch_mod_setting;
	int pwm_setting;

	int filt_level_setting;
	int filt_env_setting;
	int filt_lfo_setting;

	int sub_vol_setting;
	int noise_vol_setting;	
};

