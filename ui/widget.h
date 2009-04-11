
class Widget : public dkbClickReceiver
{
	public:
	void ReceiveClick( int clickref, int key ) = 0;
	
	Widget *next;
	Widget *prev;
};

// this class doent do anything yet
class ValReceiver
{
	public:
	virtual void receiveValChange( int val ) = 0;
};

class VolWidget : public Widget
{
	public:
	void ReceiveClick( int clickref, int key );

	int getVol();
	int setVol();
	VolWidget( int def, int clickref, dkbObj *obj, int x_off, int y_off, ValReceiver *rcv);	

	private:
	dkbShape *shape;
	dkbObj *dkb_obj;
	int ref;
	int pos_x;
	int pos_y;
	int vol;


	private:
	void ReDraw();
	ValReceiver *val_receiver;
};

class WidgetPanel
{
	public:
	dkbObj *dkb_obj;	
	WidgetPanel( );
	void addWidget( Widget *widget );

	private:

	Widget *head;
	Widget *tail;
	int count;	

};
