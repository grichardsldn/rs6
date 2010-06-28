#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "sub102comp.h"

#define STATE_ATTACK  (1)
#define STATE_DECAY (2)
#define STATE_SUSTAIN  (3)
#define STATE_RELEASE (4)

namespace Sub102
{

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

double Slewer::Clock( double input, double rate )
{
	rate = rate * rate;
	//rate = rate *4800.0/samplerate;

	double diff = input - val;

	diff *= (rate * 0.1);
	//diff = diff / rate;
	val += diff;

	return (double)val;
}

// rate is max per ms.
double LinearSlewer::Clock( double input, double rate )
{
	rate = rate * rate;

	rate *= 3.0;

	double delta = rate * 4800.0 / samplerate ;
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

	samplerate = (double) a_samplerate;
	priv_attack = 5.0;
	decay = 0.2;
	sustain_level = 0.05;
	release = 0.1;

	level = 0.0;
	state = STATE_RELEASE;
}	

void ADSR::setAttack( double val )
{
	priv_attack = val;
}

double ADSR::getAttack()
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

double  ADSR::Scale( double diff, double rate )
{
	rate += 0.05;
	rate *= (rate*2.0);
	rate *= 4.0;
	return (  (diff / (samplerate / 48) ) * rate / 4.0 );
} 

double ADSR::Clock()
{	
	double diff;
	switch( state )
	{
		case STATE_ATTACK:
		
			//diff = 1.0 - level;
			diff = 1.0;
			level += Scale( diff, getAttack() ) *2.0;
			// ^ time 2 because attack wants to be faster

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
	samplerate = (double)a_samplerate;
	
	setRate( 0.1 );
	through = 0.0;
}

void LFO::setRate( double a_rate )
{
	rate = a_rate;
	increment = a_rate  / samplerate;
}

double LFO::getRate()
{
	return rate;
}

double LFO::Clock()
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
	double freq;
	
	note = a_note;
	note += 2; // Dunno why, dev_strings did this
	
	freq= 8.1758 * pow(2.0, (double)(note/12.0) );
	
	increment = (double) freq/(double)samplerate;

	// reset the wave
	through = 0.0;
}

void ToneGen::NoteOff()
{
	note = 0;
}

double ToneGen::Clock( double note_offset, double adjust )
{
	double ret = 0.0;
	if( note == 0)
	{
		return ret;
	}

	double mod_increment = 0.0 ;
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
		//return( ((double)(random() & 255))/500.0 );
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

		return (((double) ran) / 128.0 ) - 1.0;
	}
	return ret;
}

}
