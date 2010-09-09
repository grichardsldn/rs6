#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <pthread.h>
#include "dkb.h"
#include "UDPSocket.h"
#include "dkb_const.h"

#define ELEMENT_NONE (100)
#define ELEMENT_LINE (1)
#define ELEMENT_POINT ( 2)
#define ELEMENT_FLATRECT (3)
#define ELEMENT_CLICKTRI (4)

char *writeInt( char *buf, int val)
{
	signed short sval = (signed short) val;

	*((signed short*)buf) = sval;	

	return buf + 2;
}
	
char *dkbPos::write( char *buf )
{
	printf("GDR: dkbPos::write: writing %d %d %d\n",
		x,y,z );
	buf = writeInt( buf, x );
	buf = writeInt( buf, y );
	buf = writeInt( buf, z );
}


class dkbShapeEntry
{
	public:
	dkbShape *shape;
	bool allocated;
	dkbPos position;
	int ref;

	void RxPress( int clickref, int key );
	dkbShapeEntry();	
};

class dkbElement
{
	public:
	int x1; 
	int y1;
	int z1;
	int x2;
	int y2;
	int z2;
	int x3;
	int y3;
	int z3;
	int col;
	int type;
	dkbClickReceiver *receiver;
	int clickref;

	dkbElement* next;

	dkbElement();
	char *write( char *buf);
	void RxPress( int key );
};

void dkbShapeEntry::RxPress(int clickref, int key )
{
	assert( shape != NULL);
	shape->RxPress( clickref, key );
}

char *dkbElement::write( char *buf )
{
	switch( type )
	{
		case ELEMENT_CLICKTRI:
		{
			buf = writeInt( buf, DKB_ELEMENT_CLICKTRI );

			// x1, y1, z1, x2, y2, z2, col 
			buf = writeInt( buf, x1 );
			buf = writeInt( buf, y1 );
			buf = writeInt( buf, z1 );
			buf = writeInt( buf, x2 );
			buf = writeInt( buf, y2 );
			buf = writeInt( buf, z2 );
			buf = writeInt( buf, x3 );
			buf = writeInt( buf, y3 );
			buf = writeInt( buf, z3 );
			buf = writeInt( buf, col );
			buf = writeInt( buf, clickref );
		}
		break;

		case ELEMENT_LINE:
		{
			buf = writeInt( buf, DKB_ELEMENT_LINE );

			// x1, y1, z1, x2, y2, z2, col 
			buf = writeInt( buf, x1 );
			buf = writeInt( buf, y1 );
			buf = writeInt( buf, z1 );
			buf = writeInt( buf, x2 );
			buf = writeInt( buf, y2 );
			buf = writeInt( buf, z2 );
			buf = writeInt( buf, col );
		} 
		break;
	}
}

dkbElement::dkbElement()
{
	type = ELEMENT_NONE;
	next = NULL;
	receiver = NULL;
	clickref = -1;
}

void dkbElement::RxPress(int key )
{	
	printf("dkbElement::RxPress: clickref=#%d, key=#%d\n", clickref, key );
	if( receiver != NULL)
	{		
		receiver->ReceiveClick( clickref, key );
	}
}

void dkbShape::RxPress( int clickref, int key )
{
	printf("dkbShape::RxPress\n");
	dkbElement *cur = head;
	while ( cur != NULL )
	{		
		if( cur->clickref == clickref )
		{	
			cur->RxPress( key );
		}

		cur = cur->next;
	}
}

void dkbShape::addElement( dkbElement *element )
{
	element->next = head;	
	head = element;
}

void dkbShape::addClickTriangle( int an_x1, int a_y1, int a_z1,
				int an_x2, int a_y2, int a_z2,
				int an_x3, int a_y3, int a_z3,
				int a_col,
				dkbClickReceiver *a_callback,
				int a_clickref )
{
	dkbElement *e = new dkbElement();
	e->type = ELEMENT_CLICKTRI;
	e->x1 = an_x1;	
	e->y1 = a_y1;
	e->z1 = a_z1;
	e->x2 = an_x2;
	e->y2 = a_y2;
	e->z2 = a_z2;
	e->x3 = an_x3;
	e->y3 = a_y3;
	e->z3 = a_z3;
	e->col = a_col;
	e->receiver = a_callback;
	e->clickref = a_clickref;
	addElement( e );
}


void dkbShape::addLine( int an_x1, int a_y1, int a_z1,
			int an_x2, int a_y2, int a_z2,
			int a_col)
{
	dkbElement *e = new dkbElement();
	e->type = ELEMENT_LINE;
	e->x1 = an_x1;	
	e->y1 = a_y1;
	e->z1 = a_z1;
	e->x2 = an_x2;
	e->y2 = a_y2;
	e->z2 = a_z2;
	e->col = a_col;
	
	addElement( e );
}

void dkbShape::addPoint( int ax_x1, int a_y1, int a_1, int a_col)
{
}

dkbShapeEntry::dkbShapeEntry()
{
	allocated = false;
}

dkbObj::dkbObj()
{
	FILE *inptr = fopen("/dev/urandom", "rb");
	int anint;
	fread( &ref, 4, 1, inptr );
	fclose( inptr );
	ref &= 0x7fff;

	for (int i = 0 ; i < 15 ; i++)
	{
		shapes[i] = new dkbShapeEntry();
	}

	projecting = false;
}

void dkbObj::Changed()
{
	if( projecting )
	{
		printf("Calling XMmit()\n");
		Xmit();
		printf("Called XMmit()\n");
	}
}

void dkbObj::addShape( dkbShape *shape, dkbAngle angle, dkbPos trans,
	int ref)
{
	assert( shape != NULL );

	int found = -1;
	for (int i = 0 ; i < 15 ; i++)
	{
		if( shapes[i]->allocated == false)	
		{
			found = i;
			break;
		}
	}
	if( found == -1 ) 
	{
		printf("dkbObj: too many shapes\n");
		return;
	}

	shapes[found]->shape = shape;
	shapes[found]->position = trans;
	shapes[found]->ref = ref;
	shapes[found]->allocated = true;

	Changed();
}

void dkbObj::RxPress( int clickref, int key )
{
	printf("RxPress\n");
	for (int i = 0 ; i < 15 ; i++)
	{
		printf("iteration %d\n", i );
		if( shapes[i]->allocated == true)	
		{
			shapes[i]->RxPress( clickref, key );
		}
	}
}

bool dkbObj::connect( dkbBlock block )
{
	return false;
}

void dkbObj::removeShape( int ref )
{
	int found = -1;
	for (int i = 0 ; i < 15 ; i++)
	{
		if( shapes[i]->ref == ref)	
		{
			shapes[i]->allocated = false;
		}
	}
	Changed();	
}

void dkbObj::project( dkbBlock block, dkbPos pos )
{
	txrx_socket = new UDPSocket();
	txrx_socket->Bind(0,0);

	position.x = pos.x;
	position.y = pos.y;
	position.z = pos.z;

	//txrx_socket->SetTarget(0xc0a80147, 1234);
	txrx_socket->SetTarget(0x7f000001, 1234);

	pthread_create( &send_thread, NULL, start_send_thread, this);
	pthread_create( &receive_thread, NULL, start_receive_thread, this);

	projecting = true;
}

void dkbObj::moveRel( dkbPos offset )
{
}	

dkbShape::dkbShape()
{
	head = NULL;
}	

void *dkbObj::start_receive_thread( void * a_this)
{
	dkbObj *obj = (dkbObj*) a_this;
	obj->StartReceiveThread();

	return NULL;
}

void *dkbObj::start_send_thread( void * a_this)
{
	dkbObj *obj = (dkbObj*) a_this;
	obj->StartSendThread();

	return NULL;
}

void dkbObj::StartReceiveThread()
{
	char buffer[1500];
	while(1)
	{
		txrx_socket->ReceiveFrom( &buffer[0], 1500, NULL, NULL );
		int *ip = (int*)&buffer[0];
		int *key = (int*)&buffer[4];
		printf("ReceiveThread: Received packet: clickref %d\n",
			*ip);
		for ( int obj = 0 ; obj<15; obj++)
		{
			if( shapes[obj]->allocated == true )
			{
				shapes[obj]->RxPress( *ip, *key );
			}
		}
	}
}

void dkbObj::StartSendThread()
{
	while( 1)
	{
		Xmit();
		sleep(1);
	}
}

void dkbObj::Xmit()
{
	char buffer[1024];

	char *bp = &buffer[0];

	// write the magic number and the reference number
	bp = writeInt( bp, DKB_1_MAGIC );

	bp = writeInt( bp, DKB_CONNECTION_REF );
	printf("GDR: Writing ref #%d\n", ref );
	bp = writeInt( bp, ref );

	// write the object poition
	bp = writeInt( bp, DKB_POS );
	bp = position.write( bp );

	printf("Xmit shapes are:\n");
	for( int i = 0 ; i <15 ; i++)
	{
		if (shapes[i]->allocated)
		{
			// write the ref for this shape
			bp = writeInt( bp, DKB_REF );
			bp = writeInt( bp, shapes[i]->ref );

			// write the position for this shape
			bp = writeInt( bp, DKB_RELPOS );
			bp = shapes[i]->position.write( bp );
		
			dkbElement *cur = shapes[i]->shape->head;
			while ( cur != NULL )
			{		
				bp = cur->write(bp );

				cur = cur->next;
			}
			
		}
	}
		bp = writeInt( bp, DKB_END );

		txrx_socket->Send( &buffer[0], (int)(bp - &buffer[0]) );
}
/*
class Receiver : public dkbClickReceiver
{
	public:
	void ReceiveClick( int clickref, int key );
	char name[1024];
	Receiver( char *aname);
};

Receiver::Receiver( char *aname)
{
	strcpy( name, aname);
}

void Receiver::ReceiveClick( int clickref, int key )
{
	printf("Receiver %s got clickref %d, key %d\n",
		name, clickref, key );
}


main()
{
	dkbShape cube;
	// lower face 
	cube.addLine( 0,0,0, 1,0,0, 0);
	cube.addLine( 1,0,0, 1,0,1, 0);
	cube.addLine( 1,0,1, 0,0,1, 0);
	cube.addLine( 0,0,1, 0,0,0, 0);

	// upper face
	cube.addLine( 0,1,0, 1,1,0, 0);
	cube.addLine( 1,1,0, 1,1,1, 0);
	cube.addLine( 1,1,1, 0,1,1, 0);
	cube.addLine( 0,1,1, 0,1,0, 0);

	// connect the two
	cube.addLine( 0,0,0, 0,1,0, 0 );
	cube.addLine( 1,0,0, 1,1,0, 0 );
	cube.addLine( 1,0,1, 1,1,1, 0 );
	cube.addLine( 0,0,1, 0,1,1, 0 );


	Receiver *rxa = new Receiver("A");
	Receiver *rxb = new Receiver("B");

	dkbShape tri;
	tri.addClickTriangle( 0,0,0, 1,1,1, 3,1,2, 0, rxa,100 );
	dkbShape tri2;
	tri2.addClickTriangle( 3,0,0, 1,1,1, 3,1,2, 0, rxb ,101 );

	dkbObj obj;
	dkbPos pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	dkbAngle angle;
	obj.addShape( &cube ,angle, pos,  0 );	
	dkbBlock block;	
	obj.project( block, pos );

	usleep(300000);
	pos.x = 0;
	pos.y = 5;
	pos.z = 0;
	obj.addShape( &cube ,angle, pos,  1 );	
	usleep(300000);
	obj.addShape( &tri ,angle, pos,  2 );	
	obj.addShape( &tri2 ,angle, pos,  3 );	


	sleep(8);
	//obj.removeShape( 2 );	
	sleep(10);
}	
*/
