#include <stdio.h>
#include "midi.h"

#define NO_DEBUG_MIDI

MIDIReader::MIDIReader()
{
	channel = 0;
	note = 0;
	volume = 0;
	
	state = wanting_channel;
}

bool MIDIReader::Input( int byte )
{

#ifdef DEBUG_MIDI
	printf("Received #%d &%02x.\r\n", byte, byte );
#endif
	switch( state )
	{
		case wanting_channel:
		{
			if( (byte & 0xf0) == 0x90)
			{
				channel = (byte & 0x0f) + 1;
				state = wanting_note;
#ifdef DEBUG_MIDI
				printf("Got channel, Now want note.\r\n");
#endif
			}
			else if( byte < 0x7f )
			{
#ifdef DEBUG_MIDI
				printf("Running channel.\r\n");
#endif
				note = byte;
				state = wanting_volume;
			}
		}
		break;

		case wanting_note:
		{
			if( byte < 0x7f )
			{
				note = byte;
				state = wanting_volume;

#ifdef DEBUG_MIDI
				printf("Got note, now want volume.\r\n");
#endif
			}
			else
			{
#ifdef DEBUG_MIDI
				printf("Didnt get a note, falling back to wanting_note.\r\n");
#endif
				channel = byte & 0x0f;
				state = wanting_note;
			}
		}
		break;

		case wanting_volume:
		{
			if( byte <= 0x7f )
			{
				volume = byte;
				state = wanting_channel;
#ifdef DEBUG_MIDI
				printf("MIDIReader: channel=#%d note=#%d volume=#%d.\r\n", channel, note, volume );
#endif

				return true;
			}
			else
			{
				printf("Wanted volume, didnt get one.\r\n");
				state = wanting_channel;
			}
		}
		break;
	}
	return false;
}
