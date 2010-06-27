
#define WAVEFORM_SQUARE (0)
#define WAVEFORM_SINE (1)
#define WAVEFORM_SAW (2)
#define WAVEFORM_TRIANGLE (3)
#define WAVEFORM_PWM (4)
#define WAVEFORM_NOISE (5)

namespace Sub102
{

	class LinearSlewer
	{
		public:
		LinearSlewer( int samplerate );
	
		double Clock(double input,double max_per_ms);
		private:
		double val;
		double samplerate;
	};
	
	class Slewer
	{
		public:
		Slewer( int samplerate );
		
		double Clock( double input, double rate );
		private:
		double val;
		double samplerate;
	};
	
	class ADSR
	{
		public:
		ADSR( int sammplerate);

		double getAttack();
		void setAttack(double val );
		double priv_attack;
		double decay;
		double sustain_level;
		double release;
		bool reset_on_trigger;
		double Clock();
		void Trigger();
		void Release();
		
	
		private:
		
		int state;
		double level;
		double samplerate;	
		double Scale( double diff, double rate );
	};

	class LFO
	{
		public:
		LFO(int samplerate);
		double Clock( );
		void setRate( double rate);
		double getRate();
		private:
		
		double samplerate;
		double rate;	// remembered for getRate()
		double through;
		double increment;
	};
	
	class ToneGen
	{
		public:
		ToneGen( int samplerate );
		void SetWaveform( int waveform );
		void NoteOn( int note );
		void NoteOff();  // probably not needed
		double Clock( double note_offset, double adjust );   
	
		private:
		double through;
		double increment;
		//int through;
		//int sub_through;
		//double delta;
		//double sub_delta;
		int waveform;
		int note;
		int samplerate;
	};

	class Envelope
	{
		public:
		double attack;
		double decay;	
		double sustain_level;
		double release;
		double Clock();
	}		;
}
