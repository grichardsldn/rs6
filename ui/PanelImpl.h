// requires Panel.h
// ../darkbat/dbk.h

class PanelWidget
{
	public:
	PanelWidget *next;
	PanelWidget *prev;
	int type;
	int ref;
};


class PanelWidgetSet
{
	public:
	PanelWidget *tail;
	PanelWidget *head;
	int count;
	PanelWidget *before;	
	PanelWidget *after;	
};

class VSliderPanelWidget : public PanelWidget
{
	public:
	VSliderPanelWidget();
	~VSliderPanelWidget();
	int *value;
	int size;
	int pos_x;
	int pos_y;
	int pos_z;
	int magic;
};


class PanelImpl : public Panel, dkbClickReceiver
{
	public:
	virtual bool SetPos( int x, int y, int z );
	virtual bool SetZ( int z );

	virtual bool RemoveObjects( int ref );
	virtual bool AddLine( int ref, int x1, int y1, int x2, int y2);
	virtual bool AddLabel( int ref, int x, int y, int pitch, 
		const char *text );
	virtual bool AddButton( int ref, int x, int y, PanelBtnEvRx *events ) ;
	virtual bool AddLabel( int ref, int x, int y, const char *text );
	virtual bool AddVSlider( int ref, int x, int y, int size, int *ptr );
	virtual void ReceiveClick( int shape_ref, int key );
	PanelImpl();

	private:
	int current_z;
	int pos_x, pos_y, pos_z;
	dkbObj *dkb_object;
	void RotateRight( int *x, int *y, int *z );
	PanelWidgetSet widgetset;
	int next_ref;	
	void DrawVSlider( VSliderPanelWidget *w );
	void DrawWidgets();	
	void AddFixedLine( dkbShape *shape,int x1, int y1,int z1, int x2,int y2,int z2 );
	void AddFixedClickTri( dkbShape *shape, int x1, int y1, int z1, 
		int x2, int y2, int z2, int x3, int y3, int z3, dkbClickReceiver*callback, int ref );
};


