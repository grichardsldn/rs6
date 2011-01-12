class seqTimemap
{
	public:
	int next_location( int current_location, int num_ticks );
	// if beyond the end, imagine there were ticks following it
	// if not at a tick location, return next adjusted by offset
   	// to nearest
	bool load_timingmap( char *filename, int tickgap_or_0 );
	int start_location();
	int end_location();
	int get_tickgap( int location_or_0);

	private:
	int ticks[10000]; // blimey graham, learn how to use STL!
	int last_tick_index;
	int find_nearest_tick( int pos ); // index of nearest tick
	int tickgap;
};
