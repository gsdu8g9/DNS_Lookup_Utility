/********************************************************************************
*
*  FILE NAME    : DNS_client_main.c 
*
*  DESCRIPTION: Client that Sends a request to UDP server regarding domain or ip
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

/*******************************************************************
*			HEADER FILES
********************************************************************/

#include "DNS_client_header.h"

/********************************************************************
*			MACROS
*********************************************************************/

#define port 13000

S32_INT g_trace_level = DETAILED_TRACE ;
S32_INT g_error_level = ERROR_CRITICAL ;

/********************************************************************
*		FUNCTION: main
*
*		DESCIPTION: It is the main function of the UDP client
*
*		RETURNS: SUCCESS/FAILURE
********************************************************************/

S32_INT main(
		S32_INT argc,	/*number of command line arguments*/ 
		SCHAR *argv[])	/*the command line arguments*/

/****************************************************************/
{	TRACE(DETAILED_TRACE,"Entering main\n");

	S32_INT 	sd 	=  0 ,	/*client socket*/
			retbind =  0 ,	/*bind's return value*/
			retsend =  0 , 	/*send's return value*/
			retrecv =  0 ,	/*recv's return value*/
			close_ret = 0;  /*close's return value*/	
							
	struct sockaddr_in 	client_addr;	/*client's address structure*/
						
	struct sockaddr_in	server_addr;	/*server's address structure*/	
	
	SCHAR 		buff[50] ;		/*stores the DN/IP*/
	socklen_t 	len_server = 0 ;	/*server's socket length*/
	S32_INT		new_port = 0,		/*The new port given to client*/
			choice   = 0;		/*The choice of client*/


	//Signal handler for alarm
	signal(SIGALRM,my_sigalarm);

	/*If arguments too few*/
	if(argc < 3)
	{
		printf("Arguments entered are lesser than expected,hence exiting\n");		
		exit(FAILURE);
	}

	TRACE(DETAILED_TRACE,"Creating Socket\n");

	/*creating the socket*/
	if ((sd = DNS_Socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{
		ERROR(ERROR_CRITICAL,ERROR_SOCKET);
		exit(FAILURE);
	}  
	TRACE(DETAILED_TRACE,"Socket created\n");
		

	memset(&client_addr, 0, sizeof(client_addr));

	TRACE(BRIEF_TRACE,"Populating Structure for client\n");
	
	/*setting client attributes*/
	
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	client_addr.sin_port = htons(port);
	TRACE(BRIEF_TRACE,"Populated Structure for client\n");
	
	TRACE(DETAILED_TRACE,"Binding Socket \n");
	
	/*binding the client's socket*/
	
	retbind = DNS_Bind(sd,(struct sockaddr *)&client_addr,sizeof(client_addr));
	if(retbind < 0)	
	{
		ERROR(ERROR_CRITICAL,ERROR_BIND);
		exit(FAILURE);
	}
	TRACE(DETAILED_TRACE,"Bind Complete\n");
	

	memset(&server_addr, 0, sizeof(server_addr));

	TRACE(BRIEF_TRACE,"Populating Server Structure \n");
	
	/*setting server attributes*/
	
	server_addr.sin_family = AF_INET                   ;
	server_addr.sin_addr.s_addr = inet_addr(argv[1])   ;
	server_addr.sin_port = htons(atoi(argv[2]))        ;
	TRACE(BRIEF_TRACE,"Server Structure Populated\n")  ;
	
	/* GET USER'S CHOICE*/

	memset(buff,'\0',sizeof(buff));
	printf("Enter your choice 1. DNS lookup 2. R-DNS lookup\n");
	get_int(&choice);

	if(1 != choice && 2!=choice)
	{
		printf("YOU ENTERED WRONG INPUTS\n");
		return FAILURE;
	}
	
	/*GET USER'S QUERY*/

	printf("ENTER THE QUERY\n");
	scanf("%s",buff);

	/*SEND THE FIRST UDP PACKET TO SERVER*/
	
	retsend = DNS_Sendto(sd,buff, sizeof(buff), 0, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if(retsend < 0)	
	{
		ERROR(ERROR_CRITICAL,ERROR_SEND);
		exit(FAILURE);
	}

	len_server = sizeof(server_addr);

	alarm(5);		/*FOR SERVER TIMEOUT*/

	/*RECEIVE THE NEW PORT FROM THE SERVER*/
	
	retrecv = DNS_Recvfrom(sd,&(new_port),sizeof(new_port),0,(struct sockaddr*)&server_addr,&len_server);
	if(retrecv < 0)	
	{
		ERROR(ERROR_CRITICAL,ERROR_RECV);
		exit(FAILURE);
	}

	alarm(0);

	/*SETTING THE NEW PORT*/

	server_addr.sin_port = htons(new_port);
	printf("new port is %d\n",new_port);

	/*SEND THE DOMAIN NAME/IP ADDRESS TO NEW SERVER*/
	
	retsend = DNS_Sendto(sd,buff, sizeof(buff), 0, (struct sockaddr *) &server_addr, sizeof(server_addr));	
	if(retsend < 0)	
	{
		ERROR(ERROR_CRITICAL,ERROR_SEND);
		exit(FAILURE);
	}

	/*SEND THE CHOICE: DNS/R-DNS LOOKUP*/
	
	retsend = DNS_Sendto(sd,&choice, sizeof(choice), 0, (struct sockaddr *) &server_addr, sizeof(server_addr));
	
	if(retsend < 0)	
	{
		ERROR(ERROR_CRITICAL,ERROR_SEND);
		exit(FAILURE);
	}
	memset(buff,'\0',sizeof(buff));

	/*RECEIVE THE RESOLVED QUERY FROM SERVER*/

	retrecv = DNS_Recvfrom(sd,&(buff),sizeof(buff),0,(struct sockaddr*)&server_addr,&len_server);

	if(retrecv < 0)	
	{
		ERROR(ERROR_CRITICAL,ERROR_RECV);
		exit(FAILURE);
	}
	
	/*PRINT THE RESOLVED QUERY*/
	
	printf("THE HOST/IP ADDRESS IS %s \n", buff);

	/*CLOSING THE CLIENT SOCKET*/

	close_ret = DNS_Close(sd);

	if(-1 == close_ret)
	{
		ERROR(ERROR_CRITICAL,ERROR_CLOSE);
		exit(FAILURE);
	}
	
	/*CLIENT EXECUTED SUCCESSFULLY*/	

	return SUCCESS ;
}

