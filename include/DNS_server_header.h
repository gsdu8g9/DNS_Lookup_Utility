/********************************************************************************
*
*  FILE NAME    : DNS_server_header.h
*
*  DESCRIPTION: The header file required by all the source files of server
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

#ifndef _HEADER
#define _HEADER

/*******************************HEADER FILES********************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <ctype.h>
#include "portable.h"
#include <errno.h>

/*******************************MACROS********************************/

#define FAILURE 0
#define SUCCESS 1


/**********************************************************************
				TRACING
***********************************************************************/

/*****************************TRACE LEVELS ***********************/

#define DETAILED_TRACE 3
#define BRIEF_TRACE    2
#define NO_TRACE       1

#ifdef PTRACE
	#define TRACE(level,x) if(g_trace_level >= level) printf(x)
#else
	#define TRACE(level,x) /*  Nothing */
#endif


/**********************************************************************
			ERROR HANDLING 
***********************************************************************/

/********************************ERROR LEVELS ********************/

#define ERROR_CRITICAL 3
#define ERROR_MAJOR    2
#define ERROR_MINOR    1
#define ERROR_NO       0

/********************************ERROR CODES *********************/

#define ERROR_SOCKET   0
#define ERROR_BIND     1
#define ERROR_RECV     2
#define ERROR_SEND     3
#define ERROR_UPDATE   4
#define ERROR_DNTOIP   4
#define ERROR_D_DNTOIP 5
#define ERROR_IPTODN   6
#define ERROR_D_IPTODN 7
#define ERROR_D_INSERT 8
#define ERROR_DBINOUT  9
#define ERROR_RESOLVER 10
#define ERROR_SQL_CONNECT 11
#define ERROR_SQL_QUERY 13
#define ERROR_WAIT 14
#define ERROR_CLOSE 15

#define ERROR(level,code) error_fun(level,code)


/**********************************************************************
			FUNCTIONS
**********************************************************************/
/*function for error handling*/
extern void error_fun(S32_INT,S32_INT)		 ;
/*function for updating the database*/
extern S32_INT update(void)			 ;
/*function for domain name to IP address resolution*/
extern S32_INT dntoip(SCHAR *)   		 ;
/*function for database search for a domain name*/
extern S32_INT d_dntoip(SCHAR *) 		 ;
/*function for Ip address to domain name resolution*/
extern S32_INT iptodn(SCHAR *)   		 ;
/*function for database search for an IP address*/
extern S32_INT d_iptodn(SCHAR *)   		 ;
/*function for inserting a new entry in the database*/
extern S32_INT d_insert(SCHAR *, SCHAR *)	 ;
/*function for database handling*/
extern S32_INT dbinout(S32_INT, SCHAR*) 	 ;
/*function for name server handling*/
extern S32_INT resolver(S32_INT, SCHAR*)	 ;
/*function for handling SIGCHLD signal*/
extern void my_sigchld(S32_INT)			 ;
/*function for handling SIGINT signal*/
extern void my_sigint(S32_INT)			 ;	
/*function for handling SIGALRM signal*/
extern void my_sigalarm(S32_INT)		 ;	
/*wrapper function for creating socket*/
extern S32_INT DNS_Socket(S32_INT domain, S32_INT type, S32_INT protocol);
/*wrapper function for binding socket*/
extern S32_INT DNS_Bind( S32_INT sockfd, struct sockaddr *serv_addr, socklen_t addrlen);
/*wrapper function for receiving data*/
extern ssize_t DNS_Recvfrom(S32_INT s, void *buf, size_t len, S32_INT flags, struct sockaddr *cli_addr, socklen_t *addrlen);
/*wrapper function for sending data*/
extern ssize_t DNS_Sendto(S32_INT s, const void *buf, size_t len, S32_INT flags, const struct sockaddr *serv_addr, socklen_t addrlen);
/*wrapper function for closing socket*/
extern S32_INT DNS_Close (S32_INT sockfd);

/*********************************************************************
			GLOBAL VARIABLES
************************************************************************/

extern S32_INT g_error_level ;	/*variable for global error level*/
extern S32_INT g_trace_level ;	/*variable for global trace level*/


#endif
