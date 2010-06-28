#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "IDevice.h"

// BEGIN DEVICES
#include "dev_testdevice/testdevice.h"
#include "dev_stepseq/stepseq.h"
#include "dev_seq/seq.h"
#include "dev_midirx/midirx.h"
#include "dev_strings/strings.h"
#include "dev_sampdrum/sampdrum.h"
#include "dev_mixer/mixer.h"
#include "dev_sub/sub.h"
#include "dev_recorder/recorder.h"
#include "dev_samp/samp.h"
#include "dev_smodel/smodel.h"
#include "dev_sub102/sub102comp.h"
#include "dev_sub102/sub102.h"


// END DEVICES

IDevice *create_device( const char *device_type )
{
	assert( device_type );

	if( strcmp(device_type, "testdevice") == 0)
	{
		DeviceTestDevice *dev = new DeviceTestDevice;

		return dev;
	}

	if( strcmp(device_type, "stepseq") == 0)
	{
		DeviceStepSeq *dev = new DeviceStepSeq;
		return dev;
	}

	if( strcmp(device_type, "seq") == 0 )
	{
		DeviceSeq *dev = new DeviceSeq;
		return dev;
	}

	if( strcmp(device_type, "midirx") == 0 )
	{
		DeviceMidiRX *dev= new DeviceMidiRX;
		return dev;
	}

	if( strcmp(device_type, "strings") == 0)
	{
		DeviceStrings *dev = new DeviceStrings;
		return dev;
	}

	if( strcmp(device_type, "samp") == 0)
	{
		IDevice *dev = new DeviceSamp;
		return dev;
	}

	if( strcmp(device_type, "mixer") == 0)
	{
		IDevice *dev = new DeviceMixer;
		return dev;
	}

	if( strcmp(device_type, "sampdrum") == 0)
	{
		DeviceSampDrum *dev = new DeviceSampDrum;
		return dev;
	}

	if( strcmp(device_type, "sub") == 0)
	{
		IDevice *dev = new DeviceSub;
		return dev;
	}

	if( strcmp(device_type, "recorder") == 0)
	{
		IDevice *dev = new DeviceRecorder;
		return dev;
	}

	if( strcmp( device_type, "smodel") == 0)
	{
		IDevice *dev = new DeviceSModel;
		return dev;
	}
	if( strcmp( device_type, "sub102") == 0)
	{
		IDevice *dev = new DeviceSub102;
		return dev;
	}
	return NULL;
}
	
