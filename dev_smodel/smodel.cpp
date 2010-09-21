#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../IDevice.h"
#include "smodel.h"

#include "../darkbat/dkb.h"
#include "../ui/widget.h"

void DeviceSModel::Init( 	IDeviceEvents *event,
				const char *instance_name, 
				int a_samplerate,
				const char *startup_params )
{
	printf("DeviceSModel:Init: name=\"%s\" samplerate=#%d params=\"%s\"\n",
		instance_name, a_samplerate, startup_params );

	for( int i = 0 ; i < SMODEL_NUM_NODES ; i ++ )
	{
		pos[i] = 0.0;
		vel[i] = 0.0;
	}
	output = NULL;
	running = true;
}

bool DeviceSModel::SetMidiInput( const char *input_name, int channel )
{
	//return false;
	if( strcmp( input_name, "main" ) == 0)
	{
		midi_channel = channel;
		return true;
	}
	else return false;
}

bool DeviceSModel::SetOutput( const char *output_name, int *output_ptr )
{
	printf("SModel: set output to &%08x\n", (unsigned int)output_ptr);
	output = output_ptr;
	return true;
}

DeviceSModel::DeviceSModel()
{
	running = false;
}

void DeviceSModel::Clock()
{
	if( running == false ) return;

	for( int i = 1 ; i < (SMODEL_NUM_NODES - 1) ; i++)
	{
		double diff = pos[i-1] - pos[i];
		diff += (pos[i+1] - pos[i]);
		//if( pos[i] != 0 ) printf("GDR: diff nonerp\n");	
		double mass = (700.0 * ((double) i / 10.0)) + 1000.0;
		vel[i] += (diff / mass );
	vel[i] = (vel[i] * 100.0 ) / 101.30;

		//vel[i] = (vel[i] * 100.0 ) / 118.30;
		//if( vel[i] > 0.0 )
		//{
	//		vel[i] -= 5.0;
	//		if( vel[i]< 0.0 ) vel[i] = 0.0;
	//	} else {
	//		vel[i] += 5.0;
	//		if( vel[i] > 0.0 ) vel[i] = 0.0;
	//	}
		
	}
	for( int i = 1 ; i < (SMODEL_NUM_NODES - 1) ; i++)
	{
		pos[i] += vel[i];	
	}

	if( output )
	{	
		//if( pos[SMODEL_NUM_NODES / 2] != 0 ) printf("GDR: nonzer\n");
		*output = ((int)(pos[SMODEL_NUM_NODES / 2]) << 21);
	}
}

void DeviceSModel::MidiNoteOn( int channel, int note, int vol )
{
		printf("SModel:NoteOn\n");
		//pos[1] = 200;
	if( channel == midi_channel )
	{
		printf("SModel:NoteOn _C\n");
		//vel[1 + random()% 4] = 10;
		vel[1 ] = 10;
	}

}

void DeviceSModel::MidiNoteOff( int channel, int note )
{
	printf("DeviceSModel::MidiNoteOff\n");

	if( channel == midi_channel )
	{	
	}
}



