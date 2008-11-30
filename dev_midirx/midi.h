

#ifndef __MIDI_H_
#define __MIDI_H_

class MIDIReader
{
	public:

	enum State { wanting_channel, wanting_note, wanting_volume };
	State state;

	int channel;
	int note;
	int volume;

	public:
	MIDIReader();
	
	bool Input( int byte );
};		

#endif //__MIDI_H_


