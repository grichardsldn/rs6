#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../IDevice.h"


#include "../settings.h"
#include "../ui/Panel.h"

#include "dist1.h"

void DeviceDist1::Init( 	IDeviceEvents *event,
				const char *instance_name, 
				int a_samplerate,
				const char *startup_params )
{
	settings = new Settings( startup_params );
	x_setting = 0;
	y_setting = 0;
	z_setting = 0;
	pre_setting = 2;
	post_setting = 2;
	level_setting = 8;
	engage_setting = 1;
	settings->AddSetting( "x", &x_setting );
        settings->AddSetting( "y", &y_setting );
        settings->AddSetting( "z", &z_setting );
	settings->AddSetting( "pre", & pre_setting );
	settings->AddSetting( "post", & post_setting );
	settings->AddSetting( "level", & level_setting );
	settings->AddSetting( "engage", & engage_setting );
	

	// add settings here...

        settings->Read();
        settings->Write();

        panel = Panel::CreatePanel();
        panel->SetPos( x_setting, y_setting,z_setting );
        panel->SetZ(0);
	// add panel widgets here...

	panel->AddVSlider(100, 2, 2, 10, &pre_setting,1 );
	panel->AddVSlider(101, 5, 2, 10, &level_setting,1 );
	panel->AddVSlider(102, 8, 2, 10, &post_setting,1 );
	panel->AddCheckbox( 103, 11, 4, &engage_setting );
	panel->AddButton( 104, 11, 8, this );

	input_ptr = NULL;
        output_ptr = NULL;
}

bool DeviceDist1::SetMidiInput( const char *input_name, int channel )
{
	return true;
}

bool DeviceDist1::SetMidiOutput( const char *output_name, int channel )
{
	return true;
}


bool DeviceDist1::SetOutput( const char *output_name, int *a_output_ptr )
{
	assert( a_output_ptr );
	output_ptr = a_output_ptr;
	return true;
}

bool DeviceDist1::SetInput( const char *input_name, int *a_input_ptr )
{
	assert( a_input_ptr );
	input_ptr = a_input_ptr;
	return true;
}

DeviceDist1::DeviceDist1()
{
}

void DeviceDist1::Clock()
{
	if( input_ptr == NULL ) return;
	if( output_ptr == NULL ) return;

        signed int ini = *input_ptr >> 16;
	double in = ini / ( 16.0 * 256.0);
	if( engage_setting )
	{
		in *= (double)pre_setting / 4.0;

		double sign = 1.0;
		if ( in < 0.0 ) sign = -1.0;
		// make positive
		in *= sign;

		// work out the Over amount then subtract
		// half of it.
		double level = ((double)level_setting / 5.0 );
		
		if ( in > level )
		{
			double over = in - level;
			over *= 0.5;
			in = level + over;		
		} 

		if ( in > 0.9 ) in = 0.9;
		// ut the sign back
		in *= sign;
		in *= (double)post_setting / 5.0;
	}
	double out = in;
        //signed int outi = out * 32700.0;
        //*output_ptr = outi << 24;
	*output_ptr = ((int)(out*(16.0*256.0)) << 16);
}

void DeviceDist1::MidiNoteOn( int channel, int note, int vol )
{
}

void DeviceDist1::MidiNoteOff( int channel, int note )
{
}

void DeviceDist1::Event( int ref, int key )
{
	settings->Write();
}

