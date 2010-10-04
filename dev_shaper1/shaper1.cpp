#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../IDevice.h"


#include "../settings.h"
#include "shaper1.h"

void DeviceShaper1::Init( 	IDeviceEvents *event,
				const char *instance_name, 
				int a_samplerate,
				const char *startup_params )
{
	settings = new Settings(startup_params);
	x_setting = 0;
	y_setting = 0;
	z_setting = 0;
	settings->AddSetting( "x", &x_setting );	
	settings->AddSetting( "y", &y_setting );	
	settings->AddSetting( "z", &z_setting );	
	for( int i = 0 ; i < SHAPER1_NUM_VALUES; i++ )
	{
		char settingname[1024];
		sprintf( settingname, "lpf_val%i", i );
		lpf_value_settings[i] =  0;
		settings->AddSetting( settingname, &lpf_value_settings[i] );
	}
	lpf_offset_setting = 5;
	settings->AddSetting( "lpf_offset", &lpf_offset_setting );
	lpf_range_setting = 5;
	settings->AddSetting( "lpf_range", &lpf_range_setting );
	rate_setting = 5;
	settings->AddSetting( "rte", &rate_setting );
        settings->Read();
        settings->Write();
	
}


bool DeviceShaper1::SetMidiInput( const char *input_name, int channel )
{
	return true;
}

bool DeviceShaper1::SetMidiOutput( const char *output_name, int channel )
{
	return true;
}


bool DeviceShaper1::SetOutput( const char *output_name, int *output_ptr )
{
	assert( output_ptr );
	return true;
}

bool DeviceShaper1::SetInput( const char *input_name, int *input_ptr )
{
	assert( input_ptr );
}

DeviceShaper1::DeviceShaper1()
{
}

void DeviceShaper1::Clock()
{
	//	*output = ((int)(val*(16.0*256.0)) << 16);
}

void DeviceShaper1::MidiNoteOn( int channel, int note, int vol )
{
}

void DeviceShaper1::MidiNoteOff( int channel, int note )
{
}



