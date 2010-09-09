// requires ALSAWriter.h

// reuires alsa/asoundlib.h

class ALSAWriterImpl : public ALSAWriter
{
	public:
	// returns the frame size to use
	int Init( int samplerate, int channels, int frames );
	int Write( char * data,  int num_frames );

	private: 
	snd_pcm_t *handle;
	int samplerate;
	int channels;
	int dir; // dont know what this is for
	snd_pcm_uframes_t frames;
  	snd_pcm_hw_params_t *params;
	int size;
	int rc;
};

