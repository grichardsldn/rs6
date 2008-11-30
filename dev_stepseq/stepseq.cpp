#include <stdio.h>
#include <assert.h>

#include "../IDevice.h"
#include "stepseq.h"

void DeviceStepSeq::Init( 	IDeviceEvents *event,
				char *instance_name, 
				int samplerate,
				char *startup_params )
{
	printf("DeviceStepSeq:Init: name=\"%s\" samplerate=#%d params=\"%s\"\n",
		instance_name, samplerate, startup_params );

	assert( event );
	events = event;
}

bool DeviceStepSeq::SetOutput( char *output_name, int *output_ptr )
{
	printf("DeviceStepSeq::SetOutput: returning fail\n");

	return false;
}

DeviceStepSeq::DeviceStepSeq()
{
	subdiv = 0;
	msr = 10;
	div_len = 500;
	div_left = 1;

	array[0] = 12;
	array[1] = 12;
	array[2] = 24;
	array[3] = 24;
	num_notes = 4;

	pos = num_notes -1;
	current_note = array[pos];
}

void DeviceStepSeq::Clock()
{
	//clock_count ++;
	//if(( clock_count % 8000) == 0 )
//	{
	//	printf("8000 clocks\n");
	//}

	div_left --;
	if( div_left <= 0 )
	{
		printf("GDR: Subdiv\n");
		// subdiv
		div_left = div_len;

		subdiv ++;
		if( subdiv == 16 )
		{
			subdiv = 0;
			// first subdivision, note on
			pos ++;
			if( pos >= num_notes)
			{
				pos = 0;
			}

			assert( pos < 63 );
			current_note = array[pos];

			assert(events );	
			events->MidiNoteOn(1, current_note, 128 );
		}
		if( subdiv == msr )
		{
			// note off 
			assert( events );
			events->MidiNoteOff( 1, current_note );
		}
	}
}
