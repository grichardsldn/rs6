#define NOTELIST_EVENT_NOTE_ON (1)
#define NOTELIST_EVENT_NOTE_OFF (2)
#define NOTELIST_EVENT_END (3)

class NoteListPrivate;

class NoteList
{
	public:
	NoteList();

	// Record a note in the list
	void AddEvent( int event, int channel, int note, int vel, int location );

	// clear the whole note list
	int Clear();

	// Move the playback counter to 0
	void Restart( );

	// get the number of samples till the next event
	int TimeTillNextEvent( );

	// move the playback counter on 1 sample
	void NextSample();

	// Consume the next event
	int GetNextEvent( int *event, int *channel, int *note, int *vel );

	private: 
	NoteListPrivate *priv;
};

