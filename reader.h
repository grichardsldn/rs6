class Reader
{
	public:
	bool OpenFile( const char *filename );
	char *NextWord();  // returns empty string if eof

	~Reader();
	int CurrentLine();

	private:
	bool isWhitespace( char val );
	bool isMonoWord( char val );
	bool isNewline( char vla );
	void SkipTillNewline();
	char return_buffer[1024];
	FILE *inptr;
	int line_number;
	int col_number;
	char filename[2048];
	int last_got;
};
