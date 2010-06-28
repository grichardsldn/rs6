#include <stdio.h>
#include <assert.h>

#include "IDevice.h"

void IDeviceEvents::MidiNoteOn( int channel, int note, int volume )
{
	//printf("MidiNoteOn( channel=#%d, note=#%d, volume=#%d\n",
//		channel, note, volume );
}

void IDeviceEvents::MidiNoteOff( int channel, int note )
{
	//printf("MidiNoteOff( channel=#%d, note=#%d)\n",
//		channel, note );
}

void IDevice::Clock()
{
}

int IDevice::ReceiveCommand( const	char *command,
				int command_length,
				char *response_buffer,
				int response_buffer_length )
{
	assert( command );
	assert( response_buffer );

	return 0;
}

bool IDevice::SetMidiInput( const char *input_name, int channel )
{
	return false;
}

void IDevice::MidiNoteOn( int channel, int note, int volume )
{
}

void IDevice::MidiNoteOff( int channel, int note )
{
}

bool IDevice::TransportPlay()
{
	return false;
}

bool IDevice::TransportStop()
{
	return false;
}

bool IDevice::TransportReset()
{
	return false;
}

bool IDevice::SetOutput( const char *output_name, int *output_ptr )
{
	assert( output_name );
	assert( output_ptr );

	return false;
}	

void IDevice::Init( 	IDeviceEvents *event_receiver,
			const char *instance_name,
			int samplerate,
			const char *startup_params )
{
	assert( event_receiver );
	assert( instance_name );
	assert( samplerate > 0 );
	assert( startup_params );
}

bool IDevice::SetInput( const char *input_name, int *input_ptr )
{
	assert( input_name );
	assert( input_ptr );
	return false;
}


