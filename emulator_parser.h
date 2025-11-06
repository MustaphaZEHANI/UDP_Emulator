#ifndef UDP_H_
#define UDP_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define debug   1 //Set 1 to activate Debug Message 


/*-----------------*/
/*    Variables    */
/*-----------------*/

#define SUCCESS                  0
#define ERROR                   -1


#define PORT                  2020 
#define MSG_MAX_SIZE           512 

// Signal Code (1st Byte)
#define ACK_CODE                 11  // 0x11 
#define RELAY_STATE_SIGNAL_CODE  41  // 0x41
#define RELAY_SIGNAL_CODE        21  // 0x21
#define GET_STATE_SIGNAL_CODE    31  // 0x31
#define QUIT_SIGNAL_CODE         51  // 0x51

// Signal Length (Received Signal)
#define RELAY_SIGNAL_LENGTH_1    15  
#define RELAY_SIGNAL_LENGTH_2    21 
#define GET_STATE_SIGNAL_LENGTH   6
#define QUIT_SIGNAL_LENGTH        6

// Relay State Signal : sequence number
#define RELAY_STATE_SEQ_NUMB      1 //0x01

// Received Singal
enum ReceivedSignal
{
  RELAY_SIGNAL , GET_STATE_SIGNAL , QUIT_SIGNAL
};

/* Relay Signal */
struct Relay
{
  int Value;      //Open or Close
} Relay_0,Relay_1;



//Relay Count
enum RelayCount
{
  Relay_Count_1 = 1 , Relay_Count_2
};
// Relay Number
enum RelayNbr
{
  Relay_Nbr_0 , Relay_Nbr_1
};
// Relay State
enum RelayValue
{
  Relay_Open , Relay_Close
};


/*-----------------*/
/*    Functions    */
/*-----------------*/

int Check_Message_Length (int msg_Len , char* msg);

int Parsing_CodeByte (char* msg);

int Byte_Parser (int* Byte_parsed , char* msg_parsed , char* msg );

int Parsing_SeqNrByte (int* SeqNr , char* msg_parsed , char* msg);

int Parsing_RelayCountByte (int* RelayCount_parsed , char* msg_parsed , char* msg);

int Parsing_RelayNbrByte (int* RelayNbr_parsed , char* msg_parsed , char* msg) ;

int Parsing_RelayValueByte (int* RelayValue_parsed , char* msg_parsed , char* msg);

int Set_Relay_Value (int* RelayValue_parsed , int* RelayNbr_parsed);

int Parsing_RelaySignalBytes (char* msg_parsed , char* msg);

void ACK_ToSend (char* msg , int* SeqNr );

void RelayState_ToSend (char* msg);

#endif