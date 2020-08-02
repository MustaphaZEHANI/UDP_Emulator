#include "udp.h"


int main (int argc, char** argv)
{
  int ret = ERROR;
  printf (" UDP Emulator v1.0 Started \n");

  char* Message_Rec; // The Received message 
  Message_Rec = (char*) malloc ( 100 * sizeof(char) );

  char* Message_Parsed; // The Message Parsed
  Message_Parsed = (char*) malloc ( 100 * sizeof(char) );

  if (argc == 2)
  {
    if (debug) 
      printf ("[DEBUG_MESSAGE] The message received : %s\n",Message_Rec);
    strcpy (Message_Rec , argv[1]);
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

  /* 
    Parse the Received Message byte per byte and
    send the relative signal for each one
  */
  int SeqNr_Byte ;
  ret = Parsing_CodeByte (Message_Rec);
  if (ret  == ERROR)
  {
    printf(" Wrong message received\n");
    return ERROR;
  }
  else if (ret == RELAY_SIGNAL) // Relay Signal Received
  {
    int ret;
    if(debug)
      printf("[DEBUG_MESSAGE] RELAY_SIGNAL_CODE detected \n");

    // Check the message Length
    if ( (Check_Message_Length (RELAY_SIGNAL_LENGTH_1, Message_Rec) == ERROR) &&
         (Check_Message_Length (RELAY_SIGNAL_LENGTH_2, Message_Rec) == ERROR) )
    {
      printf(" Wrong message received\n");
      return ERROR;
    }
    if(Parsing_SeqNrByte (&SeqNr_Byte , Message_Parsed , Message_Rec) == ERROR )
    {
      printf(" Wrong message received\n");
      return ERROR;
    }
    if (debug)
      printf ("[DEBUG_MESSAGE] After Parsing_SeqNrByte , SeqNr_Byte = %x , Message_Parsed =%s \n"
             , SeqNr_Byte, Message_Parsed, Message_Rec);

    // Parsing Relay Bytes   
    ret = Parsing_RelaySignalBytes (Message_Parsed, Message_Parsed);
    if (ret == ERROR)
    {
      printf(" Wrong message received\n");
      return ERROR;
    }

    // Sending ACK
    Send_ACK(&SeqNr_Byte);

    return SUCCESS;
  }
  else if (ret == GET_STATE_SIGNAL ) // Get State Signal Received
  {
    if(debug)
      printf("[DEBUG_MESSAGE]  GET_SIGNAL_CODE detected \n");
    
    //Check the message Length
    if (Check_Message_Length (GET_STATE_SIGNAL_LENGTH, Message_Rec) == ERROR)
    {
      printf("Wrong message received\n");
      return ERROR;
    }

    if (Parsing_SeqNrByte (&SeqNr_Byte , Message_Parsed , Message_Rec) == ERROR )
    {
      printf("Wrong message received\n");
      return ERROR;
    }
    //Sending ACK
    Send_ACK(&SeqNr_Byte);

    //Sending Relay_State
    Send_Relay_State(char* msg_parsed , char* msg);

    return SUCCESS;
  }
  else if (ret == QUIT_SIGNAL ) // Quit Signal Received 
  {
    if(debug)
      printf("[DEBUG_MESSAGE]  QUIT_SIGNAL_CODE detected \n");
    
    //Check the message Length
    if (Check_Message_Length (QUIT_SIGNAL_LENGTH, Message_Rec) == ERROR)
    {
      printf("Wrong message received\n");
      return ERROR;
    }

    ret = Parsing_SeqNrByte (&SeqNr_Byte , Message_Parsed , Message_Rec);
    if (ret == ERROR)
    {
      printf(" Wrong message received\n");
      return ERROR;
    }

    //Sending ACK
    Send_ACK(&SeqNr_Byte);
  }
  else 
  {
    if (debug) 
      printf("[DEBUG_MESSAGE] Bad Signal Code \n");
    return ERROR;
  }

  free (Message_Parsed);
  free (Message_Rec);
  return 0;
}
