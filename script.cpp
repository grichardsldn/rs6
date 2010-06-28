#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include "IDevice.h"
#include "IManage.h"
#include "RS7Mapper.h"
#include "script.h"
#include "reader.h"

Script::Script(IManage *a_manager)
{
	assert( a_manager );
	manager = a_manager;
}

// dev_midi_in instance inputname channel 
bool Script::dev_midi_in ( Reader *reader, const char *word )
{
	if( strcmp( word , "dev_midi_in" ) != 0 )
	{
		return false;
	}
	char instance[1024];
	char input_name[2048];

	strcpy( &instance[0], reader->NextWord() );
	strcpy( &input_name[0], reader->NextWord() );
	int channel = atoi( reader->NextWord() );

	printf("dev_midi_in %s %s %d \n", instance,input_name, channel);
	manager->SetDeviceMidiInput( manager->FindDevice("", instance ),
		input_name, channel );
	return true;
}

// dev_in instance inputname channel 
bool Script::dev_in ( Reader *reader, const char *word )
{
	if( strcmp( word , "dev_in" ) != 0 )
	{
		return false;
	}
	char instance[1024];
	char input_name[2048];

	strcpy( &instance[0], reader->NextWord() );
	strcpy( &input_name[0], reader->NextWord() );
	int channel = atoi( reader->NextWord() );

	printf("dev_in %s %s %d \n", instance,input_name, channel);
	manager->SetDeviceInput( manager->FindDevice("", instance ),
		input_name, channel );
	return true;
}

// dev_midi_out instance oututname channel 
bool Script::dev_midi_out ( Reader *reader, const char *word )
{
	if( strcmp( word , "dev_midi_out" ) != 0 )
	{
		return false;
	}
	char instance[1024];
	char output_name[2048];

	strcpy( &instance[0], reader->NextWord() );
	strcpy( &output_name[0], reader->NextWord() );
	int channel = atoi( reader->NextWord() );

	printf("dev_midi_out %s %s %d \n", instance,output_name, channel);
//	manager->SetDeviceMidiOutput( manager->FindDevice("", instance ),
//		output_name, channel );
	return true;
}

// dev_out instance oututname channel 
bool Script::dev_out ( Reader *reader, const char *word )
{
	if( strcmp( word , "dev_out" ) != 0 )
	{
		return false;
	}
	char instance[1024];
	char output_name[2048];

	strcpy( &instance[0], reader->NextWord() );
	strcpy( &output_name[0], reader->NextWord() );
	int channel = atoi( reader->NextWord() );

	printf("dev_out %s %s %d \n", instance,output_name, channel);
	manager->SetDeviceOutput( manager->FindDevice("", instance ),
		output_name, channel );
	return true;
}

// create_dev type instance param 
bool Script::create_dev( Reader *reader, const char *word )
{
	if( strcmp( word , "create_dev" ) != 0 )
	{
		return false;
	}
	char devtype[1024];
	char instance[1024];
	char param[2048];

	strcpy( &devtype[0], reader->NextWord() );
	strcpy( &instance[0], reader->NextWord() );
	strcpy( &param[0], reader->NextWord() );
	printf("create_dev %s %s %s \n", devtype, instance, param );
	manager->CreateDevice( devtype, instance, param );
	return true;
}

// monitor leftchannel rightchannel
bool Script::monitor( Reader *reader, const char *word )
{
	if( strcmp( word , "monitor" ) != 0 )
	{
		return false;
	}
	int left = atoi(reader->NextWord() );
	int right = atoi(reader->NextWord() );

	printf("monitor %d %d\n", left, right );
	manager->Monitor( left, right );
	return true;
}

bool Script::ctrl( Reader *reader )
{
	bool ok = false;
	char *word = reader->NextWord();
	printf("ctrl: %s\n", word);

	if( monitor( reader, word ) )
	{
		ok = true;
	}
	else if ( create_dev( reader, word ))
	{
		ok = true;
	}
	else if ( dev_in( reader, word ))
	{
		ok = true;
	}
	else if ( dev_midi_in( reader, word ))
	{
		ok = true;
	}
	else if ( dev_out( reader, word ))
	{
		ok = true;
	}
	else if ( dev_midi_in( reader, word ))
	{
		ok = true;
	}
	else if ( create_dev( reader, word ))
	{
		ok = true;
	}
	else
	{	
		printf("unknown ctrl subcommand: %s\n", word );
	}
	return  ok;
}

bool Script::Run( const char *filename )
{
	bool ok = false;
	assert( filename );
	Reader *reader = new Reader();
	ok = reader->OpenFile( filename );
	if( ok )
	{
		char *word = NULL;
		while ( (word = reader->NextWord()) )
		{
			if( strcmp( word, "ctrl" ) == 0)
			{
				ok = ctrl( reader );
				if( ok == false )
				{
					break;	
				}	
			}	
			else
			{
				printf("Uknown root command: %s\n", word );
				break;
			}
		}
	}
	delete reader;
	return ok;	
}
