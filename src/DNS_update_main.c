/********************************************************************************
*
*  FILE NAME    : DNS_update_main.c 
*
*  DESCRIPTION: It contains the fuctions corresponding to update module
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

/****************************************************************
*		HEADER FILES
***************************************************************/

#include "DNS_server_header.h"
#include "DNS_server_database.h"

/*****************************************************************
*	FUNCTION : update
*
*	DESCRIPTION: updates the cache
*
*	RETURNS : SUCCESS/FAILURE
*
******************************************************************/
S32_INT update(
			void)	/*It takes no argument*/
/*****************************************************************/
{
   MYSQL *conn;				/*variable for seting-up SQL connection*/
   MYSQL_RES *res;			/*variable to store query result*/
   MYSQL_ROW row;			/*variable to store an SQL row*/

   SCHAR *server = "localhost";		/*setting the servername*/
   SCHAR *user = "nalanda";		/*setting the username*/
   SCHAR *password = "abc123";		/*setting the password*/
   SCHAR *database = "test";		/*setting the database name*/

   conn = mysql_init(NULL);		/*initialising data*/	

   /*query to select all from ankur */
   SCHAR query[50] = "select * from ankur ";
   SCHAR query1[50];			/*query to update values in databse*/	
   SCHAR input[50];			/*stores the domain name to be checkd*/		
   SCHAR insert[50][50];		/*stores the new DN-IP pair*/
   S32_INT counter = 0;			/*counts the number of new DN/IP*/
   S32_INT i_loop = 0;			/*loop variable*/  
 
   /* Connect to database */
   if (!mysql_real_connect(conn, server,
   user, password, database, 0, NULL, 0)) 
   {
      	ERROR(ERROR_CRITICAL,ERROR_SQL_CONNECT);
	return(FAILURE);
   }

   /* send SQL query */
   if (mysql_query(conn, query)) 
   {
	ERROR(ERROR_CRITICAL,ERROR_SQL_QUERY);
	return(FAILURE);
   }

   res = mysql_use_result(conn);

   while ((row = mysql_fetch_row(res)) != NULL)
   {
	/*get the current Domain Name*/
      	printf("%s \n", row[0]);
	strcpy(input, row[0]);
	
	dntoip(input);		/*get the new IP address*/

	if(0 != strcmp(row[1],input))	/*If doesn't match*/
	{
		/*Save the Domain name-IP pair*/

		memset(insert[counter], '\0', sizeof(insert[counter]));
		strcpy(insert[counter], input);
		counter++;
		memset(insert[counter], '\0', sizeof(insert[counter]));
		strcpy(insert[counter], row[0]);
		counter++;
	}

	else
	{
		/* DO NOTHING */
	}
   }

   /* close connection */
   mysql_free_result(res);

   /*Update the new IP addresses*/
   while( i_loop < counter)
   {
	memset(query1, '\0', sizeof(query1));
	strcpy(query1,"update ankur set ip=");	
	strcat(query1,"'");
	strcat(query1,insert[i_loop]);
	i_loop++;
	strcat(query1,"' where dns='");
	strcat(query1,insert[i_loop]);
	i_loop++;
	strcat(query1,"'");

	/* send SQL query */
	if (mysql_query(conn, query1)) 
	{
		ERROR(ERROR_CRITICAL,ERROR_SQL_QUERY);
		return(FAILURE);
	}
   }

   /*close the mysql connection*/
   mysql_close(conn);

   /*Update function completed successfully*/
   return SUCCESS;
}
