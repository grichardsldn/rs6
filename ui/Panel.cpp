#include <stdio.h>
#include "Panel.h"
#include <pthread.h>
#include "../darkbat/dkb.h"
#include "PanelImpl.h"

Panel *Panel::CreatePanel()
{
	return new PanelImpl();
}


