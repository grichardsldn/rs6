#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "../IDevice.h"
#include "samp.h"
#include "../reader.h"

// filename <filename> 
// valuename value
// ... till end of file
//
// values are:
//  start, loop_start, loop_end, release
// samplerate, note_freq
// mono (on its own)
void SampleData::ReadFile ( const char *a_filename )
{
	strcpy( filename, a_filename );
	Reader reader;
	reader.OpenFile( filename );

	loop_start_point = -1;
	loop_end_point = -1;
	release_point = -1;
	samplerate = -1;
	mono = false;

	char *word = NULL;
	while( 1)
	{
		word = reader.NextWord();
		if( strcmp( word, "" ) == 0)
		{
			break;
		}
		else if( strcmp( word, "loop_start" ) == 0)
		{
			loop_start_point = atoi( reader.NextWord() );	
			printf("loop start read %d\n", loop_start_point );
		}
		else if( strcmp( word, "start" ) == 0)
		{
			start_point = atoi( reader.NextWord() );	
			printf("start read %d\n", start_point );
		}
		else if( strcmp( word, "loop_end" ) == 0)
		{
			loop_end_point = atoi( reader.NextWord() );	
			printf("loop end read %d\n", loop_end_point );
		}
		else if( strcmp( word, "release" ) == 0)
		{
			release_point = atoi( reader.NextWord() );	
			printf("release read %d\n", release_point );
			
		}
		else if( strcmp( word, "samplerate" ) == 0)
		{
			samplerate = atoi( reader.NextWord() );	
			printf("read %d\n", samplerate );
			
		}
		else if( strcmp( word, "freq" ) == 0)
		{
			note_freq = atoi( reader.NextWord() );	
			printf("freq %d\n", note_freq);
		}
		else if( strcmp( word, "filename" ) == 0)
		{
			strcpy( data_filename, reader.NextWord() );	
			printf("Reading %s:\n", data_filename );
		}
		else if( strcmp( word, "mono") == 0)
		{
			mono = true;
		}
	}
	
	FILE *inptr;
	inptr = fopen(data_filename, "rb");

	assert( inptr != NULL );
	fseek( inptr , 0, SEEK_END );
	length = ftell(inptr) / 2;
	fseek( inptr, 0, SEEK_SET );

	data = new signed short[length];

	length = fread( data, 2, length, inptr );

	printf("samp: file %s read %d samples.\n", data_filename, length );
	fclose( inptr );

	if( release_point == -1)
	{
		release_point = length;
	}

	if( loop_end_point == -1)
	{
		loop_end_point = length + 1000;
	}

	if( start_point == -1 )
	{
		start_point = 0;
	}

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
	freq *= 440.0 / data->note_freq;
	freq *= (float)data->samplerate / (float)samplerate;
	printf("freq=%f\n", freq);
	
	increment = (float) freq / (float)samplerate * 2.0;
	increment *= 100;
	printf("samplerate=#%d\n", samplerate);
}

void SampChannel::Release()
{
	play_point = (float)data->length;
	//play_point = (float)data->release_point;
	//printf("setting playpoint to %d\n", data->release_point );
	note =0;
}
		
void DeviceSamp::Init( 	IDeviceEvents *event,
				const char *instance_name, 
				int a_samplerate,
				const char *startup_params )
{
	printf("DeviceSamp:Init: name=\"%s\" samplerate=#%d params=\"%s\"\n",
		instance_name, a_samplerate, startup_params );

	data = new SampleData;
	assert(data !=NULL);
	samplerate = a_samplerate;

	data->ReadFile( startup_params );
}

bool DeviceSamp::SetMidiInput( const char *input_name, int channel )
{
	midi_channel = channel;
	
	return true;
}

bool DeviceSamp::SetOutput( const char *output_name, int *output_ptr )
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

	for( int i = 0 ; i < DEVSAMP_NUM_CHANNELS ; i++)
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

	if( data->mono == false )
	{
		next_channel ++;
		if( next_channel == DEVSAMP_NUM_CHANNELS)
		{
			next_channel = 0;
		}	
	}
	else
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
	for( int i = 0 ; i < DEVSAMP_NUM_CHANNELS ; i++)
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


