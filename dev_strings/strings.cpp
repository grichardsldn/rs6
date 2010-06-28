#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "../IDevice.h"
#include "strings.h"
#include "delay.h"

// This is a test device

void DeviceStrings::Init( 	IDeviceEvents *event,
				const char *instance_name, 
				int a_samplerate,
				const char *startup_params )
{
	printf("DeviceStrings:Init: name=\"%s\" samplerate=#%d params=\"%s\"\n",
		instance_name, a_samplerate, startup_params );
	count =0;

	through = 0.0;
	increment = 0.0;
	samplerate = a_samplerate;
	midi_channel = 1;
}

bool DeviceStrings::SetMidiInput( const char *input_name, int channel )
{
	midi_channel = channel;
	
	return true;
}

bool DeviceStrings::SetOutput( const char *output_name, int *output_ptr )
{
	printf("DeviceTestDevice:SetOutput: output_name=\"%s\"\n", output_name );
	outptr = output_ptr;

	return true;
}
void DeviceStrings::AddDelay( int a_delay, signed short a_amount )
{
	int *delayp = new int;
	signed short *amountp = new signed short;

	*delayp = a_delay;
	*amountp = a_amount;
	
	delay->AddDelay( 0, delayp, amountp );
}

DeviceStrings::DeviceStrings()
{
	val = -120;
	note_through = 0;	
	current_note = 0;

	delay = new Delay(100000);

	AddDelay( 1, 0x1fff);

	for (int i = 0 ; i < 5 ; i ++ )
	{
		AddDelay( random() % 10000, 0x1fff );
	}

	mod_int = 0;
	mod_amount = 0x1fff;
	delay->AddDelay( 0, &mod_int, &mod_amount );
}

void DeviceStrings::Clock()
{
	int outval = 0;

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
		float val2;
		if( current_note % 2 )
		{
			val2 = 64.0 * through;
		}
		else
		{
			val2 =-64 * through;
		}
		outval = ( ((int) val2) << 24);

		note_through++;
		mod_int = note_through / 100;
	}

	int from_clock = delay->Clock( outval );
	

	int diff = from_clock - slew;
	diff /= (note_through / 1000 ) + 1;
	slew += diff;
	*outptr = slew;


}

void DeviceStrings::MidiNoteOn( int channel, int note, int vol )
{
	if( channel != midi_channel )
	{
		// not for us
		return;
	}

	printf("Strings::MidiNoteOn( channel=#%d, note=#%d\n",
		channel, note );
	current_note = note;

	float freq;
	
	note += 2;

	freq = 8.1758 * pow(2.0, (double)(note/12.0) );

	printf("freq=%f\n", freq);

	note_through = 0;
	
	increment = (float) freq / (float)samplerate * 2.0;
	printf("samplerate=#%d\n", samplerate);
	printf("increment=%f\n", increment );
}

void DeviceStrings::MidiNoteOff( int channel, int note )
{
	if( channel != midi_channel )
	{
		// not for us
		return;
	}

	printf("Strings::MidiNoteOff( channel=#%d, note=#%d\n",
		channel, note );

	if( note==current_note )
	{
		current_note = -1;
	}
}



