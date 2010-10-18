
class PanelBtnEvRx
{
	public:
	virtual void Event( int ref, int event ) = 0;
};

class Panel 
{
	public:
	virtual bool SetPos (  int x, int y, int z )=0;
	virtual bool SetZ( int z )=0;

	virtual bool RemoveObjects( int ref )=0;
	virtual bool AddLine( int ref, int x1, int y1, int x2, int y2)=0;
	virtual bool AddLabel( int ref, int x, int y, int pitch, 
		const char *text )=0;
	virtual bool AddButton( int ref, int x, int y, PanelBtnEvRx *events )=0 ;
	virtual bool AddLabel( int ref, int x, int y, const char *text )=0;
	virtual bool AddVSlider( int ref, int x, int y, int size, int *ptr, int style )=0;
	virtual bool AddCheckbox( int ref, int x, int y, int *ptr) = 0;

	static Panel *CreatePanel();
};


