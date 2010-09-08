#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../IDevice.h"
#include "comp.h"
#include "sub.h"

#include "../darkbat/dkb.h"
#include "../ui/widget.h"

void DeviceSub::Init( 	IDeviceEvents *event,
				const char *instance_name, 
				int a_samplerate,
				const char *startup_params )
{
	printf("DeviceSub:Init: name=\"%s\" samplerate=#%d params=\"%s\"\n",
		instance_name, a_samplerate, startup_params );

	running=false;

	panel = new WidgetPanel();
	amp_attack_widget = new VolWidget( 2, 1, panel->dkb_obj, 4,3, NULL );
	panel->addWidget( amp_attack_widget );

	amp_decay_widget = new VolWidget( 2, 2, panel->dkb_obj, 2,3, NULL );
	panel->addWidget( amp_decay_widget );

	amp_sustain_widget = new VolWidget( 2, 3, panel->dkb_obj, 0,3, NULL );
	panel->addWidget( amp_sustain_widget );

	amp_release_widget = new VolWidget( 2, 4, panel->dkb_obj, -2,3, NULL );
	panel->addWidget( amp_release_widget );

	pitch_mod_widget = new VolWidget( 2, 5, panel->dkb_obj, -5,3, NULL );
	panel->addWidget( pitch_mod_widget );

	pwm_widget = new VolWidget( 2, 7, panel->dkb_obj, -8,3, NULL );
	panel->addWidget( pwm_widget );
	
	filt_level_widget = new VolWidget( 2, 9, panel->dkb_obj, -12, 1, NULL);
	panel->addWidget( filt_level_widget );
	
	filt_env_widget = new VolWidget( 2, 10, panel->dkb_obj, -14, 1, NULL);
	panel->addWidget( filt_env_widget );
	
	filt_lfo_widget = new VolWidget( 2, 11, panel->dkb_obj, -16, 1, NULL);
	panel->addWidget( filt_lfo_widget );

	sub_vol_widget = new VolWidget( 2, 12, panel->dkb_obj, -19, 1, NULL);
	panel->addWidget( sub_vol_widget );
	noise_vol_widget = new VolWidget( 2, 13, panel->dkb_obj, -21, 1, NULL );
	panel->addWidget( noise_vol_widget );

	tonegen = new ToneGen( a_samplerate );
	tonegen->SetWaveform( WAVEFORM_PWM);
	
	sub_tonegen = new ToneGen( a_samplerate);
	sub_tonegen->SetWaveform( WAVEFORM_SAW);

	noise_tonegen = new ToneGen( a_samplerate );
	noise_tonegen->SetWaveform( WAVEFORM_NOISE );

	amp_adsr = new ADSR(a_samplerate);
	amp_adsr->reset_on_trigger = true;

	pwm_lfo = new LFO(a_samplerate);
	pwm_lfo->setRate(0.1);

	slewer = new Slewer( a_samplerate );
	output = NULL;

	octave_adjust = 0;
	running = true;
}

bool DeviceSub::SetMidiInput( const char *input_name, int channel )
{
	if( strcmp( input_name, "main") == 0)
	{
		midi_channel = channel;
	}
	else
	{
		assert(false);
	}
	return true;
}

bool DeviceSub::SetOutput( const char *output_name, int *output_ptr )
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
	sub_tonegen = NULL;
	noise_tonegen = NULL;
	output = NULL;
}

void DeviceSub::CopyParams()
{
	double val;
	val = (double)amp_attack_widget->getVol() / 10.0;
	//val *= val;
	amp_adsr->setAttack( val );

	val = (double)amp_decay_widget->getVol() / 10.0;
	//val *= val;
	amp_adsr->decay = val;
	val = (double)amp_sustain_widget->getVol() / 10.0;
	amp_adsr->sustain_level =  val;
	val = (double)amp_release_widget->getVol() / 10.0;
	amp_adsr->release =  val;

	val = (double)pitch_mod_widget->getVol() / 200.0;
	pitch_mod = val;

	val = (double)pwm_widget->getVol() / 10.0;
	pwm = val;

	filt_level = (double)filt_level_widget->getVol() / 10.0;
	filt_env = (double)filt_env_widget->getVol() / 10.0;
	filt_lfo = (double)filt_lfo_widget->getVol() / 10.0;

	sub_vol = (double)sub_vol_widget->getVol() / 10.0;
	noise_vol = (double)noise_vol_widget->getVol() / 10.0;
}

void DeviceSub::Clock()
{
	if( running == false ) return;

	assert( tonegen );

	CopyParams();
	
	double al = amp_adsr->Clock();

	double lfo_val = pwm_lfo->Clock();	

	double val = tonegen->Clock( lfo_val*pitch_mod, al*pwm);
	val += sub_tonegen->Clock( lfo_val *pitch_mod , 0.0) * sub_vol;
	val += noise_tonegen->Clock( lfo_val * pitch_mod,0.0 ) * noise_vol;

	//double val = tonegen->Clock( 0.0, 0.0);
	val *= al;
	
	//	if( val > 0 ) val = 0.5;
	//if( val < 0 ) val = -0.5;
	
	
	double filt_out = filt_level +
			(filt_lfo * lfo_val ) +
			(filt_env * al );
	// 0.01 lowest?
	val = slewer->Clock( val, filt_out);

	if( output)
	{
		*output = ((int)(val*16.0) << 24);
	} 
}

void DeviceSub::MidiNoteOn( int channel, int note, int vol )
{
	if( channel == midi_channel )
	{
		//note-=12;	
		assert( tonegen );
		tonegen->NoteOn( note + (octave_adjust * 12) );
		sub_tonegen->NoteOn( note - 12 + (octave_adjust * 12));
		noise_tonegen->NoteOn( note + (octave_adjust * 12 ) );
		current_note = note;
		printf("GDR: Sub, got note #%d\n", note );
	
		amp_adsr->Trigger();
	}

}

void DeviceSub::MidiNoteOff( int channel, int note )
{
	assert(tonegen );

	printf("DeviceSub::MidiNoteOff\n");

	if( channel == midi_channel )
	{	
		if( note == current_note )
		{
			//tonegen->NoteOff();
			amp_adsr->Release();
		}
	}
}



