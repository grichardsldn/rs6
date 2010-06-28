#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "../IDevice.h"
#include "recorder.h"


void DeviceRecorder::Init( 	IDeviceEvents *event,
				const char *instance_name, 
				int a_samplerate,
			const 	char *startup_params )
{
//	printf("DeviceRecorder:Init: name=\"%s\" samplerate=#%d params=\"%s\"\n",
///		instance_name, a_samplerate, startup_params );

//	outfile=fopen(startup_params, "wb");

	head = NULL;
}

bool DeviceRecorder::SetOutput( const char *output_name, int *output_ptr )
{
	printf("DeviceRecorder:SetOutput: output_name=\"%s\"\n", output_name );

	Channel *channel = new Channel();
	strcpy( channel->filename, output_name );
	channel->ptr = fopen(output_name, "rb");
	channel->output = false;
	channel->connection = output_ptr;

	channel->next = head;
	head = channel;

	return true;
}

bool DeviceRecorder::SetInput( const char *input_name, int *input_ptr )
{
	printf("DeviceRecorder:SetInput: input_name=\"%s\"\n", input_name );

	Channel *channel = new Channel();
	strcpy( channel->filename, input_name );
	channel->ptr = fopen(input_name, "wb");
	channel->output = true;
	channel->connection = input_ptr;

	channel->next = head;
	head = channel;

	return true;
}

DeviceRecorder::DeviceRecorder()
{
}

void DeviceRecorder::Clock()
{
	if( playing )
	{	
		Channel *itr = head;
		
		while( itr != NULL)
		{
			if( itr->output == true )
			{
				unsigned short val;
				val = *(itr->connection)/=0xffff;
				fwrite( &val, 2, 1, itr->ptr );
			}
			else
			{
				unsigned short val;
				fread( &val, 2, 1, itr->ptr );
				*itr->connection = val * 0xffff;
			}

			itr = itr->next;
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
	Channel *itr = head;
		
	while( itr != NULL)
	{
		//fclose( itr->ptr );

		itr = itr->next;
	}
	return false;
}

Channel::Channel()
{
	next = NULL;
}



