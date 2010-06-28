/*
Example startup script

ctrl monitor 10 11 

ctrl create_dev strings strings1 ""
ctrl dev_out strings1 "main" 2
ctrl dev_midi_in strings1 "main" 1

ctrl create_dev sampdrum "sampdrum1" ""
ctrl dev_out sampdrum1 "main" 2
ctrl dev_midi_in sampdrum1 "main" 1

ctrl create_dev sub sub2 "dev_samp/strings.samp"
ctrl dev_out sub2 "main" 1
ctrl dev_midi_in sub2 "main" 2

ctrl create_dev mixer mixer1 ""
ctrl dev_in mixer1 "ch1" 1
ctrl dev_in mixer1 "ch2" 2
ctrl dev_in mixer1 "ch3" 3
ctrl dev_in mixer1 "ch4" 4
ctrl dev_out mixer1 "left" 10
ctrl dev_out mixer1 "right" 11

ctrl create_dev recorder rec1 ""
ctrl dev_in rec1 master.raw 10
ctrl dev_out rec1 lead.raw 3
ctrl dev_out rec1 vox.raw 4
 
	
ctrl create_dev seq seq1 "song.seq"

ctrl create_dev midirx "midi1" "/dev/midi1"
*/
class IManage;
class Reader;

class Script
{
	public:
	Script( IManage *manager);
	bool Run( const char *filename );
	private:
	bool monitor( Reader *reader, const char *word );
	bool create_dev( Reader *reader, const char *word );
	bool dev_midi_out( Reader *reader, const char *word);
	bool dev_midi_in( Reader *reader, const char *word);
	bool dev_out( Reader *reader, const char *word);
	bool dev_in( Reader *reader, const char *word);
	bool ctrl( Reader *reader );
	IManage *manager;
};
