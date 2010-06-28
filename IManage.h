
// interface for managing the plumber.

class IManage
{
	public:

	virtual int CountBusses( ) = 0;
	virtual int CreateDevice(	const char *device_type, 
				const char *instance_name, 
				const char *params ) = 0;
	
	virtual int FindDevice( const char *device_type, const char *instance_name ) = 0;
	virtual bool DeleteDevice( int device_id ) = 0;

	virtual bool SetDeviceInput( int device_id, const char *input_name, int bus ) = 0;
	virtual bool SetDeviceMidiInput( int device_id, const char *input_name, int channel ) = 0;
	virtual bool SetDeviceOutput( int device_id, const char *output_name, int bus ) =0;

	virtual void Monitor( int left_bus_id, int right_bus_id ) = 0;

	virtual void TransportPlay() = 0;
	virtual void TransportStop() = 0;
	virtual void TransportReset() = 0;	
};
					
