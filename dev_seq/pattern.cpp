#include <stdio.h>
#include <assert.h>
#include "../reader.h"
#include <stdlib.h>
#include <string.h>

#include "pattern.h"
#include "notelist.h"

#define NOTE_ON (15)
// (of 16 )

void Pattern::Load( char *filename)
{
	assert(filename);

	printf("Pattern: loading%s\n", filename );
	Reader reader;
	reader.OpenFile( filename );

	char *word = reader.NextWord();
	if(strcmp( word, "pattern" ) != 0)
	{
		printf("Not a pattern file: %s\n", word);
		return;
	}

	word = reader.NextWord();
	if(strcmp( word, "tune" ) != 0)
	{
		printf("Not of tune type file\n");
		return;
	}

	notelen = atoi( reader.NextWord() );
	//if(( notelen != 4 ) && (notelen != 3) )
	//{
//		printf("Only quarter or third notes supported\n");
//		return;
//	}

	char *read;
	while (strcmp( read = reader.NextWord(), "end" ) != 0 )
	{
		notes[num_notes].note_num = atoi( read );
		read = reader.NextWord();
		if( strcmp(read, ",") != 0 )
		{
			printf("Missing comma between note and len\n");
			return;
		}
		notes[num_notes].length = atoi( reader.NextWord() );
		read = reader.NextWord();
		if( strcmp(read, ",") != 0 )
		{
			printf("Missing comma after len\n");
			return;
		}
		printf("Read note %d, note=%d len=%d\n", num_notes, 
			notes[num_notes].note_num,
			notes[num_notes].length);
		num_notes ++;
	}
	printf("Read file\n");
}

int Pattern::Write( NoteList *nl,
	int location,
	int samplerate, 
	int tempo,
	int transpose, 	
	int midi_channel)
{
	int pos_in_bar = 0;
	for (int i = 0 ; i < num_notes ; i ++ )
	{
		bool play_note = true;
		if( notes[i].note_num == 0 )
		{
			play_note = false;
		}	

		int retard = 0;
		int note_len = samplerate * 60 / tempo / notelen * notes[i].length;
		if(( pos_in_bar % 3 ) == 2 )
		{
			retard = note_len / 10;
		}
		
		if( play_note )
		{
			nl->AddEvent( NOTELIST_EVENT_NOTE_ON,
				midi_channel, 
				notes[i].note_num + transpose, // note
				64, // vel
				location + retard );
		}

		pos_in_bar += notes[i].length;
		pos_in_bar %= notelen;

	
		int note_on_len = note_len * NOTE_ON / 16;
		if( play_note )
		{
			nl->AddEvent( NOTELIST_EVENT_NOTE_OFF,
				midi_channel, 
				notes[i].note_num + transpose, // note
				0, // unused 
				location + note_on_len );
		}	
		location += note_len;
	}

	return location;
}



