#include <stdio.h>
#include <assert.h>

#include "notelist.h"

struct NoteListEvent
{
	public:
	NoteListEvent();
	int event_type;	
	int channel;
	int note;
	int vel;
	int location;
	NoteListEvent *next;	
	NoteListEvent *prev;
};

class NoteListPrivate
{
	public:
	NoteListPrivate();
	int current_sample;
	
	NoteListEvent *head;
	NoteListEvent *tail;
	NoteListEvent *current_event;
	NoteListEvent *add_point;
};

NoteListEvent::NoteListEvent()
{
	event_type = 0;
	channel = -1;
	note = -1;
	vel = -1;

	next = NULL;
	prev = NULL;
}	

NoteList::NoteList( )
{
	priv = new NoteListPrivate();	
}

NoteListPrivate::NoteListPrivate()
{
	current_sample = 0;
	add_point = NULL;

	// create empty events to simply the list code
	head = new NoteListEvent();
	head->location = -1;
	tail = new NoteListEvent();
	tail->location = 0x7fffffff;
	head->next = tail;
	head->prev = NULL;
	tail->prev = head;	
	tail->next = NULL;

	current_event = head;
	add_point = head;
}

void NoteList::AddEvent( int event, int channel, int note, int vel,
		int location )
{
	printf("AddEvent: event=#%dm channel=#%d, note=#%d, loc=#%d\n",
		event, channel, note, location );

	// do this the slow way, start from the beginning
	NoteListEvent *cur = priv->head;

	// hunt for the first event past the given location
	while (cur->location < location )
	{
		cur = cur->next;
	}
	assert( cur != NULL );

	// make the new event
	NoteListEvent *new_event = new NoteListEvent();
	new_event->event_type = event;
	new_event->channel = channel;
	new_event->note = note;
	new_event->vel = vel;
	new_event->location = location;

	// this one sits in front of the cursor
	new_event->prev = cur->prev;
	new_event->next = cur;

	assert( new_event->prev );
	new_event->prev->next = new_event;
	cur->prev = new_event;

	// I think thats it
}
	
void NoteList::NextSample()
{
	priv->current_sample ++;
}	

void NoteList::Restart()
{
	priv->current_sample = 0;

	// point to the event past the dummy head
	assert( priv->head );
	priv->current_event = priv->head->next;
}

int NoteList::TimeTillNextEvent()
{
	assert( priv->current_event );
	return priv->current_event->location - priv->current_sample;	
}

int NoteList::GetNextEvent( int *event, 
	int *channel,
	int *note,
	int *vel)
{
	int dummy;
	if( event == NULL )
	{
		event = &dummy;
	}
	if( channel == NULL )
	{
		channel = &dummy;
	}
	if( note == NULL )
	{
		note = &dummy;
	}
	if( vel == NULL )
	{
		vel = &dummy;
	}

	assert( priv->current_event );

	if (priv->current_event->event_type == -1 )
	{
		// end of the list
		return 0;
	}

	*event = priv->current_event->event_type;
	*channel = priv->current_event->channel;
	*note = priv->current_event->note;
	*vel = priv->current_event->vel;

	// move the current event on	
	priv->current_event = priv->current_event->next;

	return 1;
}

	
