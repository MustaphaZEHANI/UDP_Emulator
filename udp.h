#ifndef UDP_H_
#define UDP_H_

#define debug 1


enum
{
  RELAY_SIGNAL , GET_STATE_SIGNAL , QUIT_SIGNAL
};



int Parsing_CodeByte (char msg[]);

int Parsing_SeqNrByte (int SeqNr , char msg[]);

int Parsing_RelaySignalBytes (int SeqNr , char msg[]);

int Send_ACK (int SeqNr );

int Send_Relay_State(int SeqNr , char msg[]);


  /// #### SENT  MESSAGE  ####

  /**** ACK Signal ****/ 
  // Type : Send Signal 
  // 2 bytes: code (0x11) , sequence nr.
  // Role :

  /**** Relay State Signal ****/
  // Type : Send Signal 
  // 3 bytes : code (0x41) , sequence nr , relay state ( 0 or 1 ).

  /////////////////////
  /*   UDP Signals   */
  /////////////////////
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