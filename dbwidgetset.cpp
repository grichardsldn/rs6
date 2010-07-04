#include <stdio.h>

#include "IWidgetSet.h"
#include "dbwidgetset.h"


bool DBWidgetSet::CreatePanel( int ref, int x, int y, int z )
{
	return false;
}

bool DBWidgetSet::RemovePanels( int ref )
{
	return false;
}

bool DBWidgetSet::SetZ( int z )
{
	return false;
}


bool DBWidgetSet::RemoveObjects( int ref )
{
	return false;
}

bool DBWidgetSet::AddLine( int ref, int x1, int y1, int x2, int y2)
{
	return false;
}

bool DBWidgetSet::AddButton( int ref, int x, int y, IWidgetBtnEvRx *events )
{
	return false;
}

bool DBWidgetSet::AddLabel( int ref, int x, int y, int pitch, const char *text )
{
	return false;
}

bool DBWidgetSet::AddVSlider( int ref, int x, int y, int size, int *ptr )
{
	return false;
}


