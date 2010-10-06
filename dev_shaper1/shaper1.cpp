#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../IDevice.h"


#include "../settings.h"
#include "shaper1.h"

#include "../ui/Panel.h"

	
class Shaper1Slewer
{
	public:
	Shaper1Slewer( int samplerate );
	
	double Clock( double input, double rate );
	private:
	double val;
	double samplerate;
};

Shaper1Slewer::Shaper1Slewer( int a_samplerate)
{
        assert( a_samplerate > 0 );

        samplerate = a_samplerate;
        val = 0.0;
}

double Shaper1Slewer::Clock( double input, double rate )
{
        //rate = rate / rate;
        //rate = rate *4800.0/samplerate;

        double diff = input - val;

        diff *= (rate * 0.1);
        //diff = diff / rate;
        val += diff;

        return (double)val;
}


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

        panel = Panel::CreatePanel();
        panel->SetPos( x_setting, y_setting,z_setting );
        panel->SetZ(0);
	for (int i = 0 ; i < SHAPER1_NUM_VALUES ;i++)
	{
        	panel->AddVSlider(104 + i, i * 3, 2, 10, 
			&lpf_value_settings[i] );
	}

	slewer1 = new Shaper1Slewer(a_samplerate);	
	slewer2 = new Shaper1Slewer(a_samplerate);	
	slewer3 = new Shaper1Slewer(a_samplerate);	
	midi_channel = 0;
	pos =-1;
	through = 0;
	input_ptr = NULL;
	output_ptr = NULL;
}


bool DeviceShaper1::SetMidiInput( const char *input_name, int channel )
{
	midi_channel = channel;
	return true;
}

bool DeviceShaper1::SetMidiOutput( const char *output_name, int channel )
{
	return true;
}


bool DeviceShaper1::SetOutput( const char *output_name, int *a_output_ptr )
{
	assert( a_output_ptr );
	output_ptr = a_output_ptr;
	return true;
}

bool DeviceShaper1::SetInput( const char *input_name, int *a_input_ptr )
{
	assert( a_input_ptr );

	input_ptr = a_input_ptr;
	return true;
}

DeviceShaper1::DeviceShaper1()
{
}

void DeviceShaper1::Clock()
{
	if( output_ptr == NULL ) return;
	if( input_ptr == NULL ) return;
	if( pos > -1 )
	{
		through += 50;
		if( through > 100000 )
		{
			through -= 100000;
			pos ++;
		}
		if( pos == SHAPER1_NUM_VALUES)
		{
			pos = -1;
		}
	}
	int ival = (*input_ptr) >>16  ;
	double val = (double)ival;
	val /= (16.0 * 256.0);
	double amount = 0.0;
	if( pos > -1 )
	{		
		amount = (double)lpf_value_settings[pos] / 3.0;		
	}
	val = slewer1->Clock( val, amount );
	val = slewer2->Clock( val, amount );
	val = slewer3->Clock( val, amount );

	*output_ptr = ((int)(val*(16.0*256.0)) << 16);
}

void DeviceShaper1::MidiNoteOn( int channel, int note, int vol )
{
	if( channel == midi_channel )
	{
		pos = 0;
		through = 0;
	}
}

void DeviceShaper1::MidiNoteOff( int channel, int note )
{
}



