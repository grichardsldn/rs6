#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../IDevice.h"
#include "comp.h"
#include "sub.h"

#include "/home/user/svn2/Darkbat/api/dkb.h"
#include "../ui/widget.h"

void DeviceSub::Init( 	IDeviceEvents *event,
				char *instance_name, 
				int a_samplerate,
				char *startup_params )
{
	printf("DeviceSub:Init: name=\"%s\" samplerate=#%d params=\"%s\"\n",
		instance_name, a_samplerate, startup_params );

	panel = new WidgetPanel();
	attack_widget = new VolWidget( 2, 1, panel->dkb_obj, 3,3, NULL );
	panel->addWidget( attack_widget );
	decay_widget = new VolWidget( 2, 5, panel->dkb_obj, 9,9, NULL );
	panel->addWidget( decay_widget );

	int note = 0;
	int midi_channel = 0;
	tonegen = new ToneGen( a_samplerate );
	tonegen->SetWaveform( WAVEFORM_PWM);
	
	mod_tonegen = new ToneGen( a_samplerate);
	mod_tonegen->SetWaveform( WAVEFORM_SAW);
	amp_adsr = new ADSR(a_samplerate);

	pwm_lfo = new LFO(a_samplerate);
	pwm_lfo->setRate(0.5);

	slewer = new LinearSlewer( a_samplerate );
	output = NULL;
}

bool DeviceSub::SetMidiInput( char *input_name, int channel )
{
	if( strcmp( input_name, "") == 0)
	{
		midi_channel = channel;
	}
	else
	{
		assert(false);
	}
	return true;
}

bool DeviceSub::SetOutput( char *output_name, int *output_ptr )
{
	printf("Sub: set output to &%08x\n", (unsigned int)output_ptr);
	assert( output_ptr );
	output = output_ptr;
	return true;
}

DeviceSub::DeviceSub()
{
	int note = 0;
	int midi_channel = 0;
	tonegen = NULL;
	output = NULL;
}

void DeviceSub::CopyParams()
{
	float val;
	val = (float)attack_widget->getVol() / 10.0;
	amp_adsr->setAttack( val );
	val = (float)decay_widget->getVol() / 10.0;
	amp_adsr->decay =  val;
}

void DeviceSub::Clock()
{
	assert( tonegen );

	CopyParams();
	
	float al = amp_adsr->Clock();
	//float val = tonegen->Clock( pwm_lfo->Clock()/80.0, al/2.0);
	float val = tonegen->Clock( 0.0, 0.0);
	val *= al;
	if( val > 0 ) val = 0.5;
	if( val < 0 ) val = -0.5;
	

	// BBB
	// 0.01 lowest?
	val = slewer->Clock( val, al);

	if( output)
	{
		*output = ((int)(val*16.0) << 24);
	} 
}

void DeviceSub::MidiNoteOn( int channel, int note, int vol )
{
	if( channel == midi_channel )
	{
		note-=12;	
		assert( tonegen );
		tonegen->NoteOn( note );
		current_note = note;
		printf("GDR: Sub, got note #%d\n", note );
	
		amp_adsr->Trigger();
	}

}

void DeviceSub::MidiNoteOff( int channel, int note )
{
	assert(tonegen );

	if( channel == midi_channel )
	{	
		if( note == current_note )
		{
			//tonegen->NoteOff();
			amp_adsr->Release();
		}
	}
}



