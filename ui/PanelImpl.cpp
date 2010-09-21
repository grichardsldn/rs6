#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

#include "Panel.h"
#include "../darkbat/dkb.h"
#include "PanelImpl.h"
#include "icd_queue.h"

#define VSLIDER_MAGIC (0x10001900)

PanelImpl::PanelImpl()
{
	current_z = 0;
	dkb_object = new dkbObj();
	pos_x = pos_y = pos_z = 0;
	INIT_SET( &widgetset );	
}

bool PanelImpl::SetPos( int x, int y, int z )
{
	pos_x = x;
	pos_y = y;
	pos_z = z;
	dkbBlock block;
	dkbPos pos;
	pos.x = x;
	pos.y = y;	
	pos.z = z;
	dkb_object->project( block, pos );	
	next_ref = 1000;
	return false;
}

bool PanelImpl::SetZ( int new_z )
{
	current_z = new_z;	
	return true;
}



bool PanelImpl::RemoveObjects( int ref )
{
	return false;
}

bool PanelImpl::AddLine( int ref, int x1, int y1, int x2, int y2)
{
	int z1 = current_z;
	int z2 = current_z;

	RotateRight( &x1, &y1, &z1 );
	RotateRight( &x2, &y2, &z2 );
	dkbShape *shape = new dkbShape();
	shape->addLine( x1, y1, z1, x2, y2, z2,0 );

	dkbPos pos;
        pos.x = pos_x;
        pos.y = pos_y;
        pos.z = pos_z;

        dkbAngle angle;

	dkb_object->addShape( shape, angle, pos, ref );
	return true;
}

bool PanelImpl::AddLabel( int ref, int x, int y, int pitch, 
	const char *text )
{
	return false;
}

bool PanelImpl::AddButton( int ref, int x, int y, PanelBtnEvRx *events ) 
{
	return false;
}

bool PanelImpl::AddLabel( int ref, int x, int y, const char *text )
{
	return false;
}

void PanelImpl::AddFixedLine( dkbShape *shape, int x1, int y1, int z1, int x2, int y2, int z2 )
{
	RotateRight( &x1, &y1, &z1);
	RotateRight( &x2, &y2, &z2 );
	shape->addLine(  x1, y1, z1, x2, y2, z2, 0 );
}
	
void PanelImpl::DrawVSlider( VSliderPanelWidget *w )
{
	assert( w->magic == VSLIDER_MAGIC );

	dkb_object->removeShape( w->ref );
	
	dkbShape *shape = new dkbShape();
	int x = w->pos_x;
	int y = w->pos_y;
	int z = w->pos_z;
	//AddFixedLine( shape, x, y + w->size , z, x + 2, y + w->size, z );
	AddFixedLine( shape, x, y + *(w->value), z, x + 2, y + *(w->value), z );
        AddFixedClickTri(shape, x, y, z,
                                 x + 2, y, z,
                                x +1, y - 1, z,
                                  this, w->ref );
	dkbPos pos;
        pos.x = pos_x;
        pos.y = pos_y;
        pos.z = pos_z;

        dkbAngle angle;

	dkb_object->addShape( shape, angle, pos, w->ref );
}

void PanelImpl::DrawWidgets()
{
	printf( "GDR: DrawWidgets\n");
	PanelWidget *current = widgetset.head;
	while ( current != NULL )
	{
		printf("GDR: Calling DrawVSlider\n");
		DrawVSlider( (VSliderPanelWidget*)current );
		current = current->next;
	}
}

bool PanelImpl::AddVSlider( int ref, int x, int y, int size, int *ptr )
{
	printf("vslider: ptr=%08x, *ptr=%d\n", 
		(unsigned int)ptr, *ptr );
	VSliderPanelWidget *w = new VSliderPanelWidget();
	w->ref = ref;	
	w->pos_x = x;
	w->pos_y = y;
	w->pos_z = current_z;
	w->size = size;
	w->value = ptr;

	ENSET( &widgetset, w );

	DrawWidgets();
	return false;
}

VSliderPanelWidget::VSliderPanelWidget()
{
	magic = VSLIDER_MAGIC;
}

VSliderPanelWidget::~VSliderPanelWidget()
{
	magic = 0;
}


// fix the fucked up display where z is left to right and x is depth
void PanelImpl::RotateRight( int *x, int *y, int *z )
{
	int t = *z;	
	*z = *x * -1;
	*x = t ;
}

void PanelImpl::AddFixedClickTri( dkbShape *shape, int x1, int y1, int z1,
                int x2, int y2, int z2, int x3, int y3, int z3, dkbClickReceiver*callback, int ref )
{
	RotateRight( &x1,&y1,&z1 );
	RotateRight( &x2,&y2,&z2 );
	RotateRight( &x3,&y3,&z3 );
	shape->addClickTriangle( x1,y1,z1, x2,y2,z2, x3,y3,z3,	0,
		callback, ref );	
}

void PanelImpl::ReceiveClick( int shape_ref, int key )
{
	VSliderPanelWidget * w = (VSliderPanelWidget*)widgetset.head ; 
	while( w != NULL )
	{
		assert( w->magic == VSLIDER_MAGIC );
		if( shape_ref == w->ref )
		{
			printf ("GDR: Click found, ref=%08x, val=%d\n", 
				(int) w->value, *(w->value) );
			if( key == '1') 
			{	
				*(w->value) = *(w->value) - 1 ;
				if( *(w->value) < 0 ) *(w->value) = 0;
			}	
			else if ( key == '2')
			{
				*(w->value) = *(w->value) + 1 ;
				if( *(w->value) > w->size ) *(w->value) = w->size;
			}
			DrawWidgets();
			break;
		}	
		w = (VSliderPanelWidget*)w->next;
	}
}
