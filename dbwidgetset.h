
class WSPanel
{
	public:
};

class WSWidget
{
	public: 
	const char *getType();
	int ref;
};

class WSLine : public WSWidget
{
	public:
	const char *getType();
	int x1, y1, z1;
	int	x2,y2,z2;
};

class WSLabel : public WSWidget
{
	public:
	const char *getType();
	int x,y,z;
	int pitch;
	const char *text;
};
	
class WSButton : public WSWidget
{
	public:
	const char *getType();
	int x,y,z;
	IWidgetBtnEvRx *events;
};

class WSVSlider : public WSWidget
{
	public:
	const char *getType();
	int x,y,z;	
	int size;
	int *data;
};		
	
class DBWidgetSet : public IWidgetSet
{
	public:
	// IWidgetSet
	virtual bool CreatePanel( int ref, int x, int y, int z );
	virtual bool RemovePanels( int ref );
	virtual bool SetZ( int z );

	virtual bool RemoveObjects( int ref );
	virtual bool AddLine( int ref, int x1, int y1, int x2, int y2);
	virtual bool AddLabel( int ref, int x, int y, int pitch, 
		const char *text );
	virtual bool AddButton( int ref, int x, int y, IWidgetBtnEvRx *events );
	virtual bool AddVSlider( int ref, int x, int y, int size, int *data );

	private:
	//list<WSPanel*> panels;
	//list<WSWidget*> widgets;
	int z;
};

