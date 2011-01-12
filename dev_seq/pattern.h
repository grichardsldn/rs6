

// .pat format:
// pattern
// linear|grid|eventlist|tune
// for linear:
// 16
// [n|-|_],...,end
// for tune:
// 16
// [n|_],len,[n|_],len,end

class NoteList;

class Note
{
	public:
	int note_num;
	int length;
	int vel;
	int offset;
};
class seqTimemap;

class Pattern
{
	public:
	void Load( char *filename); 
	
	// write the set of notes into the notelist, returning
	// the location where the next pattern would start
	int Write( NoteList *nl,
		int location,
		int samplerate, 
		int tempo, 
		seqTimemap *timemap,
		int transpose,
		int midi_channel );

	private:
	int notelen;
	Note notes[256];
	int num_notes;
	seqTimemap *timemap;
};
	

