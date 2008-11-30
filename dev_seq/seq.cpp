#include <stdio.h>
#include <assert.h>
#include "../IDevice.h"
#include "notelist.h"
#include "pattern.h"
#include "reader.h"
#include "seq.h"
#include <string.h>
#include <stdlib.h>


void ChannelSection::ProcessChannelSection( Reader *reader )
{
	assert( reader );

	char *word = reader->NextWord();

	if( word == NULL )
	{
		printf("Channel section: unexpected eof at line %d\n", reader->CurrentLine());
		return;
	}

	if( strcmp( word, "midi") != 0 )
	{
		printf("Channel section: expected \"midi\", got \"%s\"\n", word);
		return;
	}

	midi_channel = atoi( reader->NextWord() );
	if( midi_channel == 0 )
	{
		printf("Invalid midi channel at line %d\n", reader->CurrentLine());
		return;
	}

	printf("GDR: midi channel is #%d\n", midi_channel );

	// now the opening brace
	word = reader->NextWord();
	if( word == NULL )
	{
		printf("Unexpected eof at line %d\n", reader->CurrentLine());
		return;
	}

	if( strcmp( word, "{") != 0 )
	{
		printf("Open brace expected at line %d\n", reader->CurrentLine() );
		return;
	}

	for (;;)
	{
		int transpose = 0;

		word = reader->NextWord();
		if( word == NULL )
		{
			printf("Unexpected eof at line %d\n", reader->CurrentLine());
			return;
		}

		if( strcmp( word, "}" ) == 0 )
		{
			break;
		}

		// otherwise
		Pattern *p = new Pattern();
		p->Load( word );

		word = reader->NextWord();
		if( word == NULL )
		{
			printf("Unexpected eof at line %d\n", reader->CurrentLine());
			return;
		}

		if( strcmp( word, "}" ) == 0 )
		{
			break;
		}

		if( strcmp( word, "+" ) == 0 )
		{
			word = reader->NextWord();
			if( word == NULL )
			{
				printf("Unexpected eof at line %d\n", reader->CurrentLine());
				return;
			}

			transpose = atoi(word);
			word = reader->NextWord();
			if( word == NULL )
			{
				printf("Unexpected eof at line %d\n", reader->CurrentLine());
				return;
			}

		}
		if( strcmp( word, "-" ) == 0 )
		{
			word = reader->NextWord();
			if( word == NULL )
			{
				printf("Unexpected eof at line %d\n", reader->CurrentLine());
				return;
			}

			transpose = atoi(word) * -1;
			word = reader->NextWord();
			if( word == NULL )
			{
				printf("Unexpected eof at line %d\n", reader->CurrentLine());
				return;
			}

		}



		if( strcmp( word, "," ) != 0 )
		{
			printf("Comma expected at line %d\n", reader->CurrentLine());
			return;
		}

		AddPattern( p, transpose );
		printf("GDR: Added Pattern\n");
	}

	printf("GDR: Got to end brace\n");
}

ChannelSection::ChannelSection(char *a_name)
{
	assert( a_name );
	strcpy( name, a_name );
	for( int i = 0 ; i < NUM_PATTERNS ; i++)
	{
		patterns[i] = NULL;
		transposes[i] = 0;
	}

	midi_channel = -1;
}

void ChannelSection::AddPattern (Pattern *a_pattern, int transpose )
{
	bool found = false;
	for( int i = 0 ; i < NUM_PATTERNS ; i ++ )
	{
		if( patterns[i] == NULL)
		{
			patterns[i] = a_pattern;
			transposes[i] = transpose;
			found = true;
			break;
		}
	}
	assert( found );
}

Section::Section( char *a_name )
{
	assert( a_name );
	strcpy( name, a_name );
	for( int i = 0 ; i < NUM_CHANNELS ; i ++ )
	{
		channels[i] = NULL;
	}
}

void Section::AddChannel( ChannelSection *a_channel )
{
	assert( a_channel );
	bool found = false;
	for( int i = 0 ; i < NUM_CHANNELS ; i++)
	{
		if( channels[i] == NULL )
		{
			channels[i] = a_channel;
			found = true;
			break;
		}
	}
	assert( found = true );
}

void Section::ProcessSection( Reader *reader )
{
	assert( reader );

	for (;; )
	{
		char *word = reader->NextWord();
		if( word == NULL )
		{
			printf("Unexpected eof at line %d\n", reader->CurrentLine());
			return;
		}

		if( strcmp( word, "end" ) == 0 )
		{
			printf("GDR: ProcessSerion: hit end\n" );
			break;
		}

		if( strcmp( word, "channel" ) != 0 )
		{
			printf("\"channel\" expexted at line %d, got \"%s\"\n", reader->CurrentLine(), word );
			return;
		}
		word = reader->NextWord();
		if( word == NULL )
		{
			printf("Unexpected eof at line %d\n", reader->CurrentLine());
			return;
		}

		printf("GDR: Processing channel section %s\n", word );
		ChannelSection *cs = new ChannelSection( word );
		cs->ProcessChannelSection( reader );	
		AddChannel( cs );
	}
	printf("GDR: end of Section\n");
		
}

SequenceProcessor::SequenceProcessor( char *a_filename, NoteList *notelist, int samplerate, int tempo )
{
	Reader *reader = new Reader();
	reader->OpenFile( a_filename );

	char *word;

	for( ;; )
	{
		word = reader->NextWord();
		if( word ==NULL )
		{
			printf("Unexpected eof at line %d\n", reader->CurrentLine() );
			return;
		}

		if( strcmp( word, "tempo") == 0 )
		{
			word = reader->NextWord();
			if( word ==NULL )
			{
				printf("Unexpected eof at line %d\n", reader->CurrentLine() );
				return;
			}
			tempo=atoi(word);
		}

			
		if( strcmp( word, "section") == 0 )
		{
			word = reader->NextWord();
			if( word ==NULL )
			{
				printf("Unexpected eof at line %d\n", reader->CurrentLine() );
				return;
			}

			Section *s = new Section( word );
			s->ProcessSection( reader );
			AddSection( s );

			continue;
		}

		if( strcmp( word, "play" ) == 0 )
		{
			printf("GDR: At \"play\"\n" );
			break;
		}
	}	

	int current = 10000;

	for(;;)
	{
		word = reader->NextWord();
		if( word ==NULL )
		{
			printf("Unexpected eof at line %d\n", reader->CurrentLine() );
			return;
		}

		if( strcmp(word, "end" ) == 0 )
		{
			break;
		}

		// otherwise
		current = RunSection( word, current, notelist, samplerate, tempo );
	}


}

void SequenceProcessor::AddSection( Section *a_section )
{
	bool found = false;

	assert( a_section );
	for( int i = 0 ; i < NUM_SECTIONS ; i ++ )
	{
		if( sections[i] == NULL )
		{
			sections[i] = a_section;
			found = true;
			break;
		}
	}
	assert( found == true );
}

int SequenceProcessor::RunSection( char *name, int startpoint, NoteList *notelist, int samplerate, int tempo )
	{
	assert( name);
	assert( notelist );

	Section *s = NULL;
	for( int i = 0 ; i < NUM_SECTIONS; i ++)
	{
		if( strcmp( sections[i]->name, name) == 0 )
		{
			s = sections[i];
			break;
		}
	}

	if( s == NULL )
	{
		printf("Could not find section \"%s\"\n", name );
		return startpoint;
	}

	int highest_endpoint = startpoint;
	int ended;
	// for each channel:
	for( int channel = 0; s->channels[channel] != NULL ; channel ++ )
	{
		ended = startpoint;
		// for each pattern in that channel
		ChannelSection *c = s->channels[channel];
		printf("GDR: Processing channel \"%s\" midi_channel=#%d\n", c->name, c->midi_channel);

		for( int pattern = 0 ; c->patterns[pattern] != NULL ; pattern ++ )
		{
			ended = c->patterns[pattern]->Write( notelist, ended, samplerate, tempo, 
				c->transposes[pattern], c->midi_channel );
		}
		if( ended > highest_endpoint )
		{
			highest_endpoint = ended;
		}
	}
	return highest_endpoint;
}


DeviceSeq::DeviceSeq()
{
	events = NULL;
	samplerate = 0;
	tempo = 125;

	notelist = new NoteList;
}

void DeviceSeq::Init( IDeviceEvents *a_events,
	char *instance_name,
	int a_samplerate,
	char *startup_params )
{
	assert( a_events );
	assert( instance_name );

	events = a_events;
	samplerate = a_samplerate;

	/*
	int current = 100000;
	Pattern *pattern = new Pattern();
	pattern->Load("test.pat");
	current = pattern->Write( notelist, current, samplerate, tempo, 0, 1 );
	current = pattern->Write( notelist, current, samplerate, tempo, 0, 1 );

	notelist->Restart();

	// write the end marker
	notelist->AddEvent( NOTELIST_EVENT_END, 0,0,0,current );
	*/

	SequenceProcessor *sp = new SequenceProcessor( startup_params, notelist, samplerate, tempo );

	// dont set running true yet, this is done by TransportPlay()
	// running = true;
}


bool DeviceSeq::TransportReset()
{
	running = false;
	notelist->Restart();
	return true;
}

bool DeviceSeq::TransportPlay()
{
	running = true;
	return true;
}

bool DeviceSeq::TransportStop()
{
	running = false;
	return true;
}


void DeviceSeq::Clock()
{
	if( !running ) 
	{
		return;
	}

	notelist->NextSample();

	int ttne = notelist->TimeTillNextEvent();
	
	if (ttne <= 0 )
	{
		while ( notelist->TimeTillNextEvent() <= 0 )
		{
			int event;
			int note;
			int channel;
			
			notelist->GetNextEvent( &event, 
				&channel,
				&note, 
				NULL );			

			if( event == NOTELIST_EVENT_END )
			{
				printf("SEQENCE ENDED\n");
				running = false;
				break;
			}
			if( event == NOTELIST_EVENT_NOTE_ON )
			{
				events->MidiNoteOn( channel, note, 64 );
			}
			if( event == NOTELIST_EVENT_NOTE_OFF )
			{
				events->MidiNoteOff( channel, note );
			}
		}
	}
}

