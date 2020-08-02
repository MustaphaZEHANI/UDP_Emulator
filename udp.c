#include "udp.h"

/* 
  -------------------------------------------------------------------------------------
    Name          | int Check_Message_Length (int msg_Len , char* msg)
  -------------------------------------------------------------------------------------
    Description   | Check if the signal length is correct 
  -------------------------------------------------------------------------------------
    Inputs        | int msg_Len , char* msg
  -------------------------------------------------------------------------------------
    Outputs       | return 0 SUCCESS
                  |       -1 ERROR
 --------------------------------------------------------------------------------------
*/
int Check_Message_Length (int msg_Len , char* msg)
{
  //Check the message Length
  if (strlen(msg) != msg_Len)
  {
    if(debug)
      printf ("[DEBUG_MESSAGE] %s fail !, strlen(msg) = %d\n", __func__, strlen(msg) );
    return ERROR;
  }
  if(debug)
    printf ("[DEBUG_MESSAGE] %s correct \n", __func__ );
  return SUCCESS;
}

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
                  |  -1 : ERROR (Bad Signal)
 ------------------------------------------------------------------------------
*/
int Parsing_CodeByte (char* msg)
{
  switch(atoi(msg))
  {
    case RELAY_SIGNAL_CODE:     // 0x21
      return RELAY_SIGNAL;

    case GET_STATE_SIGNAL_CODE: // 0x31
      return GET_STATE_SIGNAL;

    case QUIT_SIGNAL_CODE:      // 0x51
      return QUIT_SIGNAL;
      
    default:
      return ERROR;
  }
}


/* 
  -------------------------------------------------------------------------------------
    Name          | int Byte_Parser (int* Byte_parsed , char* msg_parsed, char* msg )
  -------------------------------------------------------------------------------------
    Description   | Parsing the byte after the first ':'  and return it with Hex format
                  | e.g. msg: 21:a4:44:33:01 => Byte_parsed: a4 | msg_parsed: a4:44:33:01
  -------------------------------------------------------------------------------------
    Inputs        | char* msg_parsed, char* msg 
  -------------------------------------------------------------------------------------
    Outputs       | return 0 : SUCCESS
                  |       -1 : ERROR
 --------------------------------------------------------------------------------------
*/
int Byte_Parser (int* Byte_parsed , char* msg_parsed , char* msg )
{
  char *endpt;
  // Check if the message format is OK
  if( (! strchr(msg , ':')) ||  (msg[3] == '\0') )
  {
    if(debug)
      printf ("[DEBUG_MESSAGE] %s , Message format incorrect !\n", __func__ );
    return -1;
  } 

  // Find the first Byte after the ':'
  strcpy (msg_parsed ,  strchr(msg , ':') + 1 );

  // Convert the byte from string to hexadecimal (base 16)
  *Byte_parsed =  strtol (msg_parsed , &endpt , 16);
  if (debug)
    printf ("[DEBUG_MESSAGE] %s , msg_parsed = %s , endpt = %c \n", __func__, msg_parsed , *endpt );
     
  if ( (*Byte_parsed < 0) || (*Byte_parsed > 255) || (*endpt != ':') )
  {
    if (debug)
    printf("[DEBUG_MESSAGE] %s , This Byte is incorrect! , endpt = %c \n", __func__, *endpt);
    return ERROR;
  }

  if (debug)
    printf ("[DEBUG_MESSAGE] %s , *Byte_parsed = %x\n", __func__, *Byte_parsed);

  return SUCCESS;
}


/* 
  -----------------------------------------------------------------------------
    Name          | int  Parsing_SeqNrByte  (char* msg_parsed , char msg[])
  -----------------------------------------------------------------------------
    Description   | Parsing the 2nd byte to identify the message sequence number 
  -----------------------------------------------------------------------------
    Inputs        | char* msg_parsed  ; char* msg []
  -----------------------------------------------------------------------------
    Outputs       | return 0 SUCCESS
                  |       -1 ERROR
 ------------------------------------------------------------------------------
 */
int Parsing_SeqNrByte (int* SeqNr , char* msg_parsed , char* msg)
{
  int ret = Byte_Parser (SeqNr , msg_parsed , msg);
  if (ret == -1 )
  {
    if(debug) 
      printf("[DEBUG_MESSAGE] %s , The message format is incorrect !\n", __func__);
    return -1;
  }

  if(debug) 
    printf("[DEBUG_MESSAGE] %s , SeqNr (hex) = %x \n", __func__, *SeqNr);
  return SUCCESS;
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

/* 
  --------------------------------------------------------------------------------
    Name          | void Send_ACK (int* SeqNr )
  --------------------------------------------------------------------------------
    Description   | Send the Acknowledge signal dependant on the Sequence number
                  | if SeqNr equal    to "0xff"   =>  ACK = "11:00"   
                  | if SeqNr inferior to "0xff"   =>  ACK = "11:<SeqNr+1>"
  --------------------------------------------------------------------------------
    Input         | int* SeqNr 
  --------------------------------------------------------------------------------
    Output        | -
 ---------------------------------------------------------------------------------
 */
void Send_ACK (int* SeqNr )
{
  if (*SeqNr == 255) 
    printf(" Send_ACK %s:00\n", ACK_CODE);
  else if (*SeqNr < 15)
    printf(" Send_ACK %s:0%x\n", ACK_CODE, ++(*SeqNr) );
  else
    printf(" Send_ACK %s:%x\n ", ACK_CODE, ++(*SeqNr) );
}



/*int Send_Relay_State(char* msg_parsed , char* msg)
{
  return 0;
}*/



