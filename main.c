#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "udp.h"


int main (int argc, char** argv)
{

  printf (" UDP Emulator v1.0 Started \n");

  char* Message_Rec; // The UDP  Received message 
  Message_Rec = (char*) malloc(100);
  strcpy (Message_Rec , argv[1]);

  if (argc == 2)
  {
    if (debug) 
      printf("[DEBUG_MESSAGE] The message received : %s\n",Message_Rec);
  }
  else if (argc > 2)
  {
    if (debug) 
      printf("[DEBUG_MESSAGE] The message format is incorrect ! \n"
             " there's %d additional argument(s)\n", argc-2);
    return -1;
  }
  else
  {
    if (debug)
      printf("[DEBUG_MESSAGE] The message format is incorrect !! \n"
             "[DEBUG_MESSAGE] Pls put an argument with the right \n");
    return -1;
  }
  
  int SeqNr_Byte = 0; //
  /// Parsing the Received Message byte per byte 
  if (Parsing_CodeByte (Message_Rec) == RELAY_SIGNAL) // Relay Signal Received;
  {
    Parsing_SeqNrByte (SeqNr_Byte , Message_Rec);
    //Parsing Relay Bytes

    //Sending ACK
  }
  else if (Parsing_CodeByte (Message_Rec) == GET_STATE_SIGNAL ) // Get State Signal Received
  {
    Parsing_SeqNrByte (SeqNr_Byte , Message_Rec);
    //Sending ACK

    //Sending Relay_State
  }
  else if (Parsing_CodeByte (Message_Rec) == QUIT_SIGNAL ) // Quit Signal Received 
  {
    Parsing_SeqNrByte (SeqNr_Byte , Message_Rec);
    //Sending ACK
  }
  else 
  {
    if (debug) 
      printf("[DEBUG_MESSAGE] Bad Signal Code \n");
    return 0;
  }

  free (Message_Rec);
  return 0;
}
