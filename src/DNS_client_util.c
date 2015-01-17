/********************************************************************************
*
*  FILE NAME    : DNS_client_util.c
*
*  DESCRIPTION: This file handles error-handling for client       
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

/***************************************************************
*		HEADER FILES
****************************************************************/
							
#include "DNS_client_header.h"

/***************************************************************
*	DATA VARIABLE : error_code
*
*	DESCRIPTION: Contains the appropriate error message
****************************************************************/

static SCHAR *error_code[]={
				"Error in creating socket",
				"Error in binding socket",							"Error in receiving udp datagram",
				"Error in sending udp datagram",
				"Error in closing socket"
		    	};


S32_INT g_error_level ;


/******************************************************************
*	FUNCTION: error_fun
*
*	DESCRIPTION: prints the appropriate error message
*
*	RETURNS: void
******************************************************************/
void error_fun(
				S32_INT level,    /* error level */
				S32_INT code)	  /* error code */	
/******************************************************************/
{
	/*checking the error level*/
	if(g_error_level >= level)
	{	
		printf("The error type is: %s\n",error_code[code]);
	}
}

