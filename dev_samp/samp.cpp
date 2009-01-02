#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "../IDevice.h"
#include "samp.h"
#include "../dev_seq/reader.h"

// format
// SAMPLE
// valuename value
// ...
// end
//
// values are:
// filename, start, loop_start, loop_end, release_point
// samplerate, note_freq
void SampleData::ReadFile ( char *a_filename )
{
/*
	strcpy( filename, a_filename );
	Reader reader;
	reader.OpenFile( filename );

BBB Finish this.	
*/
}
int SampChannel::getNote()
{
	return note;
}

SampChannel::SampChannel()
{	
	data = NULL;
}

int SampChannel::Clock()
{
	if( data != NULL )
	{
		//if( note == 0)
		//{
		//	return 0;
		//}

		int val = data->data[(int)play_point] * vol / 256;;

		if( play_point < (float)data->length )
		{
			if( play_point > (float) data->loop_end_point )
			{
				play_point = (float)data->loop_start_point;
			}
			play_point += increment;
		}
		else
		{
			val = 0;	
		}	
		return val;
	}

	return 0;
}

void SampChannel::Trigger( SampleData *new_data, int new_note, int new_vol, int a_samplerate )
{
	samplerate = a_samplerate;
	data = new_data;
	play_point = (float) data->start_point;
	note = new_note;

	// adjust the volum to range between 50 and 100%
	vol = (new_vol / 3 * 2) + 42;;

	float freq;
	

	freq = 8.1758 * pow(2.0, (double)((note +2)/12.0) );

	printf("freq=%f\n", freq);
	
	increment = (float) freq / (float)samplerate * 2.0;
	increment *= 100;
	printf("samplerate=#%d\n", samplerate);
}

void SampChannel::Release()
{
	//play_point = (float)data->length;
	play_point = (float)data->release_point;
	note =0;
}
		
void DeviceSamp::Init( 	IDeviceEvents *event,
				char *instance_name, 
				int a_samplerate,
				char *startup_params )
{
	printf("DeviceSamp:Init: name=\"%s\" samplerate=#%d params=\"%s\"\n",
		instance_name, a_samplerate, startup_params );

	data = new SampleData;
	assert(data !=NULL);
	samplerate = a_samplerate;

	FILE *inptr;
	inptr = fopen("dev_samp/string.raw", "rb");

	assert( inptr != NULL );
	fseek( inptr , 0, SEEK_END );
	int length = ftell(inptr) / 2;
	fseek( inptr, 0, SEEK_SET );

	data->data = new signed short[length];

	data->length = fread( data->data, 2, length, inptr );
	fclose( inptr );
	data->start_point = 1000;
	data->release_point = length;
	data->loop_end_point = length - 200;
	data->loop_start_point =  12000;
	
}

bool DeviceSamp::SetMidiInput( char *input_name, int channel )
{
	midi_channel = channel;
	
	return true;
}

bool DeviceSamp::SetOutput( char *output_name, int *output_ptr )
{
	printf("DeviceTestDevice:SetOutput: output_name=\"%s\"\n", output_name );
	output = output_ptr;

	return true;
}

DeviceSamp::DeviceSamp()
{
	midi_channel = 0;
	output = NULL;
	next_channel = 0;
	data = NULL;
}

void DeviceSamp::Clock()
{
	int val = 0;

	for( int i = 0 ; i < NUM_CHANNELS ; i++)
	{
		val += channels[i].Clock();
	}

	if( output )
	{
		*output = (val * 0xffff );
	}
}

void DeviceSamp::MidiNoteOn( int channel, int note, int vol )
{
	if( channel != midi_channel )
	{
		// not for us
		return;
	}
	printf("DeviceSamp::MidiNoteOn( channel=#%d, note=#%d\n",
		channel, note );

	printf("Triggering channel %d\n", next_channel );
	channels[next_channel].Trigger( data, note, vol, samplerate );

	next_channel ++;
	if( next_channel == NUM_CHANNELS)
	{
		next_channel = 0;
	}	
}

void DeviceSamp::MidiNoteOff( int channel, int note )
{
	if( channel != midi_channel )
	{
		// not for us
		return;
	}

	printf("Samp::MidiNoteOff( channel=#%d, note=#%d\n",
		channel, note );

	// Release the first channel playig that note
	for( int i = 0 ; i < NUM_CHANNELS ; i++)
	{
		printf("ch %d note=%d\n", i, channels[i].getNote() );

		if( channels[i].getNote() == note )
		{
			printf("Clearing note %d\n", note );
			channels[i].Release();
			break;
		}
	}
}


