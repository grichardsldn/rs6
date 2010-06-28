#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

#include "IDevice.h"
#include "IManage.h"
#include "RS7Mapper.h"
#include "create_device.h"
#include "setsound.h"


// interface for managing the plumber.

#define WRITE_BLOCK_SIZE (128)

void RS7Manage::Monitor( int left_bus_id, int right_bus_id )
{
	if(( left_bus_id < 0 ) || (left_bus_id > NUM_BUSSES ) )
	{
		// XXX add warning here
		return;
	}

	if(( right_bus_id < 0 ) || (right_bus_id > NUM_BUSSES) )
	{
		// XXX add warning here
		return;
	}
	mapper->left_monitor_bus = left_bus_id;	
	mapper->right_monitor_bus = right_bus_id;
}

bool RS7Manage::SetDeviceOutput( int device_id, const char *output_name, int bus )
{
	assert( output_name );
	assert( bus >= 0 );

	if( bus >= NUM_BUSSES)
	{
		// XXX put a warning here
		return false;
	}

	if( mapper->devices[device_id].device_interface == NULL)
	{
		printf("SetDeviceOutput:: device #%d unknown\n", device_id );
		return false;
	}

	Device *device = &mapper->devices[device_id];

	return device->device_interface->SetOutput( output_name, 
		&mapper->busses[bus].current_value );
}

bool RS7Manage::SetDeviceMidiInput( int device_id, const char *input_name, int channel )
{
	assert( input_name );
	assert( channel >= 0 );

	if( mapper->devices[device_id].device_interface == NULL)
	{
		printf("SetDeviceMidiInput:: device #%d unknown\n", device_id );
		return false;
	}

	Device *device = &mapper->devices[device_id];

	return device->device_interface->SetMidiInput( input_name, channel );
}
bool RS7Manage::SetDeviceInput( int device_id, const char *input_name, int bus )
{
	assert( input_name );
	assert( bus >= 0 );

	if( bus >= NUM_BUSSES)
	{
		// XXX put a warning here
		return false;
	}

	if( mapper->devices[device_id].device_interface == NULL)
	{
		printf("SetDeviceInput:: device #%d unknown\n", device_id );
		return false;
	}

	Device *device = &mapper->devices[device_id];

	return device->device_interface->SetInput( input_name, 
		&mapper->busses[bus].current_value );
}

bool RS7Manage::DeleteDevice( int device_id )
{
	// note, doesnt delete the event receiver

	assert( device_id >= 0 );
	assert( device_id < NUM_DEVICES );

	if( mapper->devices[device_id].device_interface )
	{
		delete mapper->devices[device_id].device_interface;
		mapper->devices[device_id].Clear();
	}
	else
	{
		// no device to delete
		return false;
	}
	return true;
}

int RS7Manage::FindDevice( const char *device_type, const char *name )
{
	assert( device_type );
	assert( name );

	int i;
	
	for( i = 0 ; i < NUM_DEVICES ; i++)
	{
		if( strcmp( name, mapper->devices[i].name ) == 0 )
		{
			return i;
		}
	}
	
	return -1;
}

RS7Manage::RS7Manage( RS7Mapper *a_mapper )
{
	assert( a_mapper );
	mapper = a_mapper ;
}

int RS7Manage::CountBusses()
{
	return( NUM_BUSSES );
}

int RS7Manage::CreateDevice(	const char *device_type,
				const char *name,
				const char *params )
{
	assert( device_type );
	assert( name );

	printf("CreateDevice: device_type=%s, name=%s, params=%s\n",
		device_type, name, params );

	char blank_string [1] = "";

	if( params == NULL)
	{
		params = &blank_string[0];
	}

	// find a slot for the new_device
	int device_id;
	bool found = false;
	for(device_id = 0 ; device_id < NUM_DEVICES ; device_id ++)
	{
		if( mapper->devices[device_id].device_interface == NULL )
		{
			found = true;
			break;	
		}
	}	

	if( !found )
	{
		printf("RS7Mapper: CreateDevice: No device slots available\n");
		return -1;
	}

	IDevice *device_interface = create_device(device_type );

	if( device_interface == NULL)
	{
		printf("RS7Mapper:: CreateDevice: Unknown device \"%s\"\n",
			device_type );
		return -1;
	}

	mapper->devices[device_id].Set( device_interface, device_type, name );

	printf("GDR: mapper->samplerate=#%d\n", mapper->samplerate );
	// init the device
	device_interface->Init( mapper->devices[device_id].event_receiver,
		name, mapper->samplerate, params );	
	printf("GDR: Called Init\n");
	return device_id;
}
		


Bus::Bus()
{
	reference_count = 0;
	current_value = 0;
}

Device::Device()
{
	Clear();
}

void Device::Set( IDevice *a_device_interface, 
	const char *a_device_type, 
	const char *a_name )
{
	assert( a_device_interface);
	assert( a_device_type );
	assert( a_name );
	
	device_interface = a_device_interface;
	strcpy( device_type, a_device_type );
	strcpy( name, a_name );
}

void Device::Clear()
{
	device_interface = NULL;
	device_type[0] = 0;
	name[0] = 0;

	// leave event_receiver
}

IManage *RS7Mapper::GetControl()
{
	return manage_interface;
}

RS7Mapper::RS7Mapper( int a_samplerate, const char *audio_device )
{
	samplerate = a_samplerate;

	audio_output = fopen( audio_device, "wb");

	assert( audio_output );
	left_monitor_bus = -1;
	right_monitor_bus = -1;

	manage_interface = new RS7Manage( this );

	int device = 0;
	for( device = 0 ; device < NUM_DEVICES ; device ++)
	{
		// create an event receiver for this device
		DeviceEventReceiver *er = new DeviceEventReceiver();

		// create an event receiver and point it back to its device
		devices[device].event_receiver = er;
		er->device = &devices[device];
		er->mapper = this;
	}

	// XXX set samplerate
	set_card( fileno (audio_output), samplerate, 16, 2 );

	// XXX set the fragment size
	set_frag( fileno( audio_output) );

	// XXX start play thread
	pthread_create( &run_thread, NULL, run_thread_start, this );
		
}

void *RS7Mapper::run_thread_start( void * arg )
{
	RS7Mapper *me = (RS7Mapper *)arg;
	me->RunThread();
	return NULL;
}

void RS7Mapper::DoFrame( signed short * data_left, signed short *data_right )
{
	int i;
	for( i = 0 ; i < NUM_DEVICES ; i ++ )
	{
		if( devices[i].device_interface )
		{
			devices[i].device_interface->Clock();
		}
	}

	if (left_monitor_bus >= 0 )
	{
		int val = busses[left_monitor_bus].current_value;

#ifdef EIGHT_BIT	
		// convert to unsigned char
		val /= 0xffff;
		val /= 0xff;
		*data = (unsigned char) val;
		*data += 128;
#endif
		// convert to signed short
		val /= 0xffff;
		*data_left = (signed short) val;

		//printf("%02x ", *data);
	}

	if (right_monitor_bus >= 0 )
	{
		int val = busses[right_monitor_bus].current_value;
		// convert to signed short
		val /= 0xffff;
		*data_right = (signed short) val;
	}
}

void RS7Mapper::RunThread()
{
	signed short outbuffer[WRITE_BLOCK_SIZE* 2];

	printf("GDR: Started run thread.\n");
	for (;;)
	{
		int i;
		for( i = 0; i< WRITE_BLOCK_SIZE ; i++)
		{
			DoFrame( &outbuffer[i*2] , &outbuffer[(i*2)+1]);
		}
		//printf("GDR: WriteBlock\n");
		fwrite( &outbuffer[0], 2, WRITE_BLOCK_SIZE * 2, audio_output );
	}
}
	
void RS7Mapper::BroadcastMidiNoteOn( int channel, int note, int volume )
{
	int i;
	for( i = 0 ; i < NUM_DEVICES ; i ++ )
	{
		if( devices[i].device_interface )
		{
			devices[i].device_interface->MidiNoteOn( 
				channel, note, volume );
		}
	}
}

void RS7Mapper::BroadcastMidiNoteOff( int channel, int note )
{
	int i;
	for( i = 0 ; i < NUM_DEVICES ; i ++ )
	{
		if( devices[i].device_interface )
		{
			devices[i].device_interface->MidiNoteOff( 
				channel, note );
		}
	}
}

void DeviceEventReceiver::MidiNoteOn( int channel, int note, int volume )
{
	printf("MidiNoteOn( channel=#%d, note=#%d, volume=#%d\n",
		channel, note, volume );
	assert( mapper );
	mapper->BroadcastMidiNoteOn( channel, note, volume );
}

void DeviceEventReceiver::MidiNoteOff( int channel, int note )
{
	printf("MidiNoteOff( channel=#%d, note=#%d)\n",
		channel, note );
	assert( mapper );
	mapper->BroadcastMidiNoteOff( channel, note );
}


void RS7Manage::TransportPlay()
{
	int i;
	for( i = 0 ; i < NUM_DEVICES ; i ++ )
	{
		if(mapper->devices[i].device_interface )
		{
			mapper->devices[i].device_interface->TransportPlay( );
		}
	}
}

void RS7Manage::TransportStop()
{
	int i;
	for( i = 0 ; i < NUM_DEVICES ; i ++ )
	{
		if(mapper->devices[i].device_interface )
		{
			mapper->devices[i].device_interface->TransportStop( );
		}
	}
}

void RS7Manage::TransportReset()
{
	int i;
	for( i = 0 ; i < NUM_DEVICES ; i ++ )
	{
		if(mapper->devices[i].device_interface )
		{
			mapper->devices[i].device_interface->TransportReset( );
		}
	}
}
	
			
