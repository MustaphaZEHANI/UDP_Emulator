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
  //Check if the message Length is OK
  if (strlen(msg) != msg_Len)
  {
    if(debug)
      printf ("[DEBUG_MESSAGE] %s ERROR : Wrong Message Length = %d\n", __func__, strlen(msg));
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
  // Check if the message format contain this special char ':' exist
  if(msg[2] != ':')
  {
    if(debug)
      printf ("[DEBUG_MESSAGE] %s : Message format is incorrect !\n", __func__ );
    return ERROR;
  } 

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
  ----------------------------------------------------------------------------------------
    Name          | int Byte_Parser (int* Byte_parsed , char* msg_parsed, char* msg )
  ----------------------------------------------------------------------------------------
    Description   | Parsing the byte after the first ':'  and convert it to hexadecimal
                  | e.g msg: "21:a4:44:33:01" >> Byte_parsed "a4" | msg_parsed "a4:44:33:01"
  ----------------------------------------------------------------------------------------
    Inputs        | char* msg_parsed, char* msg 
  ----------------------------------------------------------------------------------------
    Outputs       | return 0 : SUCCESS
                  |       -1 : ERROR
 -----------------------------------------------------------------------------------------
*/
int Byte_Parser (int* byte_parsed , char* msg_parsed , char* msg )
{
  // Find the first Byte (string format) after the caractere ':'
  strcpy (msg_parsed ,  strchr(msg , ':') + 1 );
  /* 
    Convert the byte from string to hexadecimal (base 16) and check if there is 
    a special character (or an alphabet) using the function strtol().
  */
  char* SpChar_detected; 
  *byte_parsed =  strtol (msg_parsed , &SpChar_detected, 16);
  if(debug)
    printf("[DEBUG_MESSAGE] %s :  SpChar_detected= '%c', msg_parsed = %s, *byte_parsed = %x\n"
          , __func__, *SpChar_detected, msg_parsed, *byte_parsed );

  // Check if the Byte_parsed is between [0,255] and with the right format
  if  ( ( *byte_parsed < 0 || *byte_parsed > 255 ) || 
        ( *SpChar_detected != ':' && strlen(SpChar_detected) ) )//*SpChar_detected != NULL 
  {
    if (debug)
      printf("[DEBUG_MESSAGE][ERROR] %s : Wrong byte format detected, *SpChar_detected= %c \n"
            , __func__, *SpChar_detected );
    return ERROR;
  }

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
  if (ret == ERROR)
  {
    if(debug) 
      printf("[DEBUG_MESSAGE] %s : The message format is incorrect !\n", __func__);
    return ERROR;
  }

  if(debug) 
    printf("[DEBUG_MESSAGE] %s SUCCESS: SeqNr (hex) = %x \n", __func__, *SeqNr);
  return ret;
}

/* 
  ---------------------------------------------------------------------------------------------------
    Name          | int Parsing_RelayCountByte (int* RelayCount_parsed , char* msg_parsed , char* msg)
  ---------------------------------------------------------------------------------------------------
    Description   | Parsing the RelayCount byte to identify the number of relay (1 ou 2)
  ---------------------------------------------------------------------------------------------------
    Inputs        | int* RelayCount_parsed , char* msg_parsed , char* msg
  ---------------------------------------------------------------------------------------------------
    Outputs       | return 0 SUCCESS
                  |       -1 ERROR
 ----------------------------------------------------------------------------------------------------
 */
int Parsing_RelayCountByte (int* RelayCount_parsed , char* msg_parsed , char* msg)
{
  int ret = Byte_Parser (RelayCount_parsed , msg_parsed , msg);
  if(debug) 
    printf("[DEBUG_MESSAGE] %s : RelayCount_parsed = %d \n", __func__, *RelayCount_parsed);
  if (ret == ERROR)
  {
    if(debug) 
      printf("[DEBUG_MESSAGE][ERROR] %s : ERROR :  Wrong RelayCountByte \n", __func__);
    return ret;
  }
  return ret;
}

/* 
  ---------------------------------------------------------------------------------------------------
    Name          | int Parsing_RelayNbrByte (int* RelayNbr_parsed , char* msg_parsed , char* msg)
  ---------------------------------------------------------------------------------------------------
    Description   | Parsing the RelayNbr byte to identify the relay number (0 or 1)
  ---------------------------------------------------------------------------------------------------
    Inputs        | int* RelayNbr_parsed , char* msg_parsed , char* msg
  ---------------------------------------------------------------------------------------------------
    Outputs       | return 0 SUCCESS
                  |       -1 ERROR
 ----------------------------------------------------------------------------------------------------
 */
int Parsing_RelayNbrByte (int* RelayNbr_parsed , char* msg_parsed , char* msg)
{
  int ret = ERROR;
  
  ret = Byte_Parser (RelayNbr_parsed , msg_parsed , msg);
  if (debug)
    printf("[DEBUG_MESSAGE] %s :msg_parsed = %s  RelayNbr_parsed = %d \n"
          , __func__, msg_parsed, *RelayNbr_parsed);
  if (ret == ERROR)
  {
    if(debug) 
      printf("[DEBUG_MESSAGE][ERROR] %s : Parsing the Relay number Byte KO !\n", __func__);
    return ret;
  }

  if (*RelayNbr_parsed == Relay_Nbr_0)
    printf(" ...Relay 0 :");
  else if (*RelayNbr_parsed == Relay_Nbr_1)
    printf(" ...Relay 1 :");
  else
  {
    if(debug)
      printf("[DEBUG_MESSAGE] %s : Wrong Relay number");
    return ERROR;
  }

  return ret;
}

/* 
  ---------------------------------------------------------------------------------------------------
    Name          | int Parsing_RelayValueByte (int* RelayNbr_parsed , char* msg_parsed , char* msg)
  ---------------------------------------------------------------------------------------------------
    Description   | Parsing the RelayValue byte to identify the relay state (open(0) or close(1))
  ---------------------------------------------------------------------------------------------------
    Inputs        | int* RelayValue_parsed , char* msg_parsed , char* msg
  ---------------------------------------------------------------------------------------------------
    Outputs       | return 0 SUCCESS
                  |       -1 ERROR
 ----------------------------------------------------------------------------------------------------
 */
int Parsing_RelayValueByte (int* RelayValue_parsed , char* msg_parsed , char* msg)
{
  int ret = ERROR;
  ret = Byte_Parser (RelayValue_parsed , msg_parsed , msg);
  if (debug)
    printf("[DEBUG_MESSAGE] %s :msg_parsed = %s  Relay Value_parsed = %d \n"
          , __func__, msg_parsed, *RelayValue_parsed);
  if (ret == ERROR)
  {
    if(debug) 
      printf("[DEBUG_MESSAGE][ERROR] %s : Parsing the Relay Value Byte KO !\n", __func__);
    return ret;
  }

  // Check the Relay Value
  if (*RelayValue_parsed == Relay_Open)
    printf(" Open \n");
  
  else if (*RelayValue_parsed == Relay_Close)
    printf(" Close \n");

  else
  {
    if(debug)
      printf("[DEBUG_MESSAGE] %s : The message format is incorrect! Wrong Relay number\n");
    return ERROR;
  }

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
int Parsing_RelaySignalBytes (char* msg_parsed , char* msg)
{
  int ret = ERROR;
  int Relay_Count, Relay_Nbr, Relay_Value;

  // Parsing the RelayCount Byte
  ret = Parsing_RelayCountByte (&Relay_Count , msg_parsed , msg);
  if(ret == ERROR)
    return ERROR;
  // 1 Relay detected
  if (Relay_Count == Relay_Count_1)
  {  
    // Check the message length
    ret = Check_Message_Length (RELAY_SIGNAL_LENGTH_1 - 6, msg);
    if(ret == ERROR)
      return ERROR;

    //Parsing Relay Nbr Byte
    ret = Parsing_RelayNbrByte (&Relay_Nbr , msg_parsed , msg_parsed) ;
    if(ret == ERROR)
      return ERROR;
    
    //Parsing RelayValueByte
    ret = Parsing_RelayValueByte (&Relay_Value , msg_parsed , msg_parsed);
    if (ret == ERROR)
      return ERROR;
  }
  // 2 Relay detected
  else if (Relay_Count == Relay_Count_2)
  {
    // Check the message length
    ret = Check_Message_Length (RELAY_SIGNAL_LENGTH_2 - 6, msg);
    if(ret == ERROR)
      return ERROR;

    //Parsing Relay Nbr Byte
    int i;
    for(i=0 ; i <Relay_Count_2 ; i++)
    {
      ret = Parsing_RelayNbrByte (&Relay_Nbr , msg_parsed , msg_parsed) ;
      if(ret == ERROR)
        return ERROR;

      ret = Parsing_RelayValueByte (&Relay_Value , msg_parsed , msg_parsed);
      if (ret == ERROR)
        return ERROR;
    }
  }
  else 
  {
    if(debug) 
      printf("[DEBUG_MESSAGE][ERROR] %s : Wrong Relay_Count = %d\n", __func__, Relay_Count); 
    return ERROR;
  }

  return SUCCESS;
}

/* 
  --------------------------------------------------------------------------------
    Name          | void Send_ACK (int* SeqNr )
  --------------------------------------------------------------------------------
    Description   | Send the Acknowledge signal dependant on the Sequence number
                  | if SeqNr equal to "0xff"      =>  ACK = "11:00"   
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
  {
    if(debug)
      printf("[DEBUG_MESSAGE] Send_ACK %s:00\n", ACK_CODE);
  }
  else if (*SeqNr < 15)
  {
    if(debug)
      printf("[DEBUG_MESSAGE] Send_ACK %s:0%x\n", ACK_CODE, ++(*SeqNr) );
  }
  else
  {
    if(debug)
      printf("[DEBUG_MESSAGE] Send_ACK %s:%x\n", ACK_CODE, ++(*SeqNr) );
  }

}

/* 
  --------------------------------------------------------------------------------
    Name          | void Send_Relay_State(char* msg_parsed , char* msg)
  --------------------------------------------------------------------------------
    Description   | Send the Relay State signal dependant on the Relay value
  --------------------------------------------------------------------------------
    Input         | int* SeqNr 
  --------------------------------------------------------------------------------
    Output        | -
 ---------------------------------------------------------------------------------
 */
void Send_Relay_State(char* msg_parsed , char* msg)
{
  if(debug)
    printf(" Send Relay State %s:0%x:0%x\n", RELAY_STATE_SIGNAL_CODE
          , RELAY_STATE_SEQ_NUMB, Relay_0.Value);
}

