/********************************************************************************
*
*  FILE NAME    : DNS_dbinout_main.c
*
*  DESCRIPTION: It contains function descrptions of module dbinout
*                
*
*    DATE    NAME    REFERENCE          REASON
*
*  3-OCT-10  mayank                Initial creation
*
*  
*  Copyright 2010, Aricent Technologies (Holdings) Ltd
*
******************************************************************************/

/***************************************************************************
*				HEADER FILES
****************************************************************************/

#include "DNS_server_header.h"
#include "DNS_server_database.h"

/**************************************************************************
*	FUNCTION : d_dbinout
*
*	DESCRIPTION: calls the appropriate database function
*
*	RETURNS : SUCCESS/FAILURE
*
*************************************************************************/
S32_INT dbinout(
			S32_INT choice,  /*choice of the user: DNS or R-DNS Lookup*/
			SCHAR *input)	/*Domain name or IP address*/
/*************************************************************************/
{
	S32_INT func_ret = SUCCESS;
	
	/*call for domain name to ip resolution*/	
	if(1 == choice)
	{
		func_ret = d_dntoip(input);
		if(FAILURE == func_ret)
		{
			ERROR(ERROR_MAJOR, ERROR_D_DNTOIP);
			return(FAILURE);
		}
	}

	/*call for ip to domain name resolution*/
	else if(2 == choice)
	{
		func_ret = d_iptodn(input);
		if(FAILURE == func_ret)
		{
			ERROR(ERROR_MAJOR, ERROR_D_DNTOIP);
			return(FAILURE);
		}
	}

	return SUCCESS;
}


/**************************************************************************
*	FUNCTION : d_dntoip
*
*	DESCRIPTION: gives the ip address for a dns address
*
*	RETURNS : SUCCESS/FAILURE
*
*************************************************************************/
S32_INT d_dntoip(
			SCHAR *input)	/*Domain name*/  
/***********************************************************************/
{
   MYSQL *conn;				/*variable for seting-up SQL connection*/
   MYSQL_RES *res;			/*variable to store query result*/
   MYSQL_ROW row;			/*variable to store an SQL row*/

   SCHAR *server = "localhost";		/*setting the servername*/
   SCHAR *user = "nalanda";		/*setting the username*/
   SCHAR *password = "abc123";		/*setting the password*/
   SCHAR *database = "test";		/*setting the database name*/

   conn = mysql_init(NULL);		/*initialising data*/	

   /*query to select from table ankur for a domain name */
   SCHAR query[50] = "select * from ankur where dns = ";	
   strcat(query,"'");
   strcat(query,input);
   strcat(query,"'");
   
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
   
   /*If found domain name, then copy IP address*/	
   while ((row = mysql_fetch_row(res)) != NULL)
   {
      	printf("%s \n", row[1]);
	strcpy(input,row[1]);
  }

   /* Free cache & close connection */
   mysql_free_result(res);
   mysql_close(conn);

   return SUCCESS;
}


/**************************************************************************
*	FUNCTION : d_iptodn
*
*	DESCRIPTION: gives the ip address for a dns address
*
*	RETURNS : SUCCESS/FAILURE
*
*************************************************************************/
S32_INT d_iptodn(
			SCHAR *input)	/*IP address*/
/**************************************************************************/
{
   MYSQL *conn;				/*variable for seting-up SQL connection*/
   MYSQL_RES *res;			/*variable to store query result*/
   MYSQL_ROW row;			/*variable to store an SQL row*/

   SCHAR *server = "localhost";		/*setting the servername*/
   SCHAR *user = "nalanda";		/*setting the username*/
   SCHAR *password = "abc123";		/*setting the password*/
   SCHAR *database = "test";		/*setting the database name*/

   conn = mysql_init(NULL);		/*initialising data*/

   /* Connect to database */
   SCHAR query[50] = "select * from ankur where ip = ";	
   strcat(query,"'");
   strcat(query,input);
   strcat(query,"'");
   
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
   
   /*If found IP address, then copy domain name*/	
   while ((row = mysql_fetch_row(res)) != NULL)
   {
     	printf("%s \n", row[0]);
	strcpy(input,row[0]);
   }

   /* Free cache & close connection */
   mysql_free_result(res);
   mysql_close(conn);

   return SUCCESS;
}

/**************************************************************************
*	FUNCTION : d_insert
*
*	DESCRIPTION: updates the database cache
*
*	RETURNS : SUCCESS/FAILURE
*
*************************************************************************/
S32_INT d_insert(
			SCHAR *dns, 		/*Domain name*/
			SCHAR *ip)		/*IP address*/
/************************************************************************/
{
   MYSQL *conn;				/*variable for seting-up SQL connection*/
   MYSQL_RES *res;			/*variable to store query result*/
   
   SCHAR *server = "localhost";		/*setting the servername*/
   SCHAR *user = "nalanda";		/*setting the username*/
   SCHAR *password = "abc123";		/*setting the password*/
   SCHAR *database = "test";		/*setting the database name*/

   conn = mysql_init(NULL);		/*initialising data*/

   /*query to insert new domain name-IP address pair in database*/
   SCHAR query[50] = "insert into ankur(dns,ip) values(";	
   strcat(query,"'");
   strcat(query,dns);
   strcat(query,"'");
   strcat(query,",'");
   strcat(query,ip);
   strcat(query,"')");

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
    	
   /* Free cache & close connection */
   mysql_free_result(res);
   mysql_close(conn);

   return SUCCESS;
}



