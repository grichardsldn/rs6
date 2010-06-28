#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "../IDevice.h"
#include <stdio.h>
#include "sampdrum.h"
#include <assert.h>

// 36, 38, 42, 44

/*
class DrumChannel
{
	public:
	signed short *data;
	int current;
	int length;
	void Trigger( );
	bool Load( const char *filename );
	signed short Clock();
};
*/

DrumChannel::DrumChannel()
{
	data = NULL;
	current = -1;
	int length = 0;
}

bool DrumChannel::Load( const char *a_filename )
{
	FILE *inptr;
	inptr = fopen(a_filename, "rb");

	if( inptr == NULL )
	{
		printf("could not open file \"%s\"\n", a_filename );
		return false;
	}
	fseek( inptr, 0, SEEK_END );
	length = ftell( inptr );
	fseek( inptr, 0, SEEK_SET );
	// check even number of bytes
	assert( (length&1) == 0);

	// measure length in words.
	length /= 2;

	data = new signed short[length];

	int ret;
	ret = fread( (void*)data, 2, length, inptr );

	printf("Read %d words, length=%d\n", ret, length );
	// check all the data was loaded in
	assert (ret == length );

	printf("Loaded %s\n", a_filename );
	fclose( inptr );
	return true;
}

void DrumChannel::Trigger( )
{
	current = 0;
}

void DrumChannel::Stop()
{
	current = -1;
}

signed short DrumChannel::Clock()
{
	signed short val = 0;
	if (current != -1 )
	{
		val = data[current];
		current ++;
		if( current >= length )
		{
			current = -1;
		}
	}
	return val;
}

/*
	private:
	DrumChannel *channels[10];
	bool allocated[10];
	int note_numbers[10];
	int *output_ptrs[10];
	*/


DeviceSampDrum::DeviceSampDrum()
{
	initialised = false;
}

void DeviceSampDrum::Init( 	IDeviceEvents *event,
				const char *instance_name, 
				int a_samplerate,
				const char *startup_params )
{
	printf("DeviceSampDrum:Init: name=\"%s\" samplerate=#%d params=\"%s\"\n",
		instance_name, a_samplerate, startup_params );
	DrumChannel *temp;

	temp = new DrumChannel();
	temp->Load("dev_sampdrum/bd48.raw");	
	channels[0] = temp;

	temp = new DrumChannel();
	temp->Load("dev_sampdrum/sd48.raw");	
	channels[1] = temp;

	temp = new DrumChannel();
	temp->Load("dev_sampdrum/rimshot48.raw");	
	channels[2] = temp;

	temp = new DrumChannel();
	temp->Load("dev_sampdrum/hhopen48.raw");	
	channels[3] = temp;

	temp = new DrumChannel();
	temp->Load("dev_sampdrum/hhclosed48.raw");	
	channels[4] = temp;

	initialised = true;
}

bool DeviceSampDrum::SetMidiInput( const char *input_name, int a_channel )
{
	assert( input_name );
	
	channel = a_channel;
	return true;
}

bool DeviceSampDrum::SetOutput( const char *output_name, int *output_ptr )
{
	printf("DeviceSampDrum:SetOutput: output_name=\"%s\"\n", output_name );

	output_ptrs[0] = output_ptr;

	return true;
}


void DeviceSampDrum::Clock()
{
	if( !initialised)
	{
		return;
	}
	int val = 0;

	for (int i= 0 ; i < 5 ; i ++ )
	{
		if( channels[i] )
		{
			val += (int)channels[i]->Clock();
		}
	}
	val *= 0xffff; // convert to 24 bit

	//write all to the same output for the moment
	if( output_ptrs[0] )
	{
		*output_ptrs[0] = val;
	}
}

void DeviceSampDrum::MidiNoteOn( int a_channel, int note, int vol )
{
	printf("SampDrum::MidiNoteOn( channel=#%d, note=#%d\n",
		channel, note );

	if( channel != a_channel )
	{
		//not us
		return;
	}

	if( note == 36 )
	{
		//bassdrum
		channels[0]->Trigger();
	}
	if( note == 38 )
	{
		//snare
		channels[1]->Trigger();
	}

	if( note == 39)
	{
		// rimshot
		channels[2]->Trigger();
	}

	if(note == 42)
	{
		// hihhat open
		channels[3]->Trigger();
	}

	if( note == 44)
	{
		// hihat closed
		channels[4]->Trigger();
		channels[3]->Stop();
	}
	
}

void DeviceSampDrum::MidiNoteOff( int channel, int note )
{

	printf("SampDrum::MidiNoteOff( channel=#%d, note=#%d\n",
		channel, note );
}



