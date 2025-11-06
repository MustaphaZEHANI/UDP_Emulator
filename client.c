// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define debug 	    1

#define SUCCESS	    0
#define ERROR      -1


#define PORT	 			 2020 
#define MSG_MAX_SIZE   		  512


// Driver code 
int main (int argc, char** argv)
{ 
	int SockFD; 
	char buffer[MSG_MAX_SIZE]; 
	struct sockaddr_in	 Svr_Addr;
	char* Message_ToSend; // The message to send 
  	Message_ToSend = (char*) malloc ( 100 * sizeof(char) ); 
	
	if (argc == 2)
	{
		strcpy (Message_ToSend , argv[1]);
		if (debug) 
			printf ("[DEBUG_CLIENT] Message sent = '%s'\n",Message_ToSend);
	}
	else if (argc > 2)
	{
		if (debug)
			printf ("[DEBUG_CLIENT][ERROR] The message format is incorrect ! \n"
			" there's %d additional argument(s)\n", argc-2);
		return ERROR;
	}
	else
	{
		if (debug)
			printf ("[DEBUG_CLIENT][ERROR] The message format is incorrect ! \n");
		return ERROR;
	}

	// AF_UNIX/AF_LOCAL=localhost, AF_INET=IPV4, AF_INET6=IPV6
	// AF="@ family"
	// SOCK_STREAM=TCP, SOCK_DGRAM=UDP ,SOCK_RAW=lower-level Communication 
	// Creating socket file descriptor 
	if ( (SockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
	{ 
		printf("[DEBUG_CLIENT] Cannot create socket \n"); 
		return ERROR; 
	} 

	memset(&Svr_Addr, 0, sizeof(Svr_Addr)); 
	
	// Server Configuration   
	Svr_Addr.sin_family = AF_INET; 
	Svr_Addr.sin_port = htons(PORT); 
	Svr_Addr.sin_addr.s_addr = INADDR_ANY; 
	
	u_int16_t buffer_size;
	void* len = NULL;
	
	int ret = sendto(SockFD, (const char *)Message_ToSend, strlen(Message_ToSend), 
		MSG_CONFIRM, (const struct sockaddr *) &Svr_Addr, sizeof(Svr_Addr)); 
	if (ret == ERROR)
		return ERROR; 

	if (debug)
		printf("[DEBUG_CLIENT] Waiting to receive message ...\n"); 
		
	buffer_size = recvfrom(SockFD, (char *)buffer, MSG_MAX_SIZE, 
					MSG_WAITALL, (struct sockaddr *) &Svr_Addr, len); 
	
	buffer[buffer_size] = '\0'; 
	printf("[DEBUG_CLIENT] Received Signal = %s\n", buffer); 

	close(SockFD); 
	return SUCCESS; 
} 
