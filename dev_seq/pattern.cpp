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
	bool tunex = false;
	if( strcmp( word, "tunex" ) == 0)
	{
		strcpy( word, "tune" );
		tunex = true;
	}
	if(strcmp( word, "tune" ) == 0)
	{

		notelen = atoi( reader.NextWord() );
		//if(( notelen != 4 ) && (notelen != 3) )
		//{
//			printf("Only quarter or third notes supported\n");
//			return;
//		}

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

			if( tunex )
			{
				if( strcmp(reader.NextWord(), ",") != 0 )
				{
					printf("Missing comma between len and vel\n");
					return;
				}
				notes[num_notes].vel = atoi( reader.NextWord() );
				if( strcmp(reader.NextWord(), ",") != 0 )
				{
					printf("Missing comma between vel and offset\n");
					return;
				}
				read = reader.NextWord();
				bool neg = false;
				if( strcmp( read, "-" ) == 0 )
				{
					neg = true;
					read = reader.NextWord();
				}
				int offset = atoi(read);
				if( neg ) offset *= -1;
				notes[num_notes].offset = offset;

			} else
			{
				notes[num_notes].vel = 64;
				notes[num_notes].offset = 0;
			}	
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
	else
	{
		printf("Not of tune type file\n");
		return;
	}
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
		//	retard = note_len / 10;
		}

		// should be hundredth of a min length note
		retard = notes[i].offset * (note_len / notes[i].length / 100 );
		
		if( play_note )
		{
			nl->AddEvent( NOTELIST_EVENT_NOTE_ON,
				midi_channel, 
				notes[i].note_num + transpose, // note
				notes[i].vel, // vel
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



