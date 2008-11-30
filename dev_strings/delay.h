#define DELAY_NUM_DELAYS (100)

class Delay
{
	public:
	
	Delay( int buffer_size );

	int Clock( int input );  // Returns the sum of the delays

	bool AddDelay( int tag, int *samples, signed short *amount );
	bool RemoveDelay( int tag );

	private:
	int buffer_size;
	int *delays[DELAY_NUM_DELAYS];	
	signed short *amounts[DELAY_NUM_DELAYS];
	int tags[DELAY_NUM_DELAYS];
	bool alloced[DELAY_NUM_DELAYS];
	int *buffer;
	int write_point;
	int num_delays;
		
	int Read( int index );
};
	
	
