#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "../IDevice.h"
#include "recorder.h"


void DeviceRecorder::Init( 	IDeviceEvents *event,
				char *instance_name, 
				int a_samplerate,
				char *startup_params )
{
	printf("DeviceRecorder:Init: name=\"%s\" samplerate=#%d params=\"%s\"\n",
		instance_name, a_samplerate, startup_params );

	outfile=fopen(startup_params, "wb");
}

bool DeviceRecorder::SetInput( char *input_name, int *input_ptr )
{
	printf("DeviceTestDevice:SetInput: input_name=\"%s\"\n", input_name );
	input = input_ptr;

	return true;
}

DeviceRecorder::DeviceRecorder()
{
}

void DeviceRecorder::Clock()
{
	if( playing )
	{	
		assert( input != NULL );
		{
			unsigned short val;
			val = (*input) /=0xffff;
			fwrite( &val, 2, 1, outfile );
		}
	}
}

bool DeviceRecorder::TransportPlay()
{
	playing = true;
	return true;
}

bool DeviceRecorder::TransportStop()
{
	playing = false;
	fclose( outfile );
	return false;
}


