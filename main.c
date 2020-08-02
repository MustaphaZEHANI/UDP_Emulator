#include "udp.h"


int main (int argc, char** argv)
{
  printf (" UDP Emulator v1.0 Started \n");

  char* Message_Rec; // The Received message 
  Message_Rec = (char*) malloc ( 100 * sizeof(char) );
  strcpy (Message_Rec , argv[1]);

  char* Message_Parsed; // The Message Parsed
  Message_Parsed = (char*) malloc ( 100 * sizeof(char) );

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
    return ERROR;
  }
  else
  {
    if (debug)
      printf ("[DEBUG_MESSAGE] The message format is incorrect ! \n");
    return ERROR;
  }

  int SeqNr_Byte ; // 

  /* 
    Parse the Received Message byte per byte and
    send the relative message for each signal
  */
  if (Parsing_CodeByte (Message_Rec) == RELAY_SIGNAL) // Relay Signal Received
  {
    if(debug)
      printf("[DEBUG_MESSAGE] RELAY_SIGNAL_CODE detected \n");

    // Check the message Length
    if ( (Check_Message_Length (RELAY_SIGNAL_LENGTH_1, Message_Rec) == ERROR) &&
         (Check_Message_Length (RELAY_SIGNAL_LENGTH_2, Message_Rec) == ERROR) )
    {
      printf(" Bad Message\n");
      return ERROR;
    }
    
    if(Parsing_SeqNrByte (&SeqNr_Byte , Message_Parsed , Message_Rec) == -1 )
      {
        if(debug)
          printf(" Bad message\n");
        return ERROR;
      }
    if (debug)
      printf ("[DEBUG_MESSAGE] After Parsing_SeqNrByte , SeqNr_Byte = %x , Message_Parsed =%s \n"
             , SeqNr_Byte, Message_Parsed, Message_Rec);

    //Parsing Relay Bytes   
    
    //Sending ACK
    Send_ACK(&SeqNr_Byte);
  }
  else if (Parsing_CodeByte (Message_Rec) == GET_STATE_SIGNAL ) // Get State Signal Received
  {
    if(debug)
      printf("[DEBUG_MESSAGE]  GET_SIGNAL_CODE detected \n");
    
    //Check the message Length
    if (Check_Message_Length (GET_STATE_SIGNAL_LENGTH, Message_Rec) == ERROR)
    {
      printf("Bad Message\n");
      return ERROR;
    }

    if (Parsing_SeqNrByte (&SeqNr_Byte , Message_Parsed , Message_Rec) == ERROR )
      return ERROR;
    //Sending ACK
    Send_ACK(&SeqNr_Byte);
    //Sending Relay_State
  }
  else if (Parsing_CodeByte (Message_Rec) == QUIT_SIGNAL ) // Quit Signal Received 
  {
    if(debug)
      printf("[DEBUG_MESSAGE]  QUIT_SIGNAL \n");
    
    //Check the message Length
    if (Check_Message_Length (QUIT_SIGNAL_LENGTH, Message_Rec) == ERROR)
    {
      printf("Bad Message\n");
      return ERROR;
    }

    if (Parsing_SeqNrByte (&SeqNr_Byte , Message_Parsed , Message_Rec) == ERROR )
    {
      printf(" Bad message\n");
      return ERROR;
    }

    //Sending ACK
    Send_ACK(&SeqNr_Byte);
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
