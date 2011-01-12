#include <stdio.h>
#include "seq_timemap.h"
#include "../reader.h"
#include <string.h>
#include <stdlib.h>


// if beyond the end, imagine there were ticks following it
// if not at a tick location, return next adjusted by offset
// to nearest
// XXX doesnt cope with fallig off the end yet
int seqTimemap::next_location( int current_location, int num_ticks )
{
	printf("next_location: current=%d num_ticks=%d\n",
		current_location, num_ticks );

	int ret = 0;
	int thistick = find_nearest_tick( current_location );
	if(( thistick + num_ticks ) < last_tick_index )
	{
		ret = ticks[thistick + num_ticks];
	}
	printf("next_location: returning %d\n", ret );
	return ret;
}

bool seqTimemap::load_timingmap( char *filename, int tickgap_or_0 )
{
	printf("load_timingmap: %s\n", filename );
	bool ok = false;
	tickgap = tickgap_or_0;

	Reader reader;
        reader.OpenFile( filename );

	char *word;
	int tickno = 0;
	while( strcmp(word = reader.NextWord(), "" ) != 0 )
	{
		int tickpos = atoi( word );
		ticks[tickno++] = tickpos;
	}		
	last_tick_index = tickno - 1;
	if( last_tick_index < 1 ) {
		ok = false;
	}
	printf("loaded %d ticks\n", last_tick_index );
	return ok;
}

int seqTimemap::start_location()
{
	return ticks[0];	
}

int seqTimemap::end_location()
{
	return ticks[last_tick_index];
}

int seqTimemap::get_tickgap( int location_or_0)
{
	int tick = find_nearest_tick( location_or_0 );	
	if ( tick == last_tick_index )
	{
		return 0;
	}

	return ticks[last_tick_index + 1] - ticks[last_tick_index];
}

int seqTimemap::find_nearest_tick( int pos )
{
	printf("find_nearest_tick: pos=%d\n", pos );
	int nearest_dist = ticks[last_tick_index];
	if( pos > nearest_dist )
	{
		nearest_dist = pos;
	}	
	int nearest_tick = -1;
	for( int i = 0 ; i < (last_tick_index - 1) ; i++)
	{
		int dist = ticks[i] - pos;
		if( dist < 0 )
		{
			dist *= -1;
		}		
		if( dist < nearest_dist )	
		{
			nearest_dist = dist;
			nearest_tick = i;
		}
	}
	printf("  nearest_tick=%d\n", nearest_tick);
	printf("find_nearest_tick: returning %d\n", nearest_tick );
	return nearest_tick;
}
