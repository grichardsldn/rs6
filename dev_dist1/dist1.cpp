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
	settings->AddSetting( "x", &x_setting );
        settings->AddSetting( "y", &y_setting );
        settings->AddSetting( "z", &z_setting );
	// add settings here...

        settings->Read();
        settings->Write();

        panel = Panel::CreatePanel();
        panel->SetPos( x_setting, y_setting,z_setting );
        panel->SetZ(0);
	// add panel widgets here...

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
	//	*output = ((int)(val*(16.0*256.0)) << 16);
}

void DeviceDist1::MidiNoteOn( int channel, int note, int vol )
{
}

void DeviceDist1::MidiNoteOff( int channel, int note )
{
}



