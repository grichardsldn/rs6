#define MAX_SETTINGS (100)
#define DATATYPE_FLOAT (1)
#define DATATYPE_INT (2)
#define DATATYPE_STRING (3)

class Settings
{
	public:

	Settings( char *filename );
	void AddSetting( char *name, float *addr );
	void AddSetting( char *name, int *addr );
	void AddSetting( char *name, char *addr );

	void Write();
	void Read();
	private:
	int FindSetting( char *name );
	void AddSetting( char *name, int datatype, void *ptr );
	char *filename;
	int num_settings;
	char *names[MAX_SETTINGS];
	void *ptrs[MAX_SETTINGS];
	int datatypes[MAX_SETTINGS];
};
	
