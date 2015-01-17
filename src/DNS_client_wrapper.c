/*********************************************************************
*
*  FILE NAME    : DNS_client_wrapper.c
*
*  DESCRIPTION: Contains wrapper functions for system calls of client
*                
*
*    DATE    NAME    REFERENCE          REASON
*
*  3-OCT-10  mayank                Initial creation
*
*  
*  Copyright 2010, Aricent Technologies (Holdings) Ltd
*
***********************************************************************/

/*****************************************************************
*		HEADER FILES
******************************************************************/

#include "DNS_client_header.h"

/******************************************************************
*	FUNCTION: DNS_Socket
*
*	DESCRIPTION: creates a socket
*
*	RETURNS: S32_INT
******************************************************************/
S32_INT DNS_Socket(
			S32_INT domain, 
			S32_INT type, 
			S32_INT protocol
		)
{
	S32_INT sock = 0;
	sock = socket(domain, type, protocol);
	return sock;
}

/******************************************************************
*	FUNCTION: DNS_Bind
*
*	DESCRIPTION: binds a socket to an IP-Port pair
*
*	RETURNS: S32_INT
******************************************************************/
S32_INT DNS_Bind( S32_INT sockfd,
		 struct sockaddr *serv_addr,
		 socklen_t addrlen
		)
{

	S32_INT bind_ret = 0;
	bind_ret = bind(sockfd,serv_addr,addrlen);
	return bind_ret;
}

/******************************************************************
*	FUNCTION: DNS_Recvfrom
*
*	DESCRIPTION: It Receives UDP packets.
*
*	RETURNS: ssize_t
******************************************************************/
ssize_t DNS_Recvfrom(
			S32_INT s,
			void *buf,
			size_t len,
			S32_INT flags,
			struct sockaddr *cli_addr,
		    	socklen_t *addrlen
		)
{
	ssize_t read_bytes = 0;
	read_bytes = recvfrom(s, buf, len, flags, cli_addr, addrlen);
	return read_bytes;
}

/******************************************************************
*	FUNCTION: DNS_Sendto
*
*	DESCRIPTION: It Sends UDP packets.
*
*	RETURNS: ssize_t
******************************************************************/
ssize_t DNS_Sendto(
			S32_INT s,
			const void *buf,
			size_t len,
			S32_INT flags,
			const struct sockaddr *serv_addr,
			socklen_t addrlen
		)
{
	ssize_t write_bytes = 0;
	write_bytes = sendto(s, buf, len, flags, serv_addr, addrlen);
	return write_bytes;
}

/******************************************************************
*	FUNCTION: DNS_Close
*
*	DESCRIPTION: It closes a particular descriptor.
*
*	RETURNS: S32_INT
******************************************************************/
S32_INT DNS_Close (S32_INT sockfd)

{
	S32_INT close_ret;
	close_ret = close(sockfd);
	return close_ret;
}

/******************************************************************
*	FUNCTION: get_int
*
*	DESCRIPTION: it is a robust scanf function for getting integers
*
*	RETURNS: void
******************************************************************/
void get_int(S32_INT *piInput)
{

	S32_INT iCheck = 0;

	S32_INT iochar = 0;

	do
	{       
		iCheck = scanf("%d", piInput);
		if (iCheck != 1)
		{
			printf("\nThat is not a valid integer statement\n");
			printf("Please try again: ");
			while ((iochar = getchar()) != (S32_INT)'\n');
		}
		else
		{
			iochar = getchar();
			if (!isspace(iochar))
			{
				printf("\nThat is not a valid integer statement\n");
				printf("Please try again: ");
				iCheck = 0;
				while ((iochar = getchar()) != (S32_INT)'\n');
			}

		}

	}while (iCheck < 1);
}


