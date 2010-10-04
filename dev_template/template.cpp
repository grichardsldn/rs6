#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../IDevice.h"


#include "../settings.h"
#include "template.h"

void DeviceTemplate::Init( 	IDeviceEvents *event,
				const char *instance_name, 
				int a_samplerate,
				const char *startup_params )
{
}


bool DeviceTemplate::SetMidiInput( const char *input_name, int channel )
{
	return true;
}

bool DeviceTemplate::SetMidiOutput( const char *output_name, int channel )
{
	return true;
}


bool DeviceTemplate::SetOutput( const char *output_name, int *output_ptr )
{
	assert( output_ptr );
	return true;
}

bool DeviceTemplate::SetInput( const char *input_name, int *input_ptr )
{
	assert( input_ptr );
}

DeviceTemplate::DeviceTemplate()
{
}

void DeviceTemplate::Clock()
{
	//	*output = ((int)(val*(16.0*256.0)) << 16);
}

void DeviceTemplate::MidiNoteOn( int channel, int note, int vol )
{
}

void DeviceTemplate::MidiNoteOff( int channel, int note )
{
}



