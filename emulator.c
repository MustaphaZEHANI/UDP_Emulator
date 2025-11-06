#include "emulator_parser.h"


int main (int argc, char** argv)
{
  int ret = ERROR;

  int SockFD;  // File discriptor socket 
  struct sockaddr_in Svr_Addr, Cli_Addr; // Server and Client Adress

  char* Message_Rec; // The Received message 
  Message_Rec = (char*) malloc ( MSG_MAX_SIZE * sizeof(char) );

  char* Message_Parsed; // The Message Parsed
  Message_Parsed = (char*) malloc ( MSG_MAX_SIZE * sizeof(char) );

  // Sending ACK Signal
  char *Message_ToSend ; 
  Message_ToSend = (char*) malloc ( 100 * sizeof(char) );
  
  printf (" UDP Emulator v1.1 Started \n");
      
  // Creating socket file descriptor 
  if ( (SockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
  { 
    printf("[DEBUG_SERVER] Cannot create socket"); 
    return ERROR; 
  } 
      
  memset(&Svr_Addr, 0, sizeof(Svr_Addr)); 
  memset(&Cli_Addr, 0, sizeof(Cli_Addr)); 

  // Setting server information 
  Svr_Addr.sin_family    = AF_INET;      // IPv4 famuly
  Svr_Addr.sin_addr.s_addr = INADDR_ANY; // bind to all @
  Svr_Addr.sin_port = htons(PORT);       // Port @
  
  // Bind the socket with the server address 
  if ( bind(SockFD, (const struct sockaddr *)&Svr_Addr,  
            sizeof(Svr_Addr)) < 0 ) 
  { 
    printf("[DEBUG_SERVER] Bind failed"); 
    return ERROR;  
  } 
        
  int buffer_size ;
  unsigned int len;
  len = sizeof(Cli_Addr);  // size of Client Address

  while (1)
  {  
    // Read Received Message from the socket FD
    buffer_size = recvfrom(SockFD, (char *)Message_Rec, MSG_MAX_SIZE,  
                MSG_WAITALL, ( struct sockaddr *) &Cli_Addr, &len); 
    Message_Rec[buffer_size] = '\0'; 
    
    if (debug)
      printf("[DEBUG_SERVER] Signal Received: <%s>\n", Message_Rec); 

    /* 
      *******************************************
                    Parsing Process   
      *******************************************
      Parse the Received Message byte per byte and
      send the relative signal for each one
    */
    int SeqNr_Byte ;
    ret = Parsing_CodeByte (Message_Rec);
    if (ret  == ERROR)
    {
      if(debug)
        printf("[DEBUG_SERVER][ERROR] Parsing_CodeByte\n");
      continue;
    } // Relay Signal Received
    else if (ret == RELAY_SIGNAL) 
    {
      int ret;
      if(debug)
        printf("[DEBUG_SERVER] RELAY_SIGNAL_CODE detected \n");

      // Check the message Length : we have 2 possibility
      if ( (Check_Message_Length (RELAY_SIGNAL_LENGTH_1, Message_Rec) == ERROR) &&
          (Check_Message_Length (RELAY_SIGNAL_LENGTH_2, Message_Rec) == ERROR) )
      { 
        if(debug)
          printf("[DEBUG_SERVER] [ERROR] Check_Message_Length   \n");
        continue;
      }

      // Parsing the Sequence Number Byte
      ret = Parsing_SeqNrByte (&SeqNr_Byte , Message_Parsed , Message_Rec);
      if (ret == ERROR )
      { 
        if(debug)
          printf("[DEBUG_SERVER][ERROR] Parsing_SeqNrByte\n");
        continue;
      }

      if (debug)
        printf ("[DEBUG_SERVER] After Parsing_SeqNrByte= %x , SeqNr_Byte = %s , Message_Parsed =%s \n"
              , SeqNr_Byte, Message_Parsed, Message_Rec);

      // Parsing the 3 Relay Bytes  
      ret = Parsing_RelaySignalBytes (Message_Parsed, Message_Parsed);
      if (ret == ERROR)
        { 
          if(debug)
            printf("[DEBUG_SERVER][ERROR] Parsing_RelaySignalBytes\n");
          continue;
        }

      // Sending ACK Signal
      ACK_ToSend (Message_ToSend , &SeqNr_Byte);

      sendto (SockFD, (const char *)Message_ToSend, strlen(Message_ToSend),  
          MSG_CONFIRM, (const struct sockaddr *) &Cli_Addr, len); 

      
    }// Get State Signal Received
    else if (ret == GET_STATE_SIGNAL ) 
    {
      int ret = ERROR;
      if(debug)
        printf("[DEBUG_SERVER] GET_SIGNAL_CODE detected \n");
      
      //Check the message Length
      ret = Check_Message_Length (GET_STATE_SIGNAL_LENGTH, Message_Rec);
      if (ret == ERROR)
        { 
          if(debug)
            printf("[DEBUG_SERVER][ERROR] Check_Message_Length\n");
          continue;
        }
      
      // Parsing the Sequence Number Byte
      ret = Parsing_SeqNrByte (&SeqNr_Byte , Message_Parsed , Message_Rec);
      if (ret == ERROR)
        { 
          if(debug)
            printf("[DEBUG_SERVER][ERROR] Parsing_SeqNrByte\n");
          continue;
        }
        
      printf(" ...Received Get State\n");

      // Sending ACK
      ACK_ToSend (Message_ToSend , &SeqNr_Byte);

      sendto(SockFD, (const char *)Message_ToSend, strlen(Message_ToSend),  
                    MSG_CONFIRM, (const struct sockaddr *) &Cli_Addr, len); 

      // Sending Relay_State
      RelayState_ToSend (Message_ToSend);
      
      sendto(SockFD, (const char *)Message_ToSend, strlen(Message_ToSend),  
                    MSG_CONFIRM, (const struct sockaddr *) &Cli_Addr, len); 

    }
    else if (ret == QUIT_SIGNAL ) // Quit Signal Received 
    {
      if(debug)
        printf("[DEBUG_SERVER] QUIT_SIGNAL_CODE detected \n");
      
      //Check the message Length
      if (Check_Message_Length (QUIT_SIGNAL_LENGTH, Message_Rec) == ERROR)
      {
        if(debug)
          printf("[DEBUG_SERVER][ERROR] Check_Message_Length\n");
        continue;
      }

      // Parsing the Sequence Number Byte
      ret = Parsing_SeqNrByte (&SeqNr_Byte , Message_Parsed , Message_Rec);
      if (ret == ERROR)
        { 
          if(debug)
            printf("[DEBUG_SERVER][ERROR] Parsing_SeqNrByte\n");
          continue;
        }

      // Sending ACK Signal
      ACK_ToSend (Message_ToSend , &SeqNr_Byte);

      sendto (SockFD, (const char *)Message_ToSend, strlen(Message_ToSend),  
          MSG_CONFIRM, (const struct sockaddr *) &Cli_Addr, len); 
          
      printf(" ...QUIT\n");

      }
      else 
      {
        if (debug) 
          printf("[DEBUG_SERVER] Bad Signal Code \n");
        continue;
      }
    }
  free (Message_ToSend);
  free (Message_Parsed);
  free (Message_Rec);
}
