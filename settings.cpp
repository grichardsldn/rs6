#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "reader.h"
#include "settings.h"

Settings::Settings( char *a_filename )
{
	assert( a_filename );
	num_settings = 0;
	filename = new char[1000];
	strcpy( filename , a_filename);
}

void Settings::AddSetting( char *name, float *addr )
{
	AddSetting( name, DATATYPE_FLOAT, addr );
}

void Settings::AddSetting( char *name, int *addr )
{
	AddSetting( name, DATATYPE_INT, addr );
}

void Settings::AddSetting( char *name, char *addr )
{
	AddSetting( name, DATATYPE_STRING, addr );
}

int Settings::FindSetting( char *name )
{
	for( int i = 0 ; i < num_settings ; i ++ )
	{
		if( strcmp( names[i], name) == 0 )
		{
			return i;
		}
	}
	return -1;
}

void Settings::AddSetting( char *name, int datatype, void *ptr)
{
	if( FindSetting( name ) != -1 ) 
	{
		printf("AddSetting: tried to add setting %s twice\n", name );
		return;
	}
	int i = num_settings;
	names[i] = new char[1024];
	strcpy( names[i], name );
	datatypes[i] = datatype;
	ptrs[i] = ptr;	

	num_settings ++;
}

void Settings::Read()
{
	Reader *reader = new Reader();
	reader->OpenFile( filename );

	char *word;
	while( strcmp( (word = reader->NextWord()) , "" ) != 0 )
	{
		int setting = FindSetting( word );
		if( setting == -1 )
		{
			printf("Unknown setting ignored : %s\n", word );
		}
		else
		{
			char *val = reader->NextWord();
			printf("Setting %s val %s\n", names[setting], val );
			int datatype = datatypes[setting];
			if( datatype == DATATYPE_STRING )
			{
				printf("STRING\n");
				strcpy( (char *)ptrs[setting], val );
			} else if ( datatype == DATATYPE_INT )
			{
				printf("val=%d\n", atoi(val ));
				*(int *)(ptrs[setting]) = atoi( val );
			} else 
			{
				printf("FLOAT\n");
				*(float *)ptrs[setting] = atof( val );
			}
		}
	}	
	delete( reader );
}
void Settings::Write()
{
	FILE *outptr = fopen( filename, 	"w");
	if( outptr )
	{
		for( int i = 0 ; i < num_settings ; i++ )
		{
			if( datatypes[i] == DATATYPE_STRING )
			{
				fprintf( outptr, "%s %s\n", 
					names[i], (char *)ptrs[i] );
			} else if ( datatypes[i] == DATATYPE_INT )
			{
				fprintf( outptr, "%s %d\n", 	
					names[i], *(int*)ptrs[i] );
			} else {
				fprintf( outptr, "%s %f\n", 	
					names[i], *(float*)ptrs[i] );
			}
		}	
		fclose( outptr );
	}
}
	
