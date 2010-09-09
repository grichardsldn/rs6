
class ALSAWriter
{
	public:
	// returns the frame size to use
	virtual int Init( int samplerate, int channels, int frames ) = 0 ;
	virtual int Write( char * data,  int num_frames ) = 0;
	
	static ALSAWriter * CreateALSAWriter();
};

