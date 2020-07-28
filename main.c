#include "udp.h"


int main (int argc, char** argv)
{

  printf (" UDP Emulator v1.0 Started \n");

  char* Message_Rec; // The Received message 
  Message_Rec = (char*) malloc ( 100 * sizeof(char) );
  strcpy (Message_Rec , argv[1]);

  char* Message_Parsed; // The Message Parsed
  Message_Parsed = (char*) malloc ( 100 * sizeof(char) );

  int SeqNr_Byte = 0; // 

  if (argc == 2)
  {
    if (debug) 
      printf ("[DEBUG_MESSAGE] The message received : %s\n",Message_Rec);
  }
  else if (argc > 2)
  {
    if (debug) 
      printf ("[DEBUG_MESSAGE] The message format is incorrect ! \n"
             " there's %d additional argument(s)\n", argc-2);
    return -1;
  }
  else
  {
    if (debug)
      printf ("[DEBUG_MESSAGE] The message format is incorrect ! \n");
    return -1;
  }

  /// Parsing the Received Message byte per byte 
  if (Parsing_CodeByte (Message_Rec) == RELAY_SIGNAL) // Relay Signal Received;
  {
    if(!Parsing_SeqNrByte (SeqNr_Byte , Message_Parsed , Message_Rec))
      return -1;
    //Parsing Relay Bytes
    //Sending ACK
  }
  else if (Parsing_CodeByte (Message_Rec) == GET_STATE_SIGNAL ) // Get State Signal Received
  {
    if (!Parsing_SeqNrByte (SeqNr_Byte , Message_Parsed , Message_Rec))
        return -1;
    //Sending ACK
    //Sending Relay_State
  }
  else if (Parsing_CodeByte (Message_Rec) == QUIT_SIGNAL ) // Quit Signal Received 
  {
    if (!Parsing_SeqNrByte (SeqNr_Byte , Message_Parsed , Message_Rec))
      return -1;
    //Sending ACK
  }
  else 
  {
    if (debug) 
      printf("[DEBUG_MESSAGE] Bad Signal Code \n");
    return 0;
  }

  free (Message_Parsed);
  free (Message_Rec);
  return 0;
}
