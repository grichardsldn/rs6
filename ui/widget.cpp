#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../darkbat/dkb.h"
#include "widget.h"

#include "icd_queue.h"

WidgetPanel::WidgetPanel( )
{
	INIT_QUEUE( this );
	dkb_obj = new dkbObj();
	dkbBlock block;	
	dkbPos pos;
	pos.x = 5;
	pos.y = 5;
	pos.z = 5;
	dkb_obj->project( block, pos );
}

VolWidget::VolWidget( int def, int a_ref, dkbObj *a_obj, int x_off, int y_off,
	ValReceiver *recv )
{
	vol = def;
	ref = a_ref;
	dkb_obj = a_obj;
	pos_x = x_off;
	pos_y = y_off;
	val_receiver = recv;

	shape = NULL;
	
	ReDraw();

}

void VolWidget::ReDraw()
{
	dkb_obj->removeShape( ref );
	if (shape != NULL )
	{
		delete shape;
	}

	int x = 1;
	int y = 3;
	int z = 1;
	shape = new dkbShape();		
	//shape->addLine( x, y,z , x , y, z + 2,0 );
	shape->addLine( x, y + 10, z, x , y + 10, z+2,0);
	shape->addLine( x - 1, y + vol, z , x - 1, y + vol, z+2 ,0 );
	shape->addClickTriangle(x, y, z,
				 x, y, z + 2 ,
				x, y - 1, z + 1,
				 0, this, ref );

	dkbPos pos;
	pos.x = pos_y;
	pos.y = 0;
	pos.z = pos_x;

	dkbAngle angle;
	printf("Calling addShape from ReDraw\n");
	dkb_obj->addShape( shape, angle, pos, ref);
	printf("Called addShape from ReDraw\n");
}

void VolWidget::ReceiveClick( int clickref, int key )
{
	if( key == '1' )
	{
		if( vol > 0 ) vol --;
	}
	if( key =='2' )
	{
		if( vol < 10 ) vol ++;
	}
	printf("vol=%d\n", vol);
	ReDraw();
}

int VolWidget::getVol()
{
	return vol;
}

void WidgetPanel::addWidget( Widget *widget )
{
	ENQUEUE( this, widget );
}
