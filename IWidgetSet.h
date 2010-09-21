class IWidgetBtnEvRx
{
	public:
	virtual void Event( int ref, int event ) = 0;
};

class IWidgetSet 
{
	public:
	virtual bool CreatePanel( int ref, int x, int y, int z ) = 0;
	virtual bool RemovePanels( int ref ) = 0;
	virtual bool SetZ( int z ) = 0;

	virtual bool RemoveObjects( int ref ) = 0;
	virtual bool AddLine( int ref, int x1, int y1, int x2, int y2) = 0;
	virtual bool AddLabel( int ref, int x, int y, int pitch, 
		const char *text ) = 0;
	virtual bool AddButton( int ref, int x, int y, IWidgetBtnEvRx *events ) = 0;
	virtual bool AddLabel( int ref, int x, int y, const char *text ) = 0;
	virtual bool AddHSlider( int ref, int x, int y, int size, int *ptr ) = 0;
};


