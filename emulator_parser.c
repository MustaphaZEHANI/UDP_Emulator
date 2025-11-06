#include "emulator_parser.h"

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
      printf ("[DEBUG_SERVER] %s Message Length = %lu\n", __func__, strlen(msg));
    return ERROR;
  }
  if(debug)
    printf ("[DEBUG_SERVER] %s correct \n", __func__ );
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
      printf ("[DEBUG_SERVER] %s : Message format is incorrect !\n", __func__ );
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
  char* SpChar_detected = NULL; 
  *byte_parsed =  strtol (msg_parsed , &SpChar_detected, 16);
  if(debug)
    printf("[DEBUG_SERVER] %s :  SpChar_detected '%c', msg_parsed = %s, *byte_parsed = %x\n"
          , __func__, *SpChar_detected, msg_parsed, *byte_parsed );

  /* Check if the Byte_parsed is between [0,255] and with the 
     right format, the code ascii should be more than 105
  */
  if  ( *byte_parsed < 0 || *byte_parsed > 255 || 
      ( SpChar_detected[0] > 105 && SpChar_detected[0] != ':') )
  { 

    if (debug)
      printf("[DEBUG_SERVER][ERROR] %s : Wrong byte format detected, *SpChar_detected= %d \n"
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
      printf("[DEBUG_SERVER] %s : The message format is incorrect !\n", __func__);
    return ERROR;
  }

  if(debug) 
    printf("[DEBUG_SERVER] %s SUCCESS: SeqNr (hex) = %x \n", __func__, *SeqNr);
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
    printf("[DEBUG_SERVER] %s : RelayCount_parsed = %d \n", __func__, *RelayCount_parsed);
  if (ret == ERROR)
  {
    if(debug) 
      printf("[DEBUG_SERVER][ERROR] %s : ERROR :  Wrong RelayCountByte \n", __func__);
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
    printf("[DEBUG_SERVER] %s :msg_parsed = %s  RelayNbr_parsed = %d \n"
          , __func__, msg_parsed, *RelayNbr_parsed);
  if (ret == ERROR)
  {
    if(debug) 
      printf("[DEBUG_SERVER][ERROR] %s : Wrong Relay number\n", __func__);
    return ret;
  }

  if (*RelayNbr_parsed == Relay_Nbr_0)
  {
    if(debug) 
      printf("[DEBUG_SERVER] Relay Number 0 \n");
    return SUCCESS;
  }
  else if (*RelayNbr_parsed == Relay_Nbr_1)
  {
    if(debug) 
      printf("[DEBUG_SERVER] Relay Number 1 \n");
    return SUCCESS;
  }
  else
  {
    if(debug)
      printf("[DEBUG_SERVER] %s : Wrong Relay number", __func__);
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
    printf("[DEBUG_SERVER] %s :msg_parsed = %s  Relay Value_parsed = %d \n"
                  , __func__, msg_parsed, *RelayValue_parsed);
  if (ret == ERROR)
  {
    if(debug) 
      printf("[DEBUG_SERVER][ERROR] %s : Wrong Relay Value \n", __func__);
    return ret;
  }

  // Check the Relay Value  
  if (*RelayValue_parsed == Relay_Open)
  {
    if (debug)
      printf("[DEBUG_SERVER] Relay Status: Open \n");
  }
  else if (*RelayValue_parsed == Relay_Close)
  {
    if (debug)
      printf("[DEBUG_SERVER] Relay Status: Close \n");
  }
  else
  {
    if(debug)
      printf("[DEBUG_SERVER][ERROR] %s : Wrong Relay number\n", __func__);
    return ERROR;
  }

  return SUCCESS;
}
/* 
  ------------------------------------------------------------------------------------
    Name          | int Set_Relay_Value (int* RelayValue_parsed , int* RelayNbr_parsed)
  ------------------------------------------------------------------------------------
    Description   | Set the Relay Value to Open or Close it
  ------------------------------------------------------------------------------------
    Inputs        | int* RelayValue_parsed , int* RelayNbr_parsed
  ------------------------------------------------------------------------------------
    Outputs       | return 0 SUCCESS
                  |       -1 ERROR
 -------------------------------------------------------------------------------------
 */
int Set_Relay_Value (int* RelayValue_parsed , int* RelayNbr_parsed)
{
  if (*RelayNbr_parsed == Relay_Nbr_0)
  {
    Relay_0.Value = *RelayValue_parsed;
    if (debug)
      printf("[DEBUG_SERVER] Relay Number 0 Value = %d\n",Relay_0.Value);
    
    // Print the relay State in Received message 
    if (Relay_0.Value == Relay_Open)
      printf("Relay 0 : Open\n");
    else
      printf("Relay 0 : Close\n");

  }
  else if (*RelayNbr_parsed == Relay_Nbr_1)
  {
    Relay_1.Value = *RelayValue_parsed;
    if (debug)
      printf("[DEBUG_SERVER] Relay Number 1 Value = %d\n",Relay_1.Value);

    // Print the relay State in Received message 
    if (Relay_1.Value == Relay_Open)
      printf("Relay 1 : Open\n");
    else
      printf("Relay 1 : Close\n");

  }
  else
    return ERROR;

return SUCCESS;
  
}

/* 
  -----------------------------------------------------------------------------
    Name          | int  Parsing_SeqNrByte  (char* msg_parsed , char* msg)
  -----------------------------------------------------------------------------
    Description   | Parsing the Relay Signal Byte and Open or Close it 
  -----------------------------------------------------------------------------
    Inputs        | char* msg_parsed  ; char* msg
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
    Set_Relay_Value(&Relay_Value , &Relay_Nbr);
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
      Set_Relay_Value(&Relay_Value , &Relay_Nbr);
    }
  }
  else 
  {
    if(debug) 
      printf("[DEBUG_SERVER][ERROR] %s : Wrong Relay_Count = %d\n", __func__, Relay_Count); 
    return ERROR;
  }

  return SUCCESS;
}

/* 
  --------------------------------------------------------------------------------
    Name          | void ACK_ToSend (char* msg , int* SeqNr )
  --------------------------------------------------------------------------------
    Description   | Make the Acknowledge signal with Sequence number 
  --------------------------------------------------------------------------------
    Input         | char* msg , int* SeqNr 
  --------------------------------------------------------------------------------
    Output        | -
 ---------------------------------------------------------------------------------
 */
void ACK_ToSend (char* msg , int* SeqNr )
{

  strcpy (msg,ACK_CODE) ;
  if (*SeqNr == 255) 
  {
    strcpy (msg+2,":00") ;
    if(debug)
      printf("[DEBUG_SERVER] Send ACK Signal = %s\n", msg);
  }
  else if (*SeqNr < 15)
  {
    sprintf (msg+2,":0%x",++(*SeqNr)) ;
    if(debug)
      printf("[DEBUG_SERVER] Send ACK Signal = %s\n", msg);
  }
  else
  {
    sprintf (msg+2,":%x",++(*SeqNr)) ;
    if(debug)
      printf("[DEBUG_SERVER] Send ACK Signal = %s\n", msg);
  }

  msg [5]= '\n';
  msg [6]= '\0';
}

/* 
  --------------------------------------------------------------------------------
    Name          | void RelayState_ToSend(char* msg_parsed , char* msg)
  --------------------------------------------------------------------------------
    Description   | Send the Relay State signal dependant on the Relay value
  --------------------------------------------------------------------------------
    Input         | int* SeqNr 
  --------------------------------------------------------------------------------
    Output        | -
 ---------------------------------------------------------------------------------
 */
void RelayState_ToSend(char* msg )
{
  strcpy (msg,RELAY_STATE_SIGNAL_CODE) ;

  sprintf (msg+2,":0%x", RELAY_STATE_SEQ_NUMB) ;
  
  sprintf (msg+5,":0%x", Relay_0.Value) ;

  msg [8]= '\n';
  msg [9]= '\0';

  if(debug)
    printf("[DEBUG_SERVER] Send Relay State Signal = %s \n", msg );
}

