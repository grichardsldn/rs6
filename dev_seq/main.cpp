#include <stdio.h>

#include "notelist.h"

int GetEvent( NoteList *nl )
{
	int event;
	int channel;
	int note;
	int vel;

	int ret = nl->GetNextEvent(&event, &channel, &note, &vel );
	
	if( ret  == 1)
	{
		printf("Event: event=#%d, channel=#%d, note=#%d, vel=#%d\n",
			event, channel, note, vel );
	}
	else
	{
		printf("Event: Not event\n");
	}

	return ret;
}
main()
{
	NoteList nl;

	int i;

	nl.AddEvent( NOTELIST_EVENT_NOTE_ON, 0, 24, 0, 10 );	
	nl.AddEvent( NOTELIST_EVENT_NOTE_OFF, 0, 12, 0, 10 );	
	nl.AddEvent( NOTELIST_EVENT_NOTE_ON, 0, 12, 0, 5 );	
	nl.AddEvent( NOTELIST_EVENT_END, 0,0,0, 15 );

	nl.Restart();

	for( int i = 0 ; i <20 ; i ++ )
	{	
		nl.NextSample();
		int ttne = nl.TimeTillNextEvent();
		printf("itteration #%d; TTNE=#%d\n",
			i, ttne );

		if( ttne <= 0 )
		{
			while (nl.TimeTillNextEvent() <= 0)
			{
				GetEvent(&nl );
			}
		}
	}
}

