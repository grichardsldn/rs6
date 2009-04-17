#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "comp.h"

#define STATE_ATTACK  (1)
#define STATE_DECAY (2)
#define STATE_SUSTAIN  (3)
#define STATE_RELEASE (4)

Slewer::Slewer( int a_samplerate)
{
	assert( a_samplerate > 0 );

	samplerate = a_samplerate;
	val = 0.0;
}

LinearSlewer::LinearSlewer( int a_samplerate )
{
	assert( a_samplerate > 0 );

	samplerate = a_samplerate;
	val = 0.0;
}

float Slewer::Clock( float input, float rate )
{
	rate = rate * rate;
	//rate = rate *4800.0/samplerate;

	double diff = input - val;

	diff *= (rate * 0.1);
	//diff = diff / rate;
	val += diff;

	return (float)val;
}

// rate is max per ms.
float LinearSlewer::Clock( float input, float rate )
{
	rate = rate * rate;

	rate *= 3.0;

	float delta = rate * 4800.0 / samplerate ;
	if( input > val )
	{
		val += delta;
		if( val > input) val = input;
	}
	else
	{
		val -= delta;
		if( val < input) val = input;
	}
	return val;
}	

ADSR::ADSR( int a_samplerate)
{
	assert(a_samplerate > 0 );

	samplerate = (float) a_samplerate;
	priv_attack = 5.0;
	decay = 0.2;
	sustain_level = 0.05;
	release = 0.1;

	level = 0.0;
	state = STATE_RELEASE;
}	

void ADSR::setAttack( float val )
{
	priv_attack = val;
}

float ADSR::getAttack()
{
	return priv_attack;
}

void ADSR::Trigger( )
{
	state = STATE_ATTACK;
	if( reset_on_trigger == true )
	{
		level = 0.0;
	}	
	//state = STATE_DECAY;
	//level= 1.0;
}

void ADSR::Release( )
{	
	state = STATE_RELEASE;
}

float  ADSR::Scale( float diff, float rate )
{
	return (  (diff / (samplerate / 48) ) * rate / 4.0 );
} 

float ADSR::Clock()
{	
	float diff;
	switch( state )
	{
		case STATE_ATTACK:
		
			diff = 1.0 - level;
			level += Scale( diff, getAttack() );

			if( level > 0.98 )
			{	
				state = STATE_DECAY;
				level = 1.0;
			}
		break;

		case STATE_DECAY:
			if( level < sustain_level )
			{
				state = STATE_SUSTAIN;
				level = sustain_level;
			}

			diff = level - sustain_level;	
			level -= Scale( diff, decay );
		break;
		
		case STATE_SUSTAIN:
			// do nothing
		break;
		
		case STATE_RELEASE:
			level -= Scale( level, release );
		break;
	}

	return level;
}

LFO::LFO(int a_samplerate)
{
	assert( a_samplerate > 0 );
	samplerate = (float)a_samplerate;
	
	setRate( 0.1 );
	through = 0.0;
}

void LFO::setRate( float a_rate )
{
	rate = a_rate;
	increment = a_rate  / samplerate;
}

float LFO::getRate()
{
	return rate;
}

float LFO::Clock()
{	
	through += increment;

	if( through > 1.0 )
	{
		through -= 1.0;
	}

	if( through < 0.5 )
	{
		// on the up
		return (through * 4.0 ) - 1.0;
	}	
	else
	{	
		// on the down
		return ((1.0 - through ) * 4.0) - 1.0;
	}
}
		

ToneGen::ToneGen(int a_samplerate)
{
	assert( a_samplerate>0);
	samplerate = a_samplerate;
	waveform = WAVEFORM_SQUARE;
	through = 0.;

	// replace with integer stuff later.
	//sub_through=0;
	//delta = 0;
	//sub_delta = 0;	
	increment = 0.0;
	note = 0;
}

void ToneGen::SetWaveform(int a_waveform)
{
	waveform = a_waveform;
}

void ToneGen::NoteOn( int a_note )
{	
	float freq;
	
	note = a_note;
	note += 2; // Dunno why, dev_strings did this
	
	freq= 8.1758 * pow(2.0, (double)(note/12.0) );
	
	increment = (float) freq/(float)samplerate;

	// reset the wave
	through = 0.0;
}

void ToneGen::NoteOff()
{
	note = 0;
}

float ToneGen::Clock( float note_offset, float adjust )
{
	float ret = 0.0;
	if( note == 0)
	{
		return ret;
	}

	float mod_increment;
	if( note_offset > 0 )
	{
		mod_increment = increment * (note_offset + 1.0);
	}
	else
	{
		mod_increment = increment / ((note_offset* -1.0) + 1.0);
	}

	through += mod_increment;
	assert( mod_increment < 1.0 );
	if( through > 1.0)
	{
		through -=1.0;
	}
	
	if( waveform == WAVEFORM_SAW )
	{
		//return( ((float)(random() & 255))/500.0 );
		return( (through*2.0) - 1.0 );
	}	

	if( waveform == WAVEFORM_SQUARE )
	{
		if( through < 0.5 )	
		{
			return -1.0;
		}
		else
		{
			return 1.0;
		}	
	}
	
	if( waveform == WAVEFORM_PWM )
	{
		if( through < (0.5 - (0.5 * adjust)) )	
		{
			return -1.0;
		}
		else
		{
			return 1.0;
		}	
	}
	
	if( waveform == WAVEFORM_NOISE )
	{
		int ran = random() & 255;	

		return (((float) ran) / 128.0 ) - 1.0;
	}
	return ret;
}

