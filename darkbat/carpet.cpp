#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "dkb.h"


int main( int argc, char **argv)
{
	dkbObj *dkb_obj = new dkbObj();
        dkbShape *shape = new dkbShape();
        //shape->addLine( x, y,z + offset , x , y, z + 2 + offset ,0 );

	for( int x= -10000; x < 10000; x +=3000 )
	{
		for( int z = -10000 ; z < 10000 ; z += 3000)
		{
			shape->addLine( x,0, z,x + 10, 0, z, 0 );
		}		
	}
        dkbAngle angle;

        dkbBlock block;
        dkbPos pos;
        pos.x = 0;
        pos.y = 0;
        pos.z = 0;
        dkb_obj->addShape( shape, angle, pos, 0);
        dkb_obj->project( block, pos );

	sleep(30);
}
