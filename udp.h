#ifndef UDP_H_
#define UDP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define debug 1 //Set it to 1 to activate Debug Message


// Variables 

enum
{
  RELAY_SIGNAL , GET_STATE_SIGNAL , QUIT_SIGNAL
};


// Functions

int Byte_Parser (char* msg_parsed , char* msg );

int Parsing_CodeByte (char* msg);

int Parsing_SeqNrByte (int SeqN , char* msg_parsed , char* msg);

int Parsing_RelaySignalBytes (char* msg_parsed , char* msg);
/*
void Send_ACK (int SeqNr );

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