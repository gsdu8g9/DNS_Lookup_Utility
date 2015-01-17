/********************************************************************************
*
*  FILE NAME    : DNS_server_main.c
*
*  DESCRIPTION: It contains the fuctional description of SERVER module
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

/**************************************************************************
*			HEADER FILES
****************************************************************************/

#include "DNS_server_header.h"

/***************************************************************************
*			MACROS
****************************************************************************/

#define PORT 14000
#define MAX 10
#define SIZE 50



S32_INT g_trace_level = DETAILED_TRACE ;
S32_INT g_error_level = ERROR_CRITICAL ;


/**************************************************************************
*		FUNCTION: main
*
*		DESCRIPTION: It does the basic operation for the SERVER module
*
*		RETURNS: SUCCESS/FAILURE
***************************************************************************/

S32_INT main()

/****************************************************************/
{	
	S32_INT 		sd = 0;
	SCHAR 			buff[SIZE];
	SCHAR 			input[SIZE];
	socklen_t 		t;
	struct sockaddr_in 	seraddr,
				cliaddr;		
	S32_INT 		pid = 0,
	    			counter  = 0,
	    			new_port = 0,
				retbind  = 0,									           choice   = 0,
				close_ret = 0 ;

	S32_INT			func_ret = SUCCESS;
	ssize_t retrecv = 0;
	ssize_t retsend = 0;

	//Sig handler to catch the exit of child
	signal(SIGCHLD,my_sigchld);
	
	//Sig handler to catch Ctrl+C
	signal(SIGINT,my_sigint);

	TRACE(DETAILED_TRACE,"Entering Main\n");


	TRACE(BRIEF_TRACE,"Creating Socket\n");
	sd = DNS_Socket(AF_INET, SOCK_DGRAM, 0);
	if(sd < 0)
	{
		ERROR(ERROR_CRITICAL,ERROR_SOCKET);
		return(FAILURE);
	}
	TRACE(BRIEF_TRACE,"Socket Created\n");

	memset(&seraddr,0,sizeof(seraddr));
	
	TRACE(BRIEF_TRACE,"Populating Structure\n");
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(PORT);
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);	
	TRACE(BRIEF_TRACE,"Structure Populated\n");


	TRACE(BRIEF_TRACE,"Binding to server\n");
	retbind = DNS_Bind(sd,(struct sockaddr*)&seraddr, (socklen_t)sizeof(seraddr));	
	if(retbind < 0)
	{
		ERROR(ERROR_CRITICAL,ERROR_BIND);
		return(FAILURE);
	}
	TRACE(BRIEF_TRACE,"Binded to server\n");

	func_ret = update(); /* UPDATING SERVER CACHE*/
	if(FAILURE == func_ret)
	{
		ERROR(ERROR_MAJOR, ERROR_UPDATE);
		return(FAILURE);
	}

	while(1 == 1)
	{
		printf("SERVER RUNNING\n");

		memset(buff, (S32_INT)'\0',sizeof(buff));
		t = (socklen_t)sizeof(cliaddr);
		retrecv = DNS_Recvfrom(sd,buff,sizeof(buff),0,(struct sockaddr*)&cliaddr, &t);
		if(retrecv < 0)
		{
			ERROR(ERROR_CRITICAL,ERROR_RECV);
			return(FAILURE);
		}

		counter++;

		pid = fork();

		if(0 == pid)
		{	
			TRACE(BRIEF_TRACE,"Entering into forked server\n");
			new_port = PORT + counter;
			sd = socket(AF_INET, SOCK_DGRAM, 0);
			memset(&seraddr,0,sizeof(seraddr));
			
			seraddr.sin_family = AF_INET;	//populating server wid new port and address
			seraddr.sin_port = htons((uint16_t)new_port);
			seraddr.sin_addr.s_addr = htonl(INADDR_ANY);	
			
			retbind = DNS_Bind(sd,(struct sockaddr*)&seraddr, (socklen_t)sizeof(seraddr));	
			if(retbind < 0)
			{
		   		ERROR(ERROR_CRITICAL,ERROR_BIND);
				return(FAILURE) ;
			}

			retsend = DNS_Sendto(sd,&(new_port),sizeof(new_port),0,(struct sockaddr*)&cliaddr, (socklen_t)sizeof(cliaddr));						
			if(retsend < 0)
			{
				ERROR(ERROR_CRITICAL,ERROR_SEND);
				return(FAILURE);
			}
			memset(buff, (S32_INT)'\0',sizeof(buff));
			t = (socklen_t)sizeof(cliaddr);

			retrecv = DNS_Recvfrom(sd,buff,sizeof(buff),0,(struct sockaddr*)&cliaddr, &t);	        
			if(retrecv < 0)
			{
				ERROR(ERROR_CRITICAL,ERROR_RECV);
				return(FAILURE);
			}

			retrecv = DNS_Recvfrom(sd,&choice,sizeof(choice),0,(struct sockaddr*)&cliaddr, &t);
			if(retrecv < 0)
			{
				ERROR(ERROR_CRITICAL,ERROR_RECV);
				return(FAILURE);
			}

			memset(input, (S32_INT)'\0',sizeof(input));
			strcpy(input, buff);

			func_ret = dbinout(choice,buff);
			if(FAILURE == func_ret)
			{
				ERROR(ERROR_MAJOR, ERROR_DBINOUT);
				return(FAILURE);
			}

			if(0 != strcmp(input,buff))
			{
				retsend = DNS_Sendto(sd,buff,sizeof(buff),0,(struct sockaddr*)&cliaddr, (socklen_t)sizeof(cliaddr));			
				if(retsend < 0)
				{
					ERROR(ERROR_CRITICAL,ERROR_SEND);
					return(FAILURE);
				}
			}
			else
			{
				func_ret = resolver(choice,buff);	
				if(FAILURE == func_ret)
				{
					ERROR(ERROR_MAJOR, ERROR_RESOLVER);
					return(FAILURE);
				}

				if(0 != strcmp(input,buff))
				{
			       	retsend = DNS_Sendto(sd,buff,sizeof(buff),0,(struct sockaddr*)&cliaddr, (socklen_t)sizeof(cliaddr));			
					if(retsend < 0)
					{
						ERROR(ERROR_CRITICAL,ERROR_SEND);
						return(FAILURE);
					}
					if(1 == choice)
					{
						func_ret = d_insert(input,buff);
						if(FAILURE == func_ret)
						{
							ERROR(ERROR_MAJOR, ERROR_D_INSERT);
							return(FAILURE);
						}
					}
					else if(2 == choice)
					{
						func_ret = d_insert(buff,input);
						if(FAILURE == func_ret)
						{
							ERROR(ERROR_MAJOR, ERROR_D_INSERT);
							return(FAILURE);
						}
					}
				}

				else
				{
					memset(buff , (S32_INT)'\0', sizeof(buff));
					strcpy(buff, "NO SUCH IP/HOST");
		      			retsend = DNS_Sendto(sd,buff,sizeof(buff),0,(struct sockaddr*)&cliaddr, (socklen_t)sizeof(cliaddr));			
					if(retsend < 0)
					{
						ERROR(ERROR_CRITICAL,ERROR_SEND);
						exit(FAILURE);
					}
				}				
			}

			close_ret = DNS_Close(sd);
			if(-1 == close_ret)
			{
				ERROR(ERROR_MAJOR,ERROR_CLOSE);
				exit(FAILURE);

			}
			printf("Another client successfully served\n");
			TRACE(BRIEF_TRACE,"Exiting from forked server\n");
			
			exit(EXIT_SUCCESS);			
		}
	}

	TRACE(DETAILED_TRACE,"Exiting Main\n");
	return(SUCCESS);
}
