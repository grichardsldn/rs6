#include <stdio.h>
#include <assert.h>
#include "delay.h"

Delay::Delay( int a_buffer_size )
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


int Delay::Clock( int input )
{
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

int Delay::Read( int i )
{
	//assert( alloced[i] == true );

	//if( ( *delays[i] >= 0 ) && ( *delays[i] < buffer_size ) )
	{
		int readpoint = write_point + *delays[i];
		int val = buffer[readpoint % buffer_size];

		val /= 0x7fff; 
		val *= *amounts[i];

		return val;
	}
	//else
	//{
///		printf("GDR: Sample out of range\n");
//	}
}

bool Delay::AddDelay( int tag, int *samples, signed short *amount )
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

bool Delay::RemoveDelay( int tag )
{
	printf("GDR: RemoveDelays not written\n");
	return false;
}

	
	
