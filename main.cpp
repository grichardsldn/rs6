#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "IDevice.h"
#include "IManage.h"

#include "RS7Mapper.h"
#include "script.h"
#include "rs7global.h"
#include <string.h>
#include <stdlib.h>

char *rs7_lib_path;

int main(int argc, char **argv, char *envp[])
{
	rs7_lib_path = (char *)new char[1000];
	if( getenv("RS7_HOME") == NULL )
	{	
		strcpy( rs7_lib_path, "/home/graham/svn/RS7");
	}
	else
	{
		strcpy( rs7_lib_path, getenv("RS7_HOME"));
	}

	//if( argc != 2 )
	//{
//		printf("Please specify sequence file\n");
//		exit(1);
//	}	

	int samplerate = 48000;
	
	RS7Mapper *mapper = new RS7Mapper(samplerate, "/dev/dsp");

	IManage *ctrl;

	ctrl = mapper->GetControl();

	Script *script = new Script( ctrl );
	script->Run( argv[1] );

//	ctrl->Monitor(10, 11 );
//	//ctrl->Monitor(1, 2 );

//	int id1;
//	//id1 = ctrl->CreateDevice( "strings", "instance1", "" );
//	//ctrl->SetDeviceOutput( id1, "output", 2);
//	//ctrl->SetDeviceMidiInput(id1, "", 1 );
//
//
//	id1 = ctrl->CreateDevice("sampdrum", "sampdrum1", "" );
//	ctrl->SetDeviceOutput( id1, "output", 2 );
//	ctrl->SetDeviceMidiInput( id1, "", 1 );
//
//	int id2;
//	id2 = ctrl->CreateDevice( "sub", "instance2", "dev_samp/strings.samp" );
//	ctrl->SetDeviceOutput(id2, "output", 1);
//	ctrl->SetDeviceMidiInput(id2, "", 2 );
//
//	int id3;
//	id3= ctrl->CreateDevice("mixer", "inst1", "");
//	ctrl->SetDeviceInput(id3,"", 1);
//	ctrl->SetDeviceInput(id3,"", 2);
//	ctrl->SetDeviceInput(id3,"", 3);
//	ctrl->SetDeviceInput(id3,"", 4);
//	ctrl->SetDeviceOutput(id3,"left", 10);
//	ctrl->SetDeviceOutput(id3,"right", 11);
//
//	int id_rec;
//	//id_rec = ctrl->CreateDevice("recorder", "rec1", "");
//	//c//trl->SetDeviceInput( id_rec,"test.raw", 10 );
//	//ctrl->SetDeviceOutput( id_rec,"lead.raw", 3 );
//	////ctrl->SetDeviceOutput( id_rec,"vox.raw", 4 );
//
//
//	if( argc == 2 )
//	{
//		int seq1;
//		seq1 = ctrl->CreateDevice( "seq", "seq1", argv[1]);
//	}
//
//	int midi1;
////	midi1 = ctrl->CreateDevice( "midirx", "midi1", "/dev/midi1");
//
	ctrl->TransportPlay();	
	sleep(1000);
	ctrl->TransportStop();
}
