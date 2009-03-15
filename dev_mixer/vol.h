
class Vol : public dkbClickReceiver
{
	public:
	void ReceiveClick( int clickref, int key );
	dkbShape *shape;
	dkbObj *dkb_obj;
	int vol;
	int ref;
	int offset;

	Vol( int def, int clickref, dkbObj *obj, int offset);	
	private:
	void ReDraw();
};

class VolPanel
{
	public:
	Vol *vols[15];
	VolPanel();
	private:
	dkbObj *dkb_obj;	
};
