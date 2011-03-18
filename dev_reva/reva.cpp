#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../IDevice.h"


#include "../settings.h"
#include "reva.h"


#define DELAY_NUM_DELAYS (100)

// Things to do:
// high pass filter on input
// low pas filter on input
// morphing + rate
// predelay
// spread of delays
// feedback?
// write out the delays?
// option to randomise the delays
// with an undo? to last save? A-B button?
//   or just a load of presets? (chosen at random and saved if good)
// option to morp plus morph rate
// clip checking
// IMPROVE PERFORMANCE


class RevADelay
{
	public:
	
	RevADelay( int buffer_size );

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
	

RevADelay::RevADelay( int a_buffer_size )
{
	assert( a_buffer_size > 0);

	buffer_size = a_buffer_size;

	buffer = new int[buffer_size];

	assert( buffer );
	
	num_delays = 0;

	write_point = buffer_size;

	for( int i = 0 ; i < DELAY_NUM_DELAYS ; i++ )
	{
		delays[i] = (int *)0xbbbbbbbb;
		amounts[i] = (signed short *)0xcccccccc;
		tags[i] = 0xdddddddd;
		alloced[i] = false;
	}
}


int RevADelay::Clock( int input )
{
	if( input > 32700 ) input=32700;
	if( input < -32700 ) input=-32700;

	buffer[write_point] = input;
	write_point --;
	if( write_point <0 )
	{
		write_point = buffer_size;
	}	

	int outval = 0;

	int delays_pending = num_delays;

	for( int i = 0 ; i < DELAY_NUM_DELAYS ; i++)
	{
		if( alloced[i] == true)
		{
			outval += Read( i );
			delays_pending --;

			if( delays_pending == 0 )
			{	
				break;
			}
		}
	}

	return outval;
}

int RevADelay::Read( int i )
{
	//assert( alloced[i] == true );

	//if( ( *delays[i] >= 0 ) && ( *delays[i] < buffer_size ) )
	{
		int readpoint = write_point + *delays[i];
		int val = buffer[readpoint % buffer_size];

		val *= *amounts[i];
		val /= 0x7fff; 

		return val;
	}
	//else
	//{
///		printf("GDR: Sample out of range\n");
//	}
}

bool RevADelay::AddDelay( int tag, int *samples, signed short *amount )
{
	assert( samples );
	assert( amount );

	int i = -1;
	
	do
	{
		i++;
	} while( alloced[i] == true );

	delays[i] = samples;
	amounts[i] = amount;
	tags[i] = tag;
	alloced[i] = true;

	num_delays ++;	
		
}

bool RevADelay::RemoveDelay( int tag )
{
	printf("GDR: RemoveDelays not written\n");
	return false;
}

	
	

void DeviceRevA::Init( 	IDeviceEvents *event,
				const char *instance_name, 
				int a_samplerate,
				const char *startup_params )
{
	delay = new RevADelay(100000);
	for( int n = 0 ; n < 17 ; n++)
	{
		delay_times[n] = 2000 + random() % 10000;
		delay_amounts[n] = 5000;
		if( n & 1 ) delay_amounts[n] *= -1;
		delay->AddDelay( n, &delay_times[n], &delay_amounts[n] );
	}
	last = 0;
	tick = 0;
	change = 0;
}


bool DeviceRevA::SetMidiInput( const char *input_name, int channel )
{
	return true;
}

bool DeviceRevA::SetMidiOutput( const char *output_name, int channel )
{
	
	return true;
}


bool DeviceRevA::SetOutput( const char *output_name, int *a_output_ptr )
{
	assert( a_output_ptr );
	output_ptr = a_output_ptr;
	return true;
}



bool DeviceRevA::SetInput( const char *input_name, int *a_input_ptr )
{
	assert( a_input_ptr );
	input_ptr = a_input_ptr;
	return true;
}

DeviceRevA::DeviceRevA()
{
}

void DeviceRevA::Clock()
{
	signed int in = *input_ptr >> 16;
	
	
	//signed int out = delay->Clock( in + (last * 0.5) );
	signed int out = delay->Clock( in );
	last = out;
	*output_ptr = out << 16; 
	//	*output = ((int)(val*(16.0*256.0)) << 16);

	// freaky morphing
	tick++;
	if( tick > 10000 )
	{
		tick = 0;
		change ++;
		if( change > 16 )
		{
			change =0;
		}
		delay_times[change] = 2000 + random() % 10000;
	}
}

void DeviceRevA::MidiNoteOn( int channel, int note, int vol )
{
}

void DeviceRevA::MidiNoteOff( int channel, int note )
{
}



