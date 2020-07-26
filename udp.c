#include <stdio.h>
#include <stdlib.h>
#include "udp.h"

/* 
  -----------------------------------------------------------------------------
    Name          | int  MessageCode_Rec (char msg[])
  -----------------------------------------------------------------------------
    Description   | Identify the received message type using the signal code 
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

int Parsing_CodeByte (char msg[])
{
    switch(atoi(msg))
    {
        case 21:
            if (debug) 
                printf("[DEBUG_MESSAGE] Relay Signal Code Detected \n");
            return 0;

        case 31:
            if (debug) 
                printf("[DEBUG_MESSAGE] Get State Signal Code Detected\n");
            return 1;

        case 51:
            if (debug) 
                printf("[DEBUG_MESSAGE] Quit Signal Code Detected\n");
            return 2;
        
        default:
            return -1;
    }
}

/* 
  -----------------------------------------------------------------------------
    Name          | int  Parsing_SeqNrByte  (int SeqNr , char msg[])
  -----------------------------------------------------------------------------
    Description   | Identify the message sequence number 
  -----------------------------------------------------------------------------
    Inputs        | int SeqNr ; char* msg []
  -----------------------------------------------------------------------------
    Outputs       | return the sequence number
 ------------------------------------------------------------------------------
 */
int Parsing_SeqNrByte (int SeqNr , char msg[])
{
    return 0;
}

int Parsing_RelaySignalBytes (int SeqNr , char msg[])
{
    
    return 0;
}

int Send_Relay_State(int SeqNr , char msg[])
{
    return 0;
}
    


