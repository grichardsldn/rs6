#include <stdio.h>
#include <math.h>

#include "../IDevice.h"
#include "testdevice.h"

// This is a test device

void DeviceTestDevice::Init( 	IDeviceEvents *event,
				const char *instance_name, 
				int a_samplerate,
				const char *startup_params )
{
	printf("DeviceTestDevice:Init: name=\"%s\" samplerate=#%d params=\"%s\"\n",
		instance_name, a_samplerate, startup_params );
	count =0;

	through = 0.0;
	increment = 0.0;
	samplerate = a_samplerate;
	midi_channel = 1;
}

bool DeviceTestDevice::SetMidiInput( const char *input_name, int channel )
{
	midi_channel = channel;
	
	return true;
}

bool DeviceTestDevice::SetOutput( const char *output_name, int *output_ptr )
{
	printf("DeviceTestDevice:SetOutput: output_name=\"%s\"\n", output_name );
	outptr = output_ptr;

	return true;
}

DeviceTestDevice::DeviceTestDevice()
{
	val = -120;
}

void DeviceTestDevice::Clock()
{
	through += increment;

	if( through > 1.0 )
	{
		through = -1.0;
	}

	val ++;
	if( val == 120 )
	{
		val = -120;
	}

	if( current_note > 0 )
	{
		if( through < 0.0 )
		{
			*(outptr) = (64 << 24);
		}
		else
		{
			*(outptr) = (-64 << 24 );
		}
	}
}

void DeviceTestDevice::MidiNoteOn( int channel, int note, int vol )
{
	if( channel != midi_channel )
	{
		// not for us
		return;
	}
	printf("TestDevice::MidiNoteOn( channel=#%d, note=#%d\n",
		channel, note );
	current_note = note;

	float freq;
	
	note += 2;

	freq = 8.1758 * pow(2.0, (double)(note/12.0) );

	printf("freq=%f\n", freq);
	
	increment = (float) freq / (float)samplerate * 2.0;
	printf("samplerate=#%d\n", samplerate);
	printf("increment=%f\n", increment );
}

void DeviceTestDevice::MidiNoteOff( int channel, int note )
{
	if( channel != midi_channel )
	{
		// not for us
		return;
	}

	printf("TestDevice::MidiNoteOff( channel=#%d, note=#%d\n",
		channel, note );

	if( note==current_note )
	{
		current_note = -1;
	}
}


