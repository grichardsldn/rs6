
class dkbElement;

class dkbClickReceiver
{
	public:
	virtual void ReceiveClick( int shape_ref, int key ) = 0;
};

// shapes are relative to 0-0-0.
class dkbShape
{
	public:	
	void addLine( int x1, int y1, int z1, int x2, int y2, int z2, int col);
	void addPoint( int x1, int x2, int y2, int col);
	void addFlatRect( int x, int y, int z, int x2, int y2, int z2, int col );
	void addClickTriangle( int x1, int y1, int z1,
				int x2, int y2, int z2,
				int x3, int y3, int z3, 
				int col, dkbClickReceiver *callback, int click_ref );

	dkbShape();

	dkbElement *head;
	//bool signalClick( int clickref );

	void RxPress(int clickref, int key );

	private:
	void addElement( dkbElement *element );
};

class dkbAngle
{
};
	   

class dkbPos
{
	public:
	int x; 
	int y;
	int z;

	char * write( char *buf );
};

class dkbBlock
{
	public:
	unsigned short block_x;
	unsigned short block_y;
	unsigned char block_z;
};

class dkbShapeEntry;
class UDPSocket;

class dkbObj
{
	public:
	void addShape( dkbShape *shape, dkbAngle angle, dkbPos
		traslation, int ref);
	void removeShape( int ref);

	bool connect( dkbBlock block);
	void project( dkbBlock block, dkbPos position  );
	void moveRel( dkbPos pos );

	dkbObj();

	static void *start_send_thread(void *ptr);
	static void *start_receive_thread(void *ptr);
	void StartSendThread();
	void StartReceiveThread();

	// internal methods
	void RxPress( int clickref, int key );
	
	private:
	void Xmit();
	void Changed();
	dkbShapeEntry *shapes[15];
	dkbPos position;
	pthread_t send_thread;
	pthread_t receive_thread;
	UDPSocket *txrx_socket;
	int ref;
	bool projecting;
};


