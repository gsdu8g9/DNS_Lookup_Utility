/********************************************************************************
*
*  FILE NAME    : DNS_client_handler.c
*
*  DESCRIPTION: It contains the handler fucntions'  descriptions for  client
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

#include "DNS_client_header.h"


/******************************************************************
*		FUNCTION: my_sigalarm
*		
*		DESCRIPTION: It handles the condition of server being   *                            too busy
*
*		RETURNS: void
*******************************************************************/
void my_sigalarm( /*@unused@*/ S32_INT signo)
{
	/*If the client doesnot get response within 5 seconds*/
	printf("SERVER TOO BUSY, TRY SOME OTHER TIME \n");
    	exit(EXIT_SUCCESS);
}
