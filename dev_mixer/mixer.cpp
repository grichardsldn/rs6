#include <stdio.h>
#include "../IDevice.h"
#include "mixer.h"
#include <assert.h>
#include <string.h>

#include <pthread.h>
#include "../darkbat/dkb.h"
#include "vol.h"
#include "../settings.h"


#include "../ui/Panel.h"

static int nowhere = 0;

void DeviceMixer::Init( IDeviceEvents *event,
			const char *instance_name,
			int samplerate,
			const char *startup_params)
{
	char name[1024];
	printf( "instance=%s\n", instance_name);
	sprintf( &name[0], "%s.set", instance_name);
	printf( "instance=%s\n", name);
	x = 0; y =0; z = 0;
	settings = new Settings( name );
	settings->AddSetting( "x", &x );
	settings->AddSetting( "y", &y );
	settings->AddSetting( "z", &z );
	printf( "y=%d\n", y );
	// dont care about any of these
	//vol_panel = new VolPanel();
	//vol_panel = new VolPanel(x,y,z);
	for( int i = 0 ; i < NUM_INPUTS ; i ++ )
	{
		char sname[100];
		sprintf( &sname[0], "chvol%d", i );
		volumes[i] = 0;
		settings->AddSetting( sname, &volumes[i] );
	}
	out_left = &nowhere;
	out_right = &nowhere;	
	settings->Read();
	settings->Write();
	CreatePanel();
}

void DeviceMixer::CreatePanel()
{
        int w= (NUM_INPUTS * 3) + 4 ;
        int h = 14;
        panel = Panel::CreatePanel();
        panel->SetPos( x,y,z );
        panel->SetZ(5);
        panel->AddLine( 100, 0,0, w, 0 );
        panel->AddLine( 101, w,0, w, h );
        panel->AddLine( 102, w,h, 0, h );
        panel->AddLine( 103, 0,h, 0, 0 );
        panel->SetZ(0);

	for( int i = 0 ; i < NUM_INPUTS ; i ++ )
	{
	
        	panel->AddVSlider(104+i, 2 + (i * 3), 2, 10, &volumes[i],0 );
	}
}
	
void DeviceMixer::Clock()
{
	int val = 0;

	for( int i = 0 ; i < NUM_INPUTS ; i++)
	{
		if( inputs[i] != NULL )
		{	
			val += (*inputs[i] / 20 * volumes[i]);	
		}
	}
	if( val >2000000000  )	
	{
		val = 2000000000;;
	}
	if( val < -2000000000)
	{
		val = -2000000000;
	}
	/*
	if( val >8388600  )	
	{
		val == 8388600;;
	}
	if( val < 8388600)
	{
		val = -8388600;
	}*/

	if( out_left != NULL ) *out_left = val;
	if( out_right != NULL ) *out_right = val;
}


bool DeviceMixer::SetInput( const char *input_name, int *input_ptr )
{
	int index = -1;
	for( int i = 0 ; i<  NUM_INPUTS; i++)
	{
		if( inputs[i] == NULL)
		{
			index = i;
			break;
		}
	}
	if (index == -1 )
	{
		printf("DeviceMixer: Too many inputs\n");
		return false;
	}

	inputs[index] = input_ptr;

	return true;
}

bool DeviceMixer::SetOutput( const char *output_name, int *output_ptr)
{
	assert( output_name );
	assert( output_ptr );

	if( strcmp( "left", output_name) == 0 )
	{
		out_left = output_ptr;
	}
	else if( strcmp( "right", output_name) == 0 )
	{
		out_right = output_ptr;
	}
	else
	{
		printf("DeviceMixer: Output %s unknown\n",
			output_name);
		return false;
	}
	return true;
}	
		

DeviceMixer::DeviceMixer()
{
	for( int i = 0 ; i < NUM_INPUTS; i++)
	{
		inputs[i] = NULL;
	}
	out_left = NULL;
	out_right = NULL;
}
