#ifndef UDP_H_
#define UDP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define debug 1 //Set it to 1 to activate Debug Message


/*-----------------*/
/*    Variables    */
/*-----------------*/

#define SUCCESS                  0
#define ERROR                   -1

// Signal Code (1st Byte)
#define ACK_CODE                "11" // 0x11 
#define RELAY_STATE_SIGNAL_CODE "41" // 0x41
#define RELAY_SIGNAL_CODE        21  // 0x21
#define GET_STATE_SIGNAL_CODE    31  // 0x31
#define QUIT_SIGNAL_CODE         51  // 0x51

// Signal Length (Received Signal)
#define RELAY_SIGNAL_LENGTH_1    14  
#define RELAY_SIGNAL_LENGTH_2    20 
#define GET_STATE_SIGNAL_LENGTH   5
#define QUIT_SIGNAL_LENGTH        5

// Relay State Signal (send) : sequence number
#define RELAY_STATE_SEQNUMB_SEND  1 //0x01

// Received Singal
enum ReceivedSignal
{
  RELAY_SIGNAL , GET_STATE_SIGNAL , QUIT_SIGNAL
};

/* Relay Signal */
struct Relay
{
  int Value; //Default Value Close
} Relay_0, Relay_1 ;


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

int Parsing_RelaySignalBytes (char* msg_parsed , char* msg);

void Send_ACK (int* SeqNr );

void Send_Relay_State(char* msg_parsed , char* msg);


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