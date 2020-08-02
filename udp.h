#ifndef UDP_H_
#define UDP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define debug 1 //Set it to 1 to activate Debug Message


//* Variables *//

#define SUCCESS 0
#define ERROR  -1

// Signal Code
#define ACK_CODE                "11" // 0x11 
#define RELAY_STATE_SIGNAL_CODE "41" // 0x41

#define RELAY_SIGNAL_CODE      21  // 0x21
#define GET_STATE_SIGNAL_CODE  31  // 0x31
#define QUIT_SIGNAL_CODE       51  // 0x51

// Signal Length (Received Signal)
#define RELAY_SIGNAL_LENGTH_1   14  
#define RELAY_SIGNAL_LENGTH_2   23 
#define GET_STATE_SIGNAL_LENGTH 5
#define QUIT_SIGNAL_LENGTH      5


enum 
{
  RELAY_SIGNAL , GET_STATE_SIGNAL , QUIT_SIGNAL
};


//* Functions *//

int Check_Message_Length (int msg_Len , char* msg);

int Parsing_CodeByte (char* msg);

int Byte_Parser (int* Byte_parsed , char* msg_parsed , char* msg );

int Parsing_SeqNrByte (int* SeqNr , char* msg_parsed , char* msg);

int Parsing_RelaySignalBytes (char* msg_parsed , char* msg);

void Send_ACK (int* SeqNr );

/*
int Send_Relay_State(int SeqNr , char* msg); */


  /// #### SENT  MESSAGE  ####

  /**** ACK Signal ****/ 
  // Type : Send Signal 
  // 2 bytes: code (0x11) , sequence nr.

  /**** Relay State Signal ****/
  // Type : Send Signal 
  // 3 bytes : code (0x41) , sequence nr , relay state ( 0 or 1 ).


  /// #### RECEIVED MESSAGE  ####
  /**** Relay Signal ****/
  // Type : Receive Signal
  // 4 bytes : code (0x21) , sequence nr , count , relay number ( 0 or 1 ) , relay value.

   /**** Get State Signal ****/
  // Type : Receive Signal
  // 2 bytes : code (0x31) , sequence nr.

  /**** Quit Signal ****/
  // Type : Receive Signal
  // 2 bytes : code (0x51) , sequence nr.

#endif