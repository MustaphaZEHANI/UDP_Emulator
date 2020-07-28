#include "udp.h"


/* 
  -----------------------------------------------------------------------------
    Name          | int  int Parsing_CodeByte (char* msg)
  -----------------------------------------------------------------------------
    Description   | Parsing the 1st byte to identify the received message 
  -----------------------------------------------------------------------------
    Inputs        | char* msg []
  -----------------------------------------------------------------------------
    Outputs       | return 
                  |   0 : Relay Signal 
                  |   1 : Get State Signal
                  |   2 : Quit Signal
                  |  -1 : Bad Signal
 ------------------------------------------------------------------------------
*/
int Parsing_CodeByte (char* msg)
{
  switch(atoi(msg))
  {
    case 21:
      if (debug) 
        printf ("[DEBUG_MESSAGE] Relay Signal Code Detected \n");
      return 0;

    case 31:
      if (debug) 
        printf ("[DEBUG_MESSAGE] Get State Signal Code Detected\n");
      return 1;

    case 51:
      if (debug) 
        printf ("[DEBUG_MESSAGE] Quit Signal Code Detected\n");
      return 2;
      
    default:
      return -1;
  }
}


/* 
  -----------------------------------------------------------------------------
    Name          | int Byte_Parser (char* msg_parsed, char* msg )
  -----------------------------------------------------------------------------
    Description   | Parsing the byte after the ':'  and return it
                  | e.g. msg = 11:2a:22 =>>  msg_parsed = "2a:22" ; return "2a" 
  -----------------------------------------------------------------------------
    Inputs        | char* msg_parsed, char* msg 
  -----------------------------------------------------------------------------
    Outputs       | return the Byte to parse in hexa format
 ------------------------------------------------------------------------------
*/
int Byte_Parser (char* msg_parsed , char* msg )
{
  int ret = 0 ;

  // Check if the message format is OK
  if (! strchr(msg , ':'))
  {
    if(debug)
      printf ("[DEBUG_MESSAGE] There is no ':' in this message\n");
    return -1;
  } 

  // Find the first Byte after ':'
  strcpy (msg_parsed ,  strchr(msg , ':') + 1 );

  // Convert the byte from string to hexadycimal (base 16)
  ret = strtol (msg_parsed , NULL , 16);
  if (debug)
    printf ("[DEBUG_MESSAGE] ret = %x\n", ret);

  return ret;
}


/* 
  -----------------------------------------------------------------------------
    Name          | int  Parsing_SeqNrByte  (char* msg_parsed , char msg[])
  -----------------------------------------------------------------------------
    Description   | Parsing the 2nd byte to identify the message sequence number 
  -----------------------------------------------------------------------------
    Inputs        | char* msg_parsed  ; char* msg []
  -----------------------------------------------------------------------------
    Outputs       | return the sequence number (int)
 ------------------------------------------------------------------------------
 */
Parsing_SeqNrByte (int SeqN , char* msg_parsed , char* msg)
{
  int ret = 0 ;

  SeqN = Byte_Parser (msg_parsed , msg);
  /*if (SeqN)
  {
    printf("The message format is incorrect !\n");
    return -1;
  }*/

  return ret;
}


/* 
  -----------------------------------------------------------------------------
    Name          | int  Parsing_SeqNrByte  (char* msg_parsed , char msg[])
  -----------------------------------------------------------------------------
    Description   | Parsing the 2nd byte to identify the message sequence number 
  -----------------------------------------------------------------------------
    Inputs        | char* msg_parsed  ; char* msg []
  -----------------------------------------------------------------------------
    Outputs       | return the sequence number (int)
 ------------------------------------------------------------------------------
 */
int Parsing_RelaySignalBytes (char* msg_parsed , char* msg)
{
  
  return 0;
}

/*int Send_Relay_State(char* msg_parsed , char* msg)
{
  return 0;
}*/
    


