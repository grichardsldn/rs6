
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include "midi.h"
#include "../IDevice.h"
#include "midirx.h"


DeviceMidiRX::DeviceMidiRX( )
{
}

void DeviceMidiRX::Init( 	IDeviceEvents *a_event,
				char *a_instance_name, 
				int a_samplerate,
				char *a_startup_params )
{
	assert(a_event);
	assert(a_instance_name);
	assert(a_startup_params);

	events = a_event;
	strcpy( instance_name, a_instance_name );

	midi_reader = new MIDIReader();

	midi_in = open( a_startup_params , O_RDONLY |O_NONBLOCK);

	if( midi_in == -1 )
	{
		printf("MidiRX %s: Cannot open \"%s\"\n", instance_name, a_startup_params );
		return;
	}

	printf("MidiRX %s: Opened %s\n", instance_name, a_startup_params );
}

void DeviceMidiRX::Clock()
{
	int amount_read ;
	do
	{
		unsigned char buf;
		amount_read = read( midi_in, &buf, 1 );
	
		if( amount_read > 0 )
		{
			printf("MidiRX %d: Got 0x%02x\n", instance_name, buf );
	
			bool was_data;
			was_data = midi_reader->Input((int)buf );

			if( was_data )
			{
				// split keyboard
				//if( midi_reader->note > 58 ) midi_reader->channel ++;

				printf("MidiRX %s: Got note #%d\n", instance_name, midi_reader->note );
				if( midi_reader->volume == 0 )
				{
					events->MidiNoteOff( midi_reader->channel,
							midi_reader->note);
				}
				else
				{
					events->MidiNoteOn( midi_reader->channel,
							midi_reader->note,
							midi_reader->volume );
				}
			}
		}
	} while (amount_read > 0 );

}

