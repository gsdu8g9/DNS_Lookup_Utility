/********************************************************************************
*
*  FILE NAME    : DNS_resolver_main.c
*
*  DESCRIPTION: It contains the fuctions corresponding to resolver module  
*                
*
*    DATE    NAME    REFERENCE          REASON
*
*  3-OCT-10  mayank                Initial creation
*
*  
*  Copyright 2010, Aricent Technologies (Holdings) Ltd
*
*******************************************************************************/

/****************************************************************************
*			HEADER FILES	
*****************************************************************************/

#include "DNS_server_header.h"

/**************************************************************************
*	FUNCTION : resolver
*
*	DESCRIPTION: calls the appropriate function for domain name/IP                       resolution
*
*	RETURNS : SUCCESS/FAILURE
*
*************************************************************************/
S32_INT resolver(
			S32_INT choice,         /*choice of client: DNS or R-DNS Lookup*/
			SCHAR *input)		/*Domain nameor IP address*/
/************************************************************************/
{
	S32_INT func_ret =  SUCCESS;	
	
	if(1 == choice)
	{
		func_ret = dntoip(input);
		if(FAILURE == func_ret)
		{
			ERROR(ERROR_MAJOR, ERROR_DNTOIP);
			return(FAILURE);
		}
	}
	else if(2 == choice)
	{
		func_ret = iptodn(input);
		if(FAILURE == func_ret)
		{
			ERROR(ERROR_MAJOR, ERROR_IPTODN);
			return(FAILURE);
		}
	}

	return SUCCESS;
}

/**************************************************************************
*	FUNCTION : dntoip
*
*	DESCRIPTION: gives the ip address for a dns address
*
*	RETURNS : SUCCESS/FAILURE
*
*************************************************************************/
S32_INT dntoip(
			SCHAR *input)	/*domain name */
/************************************************************************/
{
	struct addrinfo hints, *res, *p;
	SCHAR ipstr[INET6_ADDRSTRLEN];
	S32_INT status;
	char *inet_error;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo(input, NULL, &hints, &res)) != 0) 
	{
        	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        	printf("error in getaddrinfo\n");
    	}

	printf("IP addresses for %s:\n", input);

	for(p = res;p != NULL; p = p->ai_next)
	{
        	void *addr;	// get the pointer to the address itself,
        	SCHAR *ipver;	// different fields in IPv4 and IPv6:

        	if (p->ai_family == AF_INET)  //IpV4
	 	{ 
            		struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            		addr = &(ipv4->sin_addr);
            		ipver = "IPv4";
        	} 
		else //IpV6
		{ 
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
        	}

        	// convert the IP to a string and print it:
        	inet_error = inet_ntop(p->ai_family, addr, ipstr, (socklen_t)sizeof(ipstr));
        	printf("  %s: %s\n", ipver, ipstr);
    	}

	memset(input, (S32_INT)'\0', 50);	
	strcpy(input,ipstr);
	freeaddrinfo(res); 	// free the linked list

	return SUCCESS;
}


/**************************************************************************
*	FUNCTION : iptodn
*
*	DESCRIPTION: gives the donmain name for a ip address
*
*	RETURNS : SUCCESS/FAILURE
*
*************************************************************************/
S32_INT iptodn(
			SCHAR *input)	/*IP address */
/************************************************************************/
{
	//SCHAR szHostName[200];
	struct sockaddr_in addr;
	//struct in_addr ipv6;
	struct hostent *h;

	/*Fill in address structure*/
	addr.sin_family=AF_INET;
	addr.sin_port=htons(10000);
	addr.sin_addr.s_addr=inet_addr(input);

	h = gethostbyaddr((SCHAR*)&(addr.sin_addr),
	(socklen_t)sizeof(struct in_addr),
	AF_INET );

	if(NULL == h)
	{
		printf("Could not locate %s\n",input);
		return SUCCESS;
	}

	memset(input, (S32_INT)'\0', 50);	
	strcpy(input,h->h_name);
	return SUCCESS;
}
