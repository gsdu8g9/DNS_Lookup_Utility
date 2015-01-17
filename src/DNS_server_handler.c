/********************************************************************************
*
*  FILE NAME    : DNS_server_handler.c
*
*  DESCRIPTION: It contains the handler fucntions'  descriptions for server 
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

/********************************************************************
*			HEADER FILES
********************************************************************/

#include "DNS_server_header.h"

/*******************************************************************
*		FUNCTION: my_sigchld
*		
*		DESCRIPTION: It handles the exit of a child             *                             (forked server) in server.c
*
*		RETURNS: void
********************************************************************/

void my_sigchld(/*@unused@*/ S32_INT signo)
{
	/*variable to store the exit status of child*/

    S32_INT status = 0 ;
    pid_t wait_error = 0; 	/*return value of wait*/
	
    printf("The server has exited now, after serving the purpose\n");

    wait_error = wait(&status);		/*store the exit status of the child*/

    if(-1 == wait_error)
    {
	ERROR(ERROR_CRITICAL, ERROR_WAIT);
    }			
}


/**************************************************************
*		FUNCTION: my_sigint
*		
*		DESCRIPTION: It handles the pressing of CTRL+C  key in server.c
*
*		RETURNS: void
*****************************************************************/
void my_sigint(/*@unused@*/ S32_INT signo)
{
	/* When the server is exited*/
	printf("YOU PRESSED CTRL+C \n");
    	exit(EXIT_SUCCESS);
}

