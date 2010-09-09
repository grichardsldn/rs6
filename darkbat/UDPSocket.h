#include <netinet/in.h>

class UDPSocket
{
	public: 
	UDPSocket();
	~UDPSocket();
	//int SendTo( char *buffer, int length, unsigned int target_addr, 
	//	unsigned int target_port );
	int Send( char *buffer, int length );
	void SetTarget( unsigned int addr, unsigned short port);
	int Bind( unsigned short port = 0, unsigned int local_addr = 0);
	int ReceiveFrom( char *buffer, int len, unsigned int *addr = NULL, 
   		unsigned short *port = NULL );
	private:
	int my_socket;
	struct sockaddr target_sa;
	struct sockaddr local_sa;

};

