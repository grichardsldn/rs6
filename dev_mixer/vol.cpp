#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "/home/user/svn2/Darkbat/api/dkb.h"
#include "vol.h"

VolPanel::VolPanel()
{
	dkb_obj = new dkbObj();

	for( int i = 0 ; i < 15 ; i++)
	{
		vols[i] = new Vol( 2, 300 + i, dkb_obj, i * 3 );
	}

	dkbBlock block;	
	dkbPos pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	dkb_obj->project( block, pos );
}

Vol::Vol( int def, int a_ref, dkbObj *a_obj, int a_offset )
{
	vol = def;
	ref = a_ref;
	dkb_obj = a_obj;
	offset = a_offset;

	shape = NULL;
	
	ReDraw();

}

void Vol::ReDraw()
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
	//shape->addLine( x, y,z + offset , x , y, z + 2 + offset ,0 );
	shape->addLine( x, y + 10, z + offset , x , y + 10, z+2 + offset ,0);
	shape->addLine( x - 1, y + vol, z + offset , x - 1, y + vol, z+2 + offset ,0 );
	shape->addClickTriangle(x, y, z + offset ,
				 x, y, z + 2 + offset ,
				x, y - 1, z + 1 + offset,
				 0, this, ref );

	dkbPos pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;

	dkbAngle angle;
	printf("Calling addShape from ReDraw\n");
	dkb_obj->addShape( shape, angle, pos, ref);
	printf("Called addShape from ReDraw\n");
}

void Vol::ReceiveClick( int clickref, int key )
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
