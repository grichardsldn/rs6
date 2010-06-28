#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../IDevice.h"


#include "../settings.h"
#include "sub102comp.h"
#include "sub102.h"


void DeviceSub102::Init( 	IDeviceEvents *event,
				const char *instance_name, 
				int a_samplerate,
				const char *startup_params )
{
	printf("DeviceSub102:Init: name=\"%s\" samplerate=#%d params=\"%s\"\n",
		instance_name, a_samplerate, startup_params );

	running=false;

	settings = new Settings(startup_params);

	amp_attack_setting = 1;
	settings->AddSetting( "amp_attack", &amp_attack_setting );

	amp_decay_setting = 1;
	settings->AddSetting( "amp_decay", &amp_decay_setting );

	amp_sustain_setting = 1;
	settings->AddSetting("amp_sustain", &amp_sustain_setting );

	amp_release_setting = 1;
	settings->AddSetting("amp_release", &amp_release_setting );

	pwm_setting = 1;
	settings->AddSetting("pmw", &pwm_setting );

	
	filt_level_setting = 1;
	settings->AddSetting("filt_level", &filt_level_setting );

	filt_env_setting = 1;
	settings->AddSetting("filt_env", &filt_env_setting );

	filt_lfo_setting = 1;
	settings->AddSetting("filt_lfo", &filt_lfo_setting );

		
	sub_vol_setting = 1;
	settings->AddSetting("sub_vol", &sub_vol_setting );

	noise_vol_setting = 1;
	settings->AddSetting("noise_vol", &noise_vol_setting );

	int note = 0;
	int midi_channel = 0;
	tonegen = new Sub102::ToneGen( a_samplerate );
	tonegen->SetWaveform( WAVEFORM_PWM);
	
	sub_tonegen = new Sub102::ToneGen( a_samplerate);
	sub_tonegen->SetWaveform( WAVEFORM_SAW);

	noise_tonegen = new Sub102::ToneGen( a_samplerate );
	noise_tonegen->SetWaveform( WAVEFORM_NOISE );

	amp_adsr = new Sub102::ADSR(a_samplerate);
	amp_adsr->reset_on_trigger = true;

	pwm_lfo = new Sub102::LFO(a_samplerate);
	pwm_lfo->setRate(0.1);

	slewer = new Sub102::Slewer( a_samplerate );
	slewer2 = new Sub102::Slewer( a_samplerate );
	output = NULL;

	octave_adjust = 0;
	pitch_mod = 0.0;
	settings->Read();
	settings->Write();
	running = true;
}

bool DeviceSub102::SetMidiInput( const char *input_name, int channel )
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

bool DeviceSub102::SetOutput( const char *output_name, int *output_ptr )
{
	printf("Sub102: set output to &%08x\n", (unsigned int)output_ptr);
	assert( output_ptr );
	output = output_ptr;
	return true;
}

DeviceSub102::DeviceSub102()
{
	int note = 0;
	int midi_channel = 0;
	tonegen = NULL;
	sub_tonegen = NULL;
	noise_tonegen = NULL;
	output = NULL;
	running = false;
}

void DeviceSub102::CopyParams()
{
	double val;
	val = (double)amp_attack_setting / 10.0;
	//val *= val;
	amp_adsr->setAttack( val );

	val = (double)amp_decay_setting / 10.0;
	//val *= val;
	amp_adsr->decay = val;
	val = (double)amp_sustain_setting / 10.0;
	amp_adsr->sustain_level =  val;
	val = (double)amp_release_setting / 10.0;
	amp_adsr->release =  val;

	val = (double)pwm_setting / 10.0;
	pwm = val;

	filt_level = (double)filt_level_setting / 10.0;
	filt_env = (double)filt_env_setting / 10.0;
	filt_lfo = (double)filt_lfo_setting / 10.0;

	sub_vol = (double)sub_vol_setting / 10.0;
	noise_vol = (double)noise_vol_setting / 10.0;
}

void DeviceSub102::Clock()
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
	val = slewer2->Clock( val, filt_out);

	if( output)
	{
		*output = ((int)(val*(16.0*256.0)) << 16);
	} 
}

void DeviceSub102::MidiNoteOn( int channel, int note, int vol )
{
	if (!running ) return;

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

void DeviceSub102::MidiNoteOff( int channel, int note )
{
	if (!running ) return;
	assert(tonegen );

	printf("DeviceSub102::MidiNoteOff\n");

	if( channel == midi_channel )
	{	
		if( note == current_note )
		{
			//tonegen->NoteOff();
			amp_adsr->Release();
		}
	}
}



