#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

#include "Panel.h"
#include "../darkbat/dkb.h"
#include "PanelImpl.h"
#include "icd_queue.h"
#include <ctype.h>

#define VSLIDER_MAGIC (0x10001900)

PanelImpl::PanelImpl()
{
	current_z = 0;
	dkb_object = new dkbObj();
	pos_x = pos_y = pos_z = 0;
	INIT_SET( &widgetset );	
}

void PanelImpl::GetText( char ch, bool &a, bool &b, bool &c, bool &d, bool&e, bool &f )
{
	ch = tolower(ch);
	a = b=c=d=e=f= false;
	switch (ch)
	{
		case 'a': a = true; break;
		case 'b': a = true; c = true; break;
		case 'c': a = true; b = true; break;
		case 'd': a = true; b = true; d = true; break;
		case 'e': a = true; d = true; break;
		case 'f': a = true; b = true; c=true;break;
		case 'g': a = true; b = true; c=true; d=true; break;
		case 'h': a = true; c = true; d=true;break;
		case 'i': c = true; b = true; break;
		case 'j': c = true; b = true; d=true; break;
		case 'k': a = true; e = true; break;
		case 'l': a = true; c = true; e=true; break;
		case 'm': a = true; b = true; e=true; break;
		case 'n': a = true; b = true; d=true; e=true; break;
		case 'o': c = true; b = true; e=true; break;
		case 'p': a = true; b = true; c=true; e=true; break;
		case 'q': a = true; b = true; c=true; d=true; e=true; break;
		case 'r': a = true; c = true; d=true; e=true;break;
		case 's': c = true; b = true; e=true; break;
		case 't': c = true; d=true; b = true; e=true; break;
		case 'u': a = true; e=true; f=true; break;
		case 'v': a = true; c = true; e=true; f=true; break;
		case 'w': c = true; b = true; d=true; f=true; break;
		case 'x': a = true; b = true; e=true; f=true;break;
		case 'y': a = true; b = true; d=true; e=true; f=true;break;
		case 'z': a = true; d = true; e=true; f=true; break;
	}
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
	dkbShape *shape = new dkbShape();
	while( *text != 0 ) {
		bool a,b,c,d,e,f;
		char t = *text;
		GetText( t, a,b,c,d,e,f );
		if ( a ) AddFixedPoint( shape, x, y +2, current_z );
		if ( b ) AddFixedPoint( shape, x + 1, y +2, current_z );
		if ( c ) AddFixedPoint( shape, x, y +1, current_z );
		if ( d ) AddFixedPoint( shape, x + 1, y +1, current_z );
		if ( e ) AddFixedPoint( shape, x, y , current_z );
		if ( f ) AddFixedPoint( shape, x + 1, y , current_z );

		x += 2;
		text++;
	}
	dkbPos pos;
        pos.x = pos_x;
        pos.y = pos_y;
        pos.z = pos_z;

        dkbAngle angle;

	dkb_object->addShape( shape, angle, pos, ref );
	return false;
}

bool PanelImpl::AddButton( int ref, int x, int y, PanelBtnEvRx *events ) 
{
	
        ButtonPanelWidget *w = new ButtonPanelWidget();
        w->ref = ref;
        w->pos_x = x;
        w->pos_y = y;
        w->pos_z = current_z;
        w->receiver = events;
        w->type = WBUTTON;

        ENSET( &widgetset, w );

        DrawWidgets();
        return true;
}


void PanelImpl::AddFixedLine( dkbShape *shape, int x1, int y1, int z1, int x2, int y2, int z2 )
{
	RotateRight( &x1, &y1, &z1);
	RotateRight( &x2, &y2, &z2 );
	shape->addLine(  x1, y1, z1, x2, y2, z2, 0 );
}

void PanelImpl::AddFixedPoint( dkbShape *shape, int x1, int y1, int z1 )
{
	RotateRight( &x1, &y1, &z1);
	// this doesnt work at the mo:
	//shape->addPoint(  x1, y1, z1,0 );
	shape->addLine(  x1, y1, z1, x1+1,y1,z1,0 );
}


void PanelImpl::DrawCheckbox( CheckboxPanelWidget *w )
{
	dkb_object->removeShape( w->ref );
	
	dkbShape *shape = new dkbShape();
	int x = w->pos_x;
	int y = w->pos_y;
	int z = w->pos_z;
	if( *(w->value) == 0 )
	{
		// cross
		AddFixedLine(shape,  x, y, z, x + 2, y + 2, z );
		AddFixedLine(shape,  x, y + 2, z, x + 2, y, z );
	}
	else
	{
		// tick
		AddFixedLine(shape,  x, y+1, z, x + 1, y , z );
		AddFixedLine(shape,  x+ 1, y, z, x + 2, y + 2 , z );
	}

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

void PanelImpl::DrawButton( ButtonPanelWidget *w )
{
	dkb_object->removeShape( w->ref );
	
	dkbShape *shape = new dkbShape();
	int x = w->pos_x;
	int y = w->pos_y;
	int z = w->pos_z;
        AddFixedClickTri(shape, x, y, z,
                                 x + 2, y, z,
                                x +1, y + 2, z,
                                  this, w->ref );

	dkbPos pos;
        pos.x = pos_x;
        pos.y = pos_y;
        pos.z = pos_z;

        dkbAngle angle;

	dkb_object->addShape( shape, angle, pos, w->ref );
}
	
void PanelImpl::DrawVSlider( VSliderPanelWidget *w )
{
	assert( w->magic == VSLIDER_MAGIC );

	dkb_object->removeShape( w->ref );
	
	dkbShape *shape = new dkbShape();
	int x = w->pos_x;
	int y = w->pos_y;
	int z = w->pos_z;
	switch( w->style )
	{
		case 1:
	AddFixedLine( shape, x+1, y+ *(w->value), z, x + 1, y + w->size, z + 30 );
        AddFixedClickTri(shape, x, y, z,
                                 x + 2, y, z,
                                x +1, y - 1, z,
                                  this, w->ref );
		break;
		default:

	AddFixedLine( shape, x + 1, y, z, x + 1, y + w->size, z );
	AddFixedLine( shape, x, y + *(w->value), z, x + 2, y + *(w->value), z );
        AddFixedClickTri(shape, x, y, z,
                                 x + 2, y, z,
                                x +1, y - 1, z,
                                  this, w->ref );
		break;
	}
		
	dkbPos pos;
        pos.x = pos_x;
        pos.y = pos_y;
        pos.z = pos_z;

        dkbAngle angle;


	dkb_object->addShape( shape, angle, pos, w->ref );
}

void PanelImpl::DrawWidgets()
{
	//printf( "GDR: DrawWidgets\n");
	PanelWidget *current = widgetset.head;
	while ( current != NULL )
	{
		if( current->type == WVSLIDER )
		{
			//printf("GDR: Calling DrawVSlider\n");
			DrawVSlider( (VSliderPanelWidget*)current );
		}
		if( current->type == WCHECKBOX )
		{
			DrawCheckbox( (CheckboxPanelWidget*)current );
		}
		if( current->type == WBUTTON )
		{
			DrawButton( (ButtonPanelWidget*)current );
		}
		current = current->next;
	}
}

bool PanelImpl::AddCheckbox( int ref, int x, int y, int *ptr)
{	
	CheckboxPanelWidget *w = new CheckboxPanelWidget();
	w->ref = ref;
	w->pos_x = x;
	w->pos_y = y;
	w->pos_z = current_z;
	w->value = ptr;
	w->type = WCHECKBOX;

	ENSET( &widgetset, w );

	DrawWidgets();
	return true;
}


bool PanelImpl::AddVSlider( int ref, int x, int y, int size, int *ptr, int style )
{
	//printf("vslider: ptr=%08x, *ptr=%d\n", 
	//	(unsigned int)ptr, *ptr );
	VSliderPanelWidget *w = new VSliderPanelWidget();
	w->ref = ref;	
	w->pos_x = x;
	w->pos_y = y;
	w->pos_z = current_z;
	w->size = size;
	w->value = ptr;
	w->type = WVSLIDER;
	w->style = style;

	ENSET( &widgetset, w );

	DrawWidgets();
	return false;
}

void ButtonPanelWidget::ReceiveClick( int key )
{
	if( receiver != NULL )
	{
		receiver->Event( ref, key );
	}
}

ButtonPanelWidget::ButtonPanelWidget()
{
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

void CheckboxPanelWidget::ReceiveClick( int key )
{
	if( key == '1') 
	{	
		*(value) = *(value) - 1 ;
		if( *(value) < 0 ) *(value) = 0;
	}	
	else if ( key == '2')
	{
		*(value) = *(value) + 1 ;
		if( *(value) > 1 ) *(value) = 1;
	}
}

void VSliderPanelWidget::ReceiveClick( int key )
{
	assert( magic == VSLIDER_MAGIC );
	if( key == '1') 
	{	
		*(value) = *(value) - 1 ;
		if( *(value) < 0 ) *(value) = 0;
	}	
	else if ( key == '2')
	{
		*(value) = *(value) + 1 ;
		if( *(value) > size ) *(value) = size;
	}
}

void PanelImpl::ReceiveClick( int shape_ref, int key )
{
	PanelWidget * w = widgetset.head ; 
	while( w != NULL )
	{
		if( shape_ref == w->ref )
		{
			switch( w->type )
			{
			case WVSLIDER:
			{
				 VSliderPanelWidget * svw = 
					(VSliderPanelWidget*)w;
				svw->ReceiveClick( key );
			}
			break;
			case WCHECKBOX:
			{
				 CheckboxPanelWidget * cw = 
					(CheckboxPanelWidget*)w;
				cw->ReceiveClick( key );
			}
			case WBUTTON:
			{
				 ButtonPanelWidget * cw = 
					(ButtonPanelWidget*)w;
				cw->ReceiveClick( key );
			}
			break;
			}
			DrawWidgets();
		}	
		w = (VSliderPanelWidget*)w->next;
	}
}
