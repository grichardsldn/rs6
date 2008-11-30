#include <stdio.h>
#include "../IDevice.h"
#include "mixer.h"
#include <assert.h>
#include <string.h>

void DeviceMixer::Init( IDeviceEvents *event,
			char *instance_name,
			int samplerate,
			char *startup_params)
{
	// dont care about any of these
}
	
void DeviceMixer::Clock()
{
	int val = 0;

	for( int i = 0 ; i < NUM_INPUTS ; i++)
	{
		if( inputs[i] != NULL )
		{	
			val += (*inputs[i] / 2);	
		}
	}
	if( val >2000000000  )	
	{
		val == 2000000000;;
	}
	if( val < -2000000000)
	{
		val = -2000000000;
	}
	/*
	if( val >8388600  )	
	{
		val == 8388600;;
	}
	if( val < 8388600)
	{
		val = -8388600;
	}*/
	*out_left = val;
	*out_right = val;
}


bool DeviceMixer::SetInput( char *input_name, int *input_ptr )
{
	int index = -1;
	for( int i = 0 ; i<  NUM_INPUTS; i++)
	{
		if( inputs[i] == NULL)
		{
			index = i;
			break;
		}
	}
	if (index == -1 )
	{
		printf("DeviceMixer: Too many inputs\n");
		return false;
	}

	inputs[index] = input_ptr;

	return true;
}

bool DeviceMixer::SetOutput( char *output_name, int *output_ptr)
{
	assert( output_name );
	assert( output_ptr );

	if( strcmp( "left", output_name) == 0 )
	{
		out_left = output_ptr;
	}
	else if( strcmp( "right", output_name) == 0 )
	{
		out_right = output_ptr;
	}
	else
	{
		printf("DeviceMixer: Output %s unknown\n",
			output_name);
		return false;
	}
	return true;
}	
		

DeviceMixer::DeviceMixer()
{
	for( int i = 0 ; i < NUM_INPUTS; i++)
	{
		inputs[i] = NULL;
	}
	out_left = NULL;
	out_right = NULL;
}
