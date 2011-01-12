#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "UDPSocket.h"

UDPSocket::UDPSocket()
{
	my_socket = socket( PF_INET, SOCK_DGRAM, 0);
	memset( &target_sa, 0, sizeof( target_sa ));
	memset( &local_sa, 0, sizeof( local_sa ));
}

UDPSocket::~UDPSocket()
{
	close( my_socket );
}

void UDPSocket::SetTarget( unsigned int addr, unsigned short port)
{
	struct sockaddr_in *sin;
	sin = (struct sockaddr_in *)&target_sa;

	sin->sin_family = AF_INET;
	sin->sin_port = htons( port );
	sin->sin_addr.s_addr = htonl( addr );	

	printf("sin_port=&%x", sin->sin_port );
}

int UDPSocket::Bind( unsigned short port, unsigned int addr )
{
	int res;

	struct sockaddr_in *sin;
	sin = (struct sockaddr_in *)&local_sa;

	sin->sin_family = AF_INET;
	sin->sin_port = htons( port );
	sin->sin_addr.s_addr = htonl( addr );	

	res = bind(my_socket, &local_sa, sizeof(local_sa) );

	return res;
}
		
int UDPSocket::ReceiveFrom( char *buffer, int length, unsigned int *addr, unsigned short *port )
{
	struct sockaddr temp_addr;
	memset( &temp_addr, 0, sizeof( temp_addr ));
	
	struct sockaddr_in *sin;
	sin = (struct sockaddr_in *)&temp_addr;

	int res;

	socklen_t addr_len = sizeof( temp_addr );

	//printf("GDR: recvfrom\n");
	res = recvfrom( my_socket, buffer, length, 0, &temp_addr,  &addr_len );
	//printf("GDR: recvfrom returned\n");

	if( addr != NULL)
	{	
		*addr = ntohl(sin->sin_addr.s_addr);
	}
	
	if( port != NULL )
	{
		*port = ntohs( sin->sin_port );
	}

	return res;
}

int UDPSocket::Send( char *buffer, int length)
{
	printf("GDR: Send: len=%d\n", length );
	int res;	
	//perror("Before UDPSocket");
	res = sendto( my_socket, buffer, length, 0, &target_sa, 
		sizeof(target_sa));

	perror("UDPSocket::Send()");
	//printf("GDR: sendto returned #%d\n", res );
	return res;
}
