/********************************************************************************
*
*  FILE NAME    : DNS_server_util.c
*
*  DESCRIPTION: This file handles error-handling for server       
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
							
#include "DNS_server_header.h"

/***************************************************************
*	DATA VARIABLE : error_code
*
*	DESCRIPTION: Contains the appropriate error message
****************************************************************/

static SCHAR *error_code[]={
				"Error in creating socket",
				"Error in binding socket",							"Error in receiving udp datagram",
				"Error in sending udp datagram"
				"Error in update() function",
				"Error in dntoip() function",
				"Error in d_dntoip() function",
				"Error in iptodn() function",
				"Error in d_iptodn() function",
				"Error in insert() function",
				"Error in dbinout() function",
				"Error in resolver() function",
				"Error in connecting to SQL database",
				"Error in executing SQL query",
				"Error in wait function",
				"Error in close function"
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
				S32_INT level,        /* error level */
				S32_INT code)	  /* error code */	
/******************************************************************/
{
	/*checking the error level*/
	if(g_error_level >= level)
	{	
		printf("The error type is: %s\n",error_code[code]);
	}
}

