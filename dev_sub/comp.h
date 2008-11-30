
#define WAVEFORM_SQUARE (0)
#define WAVEFORM_SINE (1)
#define WAVEFORM_SAW (2)
#define WAVEFORM_TRIANGLE (3)
#define WAVEFORM_PWM (4)
#define WAVEFORM_NOISE (5)

class LinearSlewer
{
	public:
	LinearSlewer( int samplerate );

	float Clock(float input,float max_per_ms);
	private:
	float val;
	float samplerate;
};

class ADSR
{
	public:
	ADSR( int sammplerate);

	float attack;
	float decay;
	float sustain_level;
	float release;

	float Clock();
	void Trigger();
	void Release();

	private:
	
	int state;
	float level;
	float samplerate;	
	float Scale( float diff, float rate );
};

class LFO
{
	public:
	LFO(int samplerate);
	float Clock( );
	void setRate( float rate);
	float getRate();
	private:
	
	float samplerate;
	float rate;	// remembered for getRate()
	float through;
	float increment;
};

class ToneGen
{
	public:
	ToneGen( int samplerate );
	void SetWaveform( int waveform );
	void NoteOn( int note );
	void NoteOff();  // probably not needed
	float Clock( float note_offset, float adjust );   

	private:
	float through;
	float increment;
	//int through;
	//int sub_through;
	//float delta;
	//float sub_delta;
	int waveform;
	int note;
	int samplerate;
};

class Envelope
{
	public:
	float attack;
	float decay;	
	float sustain_level;
	float release;
	float Clock();
};
