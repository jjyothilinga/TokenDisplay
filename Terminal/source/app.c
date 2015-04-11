
/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/
#include "config.h"
#include "board.h"
#include "timer.h"
#include "communication.h"
#include "app.h"
#include "keypad.h"
#include "lcd.h"
#include "string.h"
#include "eep.h"
#include "ui.h"


//#define SIMULATION
/*
*------------------------------------------------------------------------------
* Structures
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Variables
*------------------------------------------------------------------------------
*/


/*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/


UINT8 APP_comCallBack( far UINT8 *rxPacket, far UINT8* txCode,far UINT8** txPacket);


/*
*------------------------------------------------------------------------------
* void APP-init(void)
*------------------------------------------------------------------------------
*/

void APP_init(void)
{

	

	COM_init(CMD_SOP , CMD_EOP ,RESP_SOP , RESP_EOP , APP_comCallBack);



}


/*
*------------------------------------------------------------------------------
* void APP-task(void)
*------------------------------------------------------------------------------
*/
void APP_task(void)
{


}



void APP_incrementAndCall(UINT8 *buffer)
{
	UINT8 temp, i;
	
	
	//increment buffer value
	if( buffer[2] < '9')
		buffer[2]++;
	else
	{
		buffer[2] = '0';
		
		if(buffer[1] < '9')
			buffer[1]++;		
		else
		{
			buffer[1] = '0';

			if(buffer[0] < '9')
				buffer[0]++;
			else
				temp = TRUE;
		}
	}
	
	//Reset buffer if it exceeds its limit
	if(temp == TRUE)
	{
		for(i = 0; i < MAX_OUTPUT_CHARS; i++)
			buffer[i] = '0';
	}

	COM_txCMD( DEVICE_ADDRESS, buffer, 3);

}



void APP_decrementAndCall(UINT8 *buffer)
{
	//decrement buffer value
	if(buffer[2] > '0')
		buffer[2]--;
	else
	{
		if( buffer[1] > '0' )
		{
			buffer[1]--;	
			buffer[2] = '9';
		}	
		else
		{
			if( buffer[0] > '0' )
			{
				buffer[0]--;
				buffer[1] = '9';
				buffer[2] = '9';
			}
		}
	}

	COM_txCMD( DEVICE_ADDRESS, buffer, 3);
}

void APP_call( UINT8 *buffer )
{
	COM_txCMD( DEVICE_ADDRESS, buffer, 3);
}





UINT8 APP_comCallBack( far UINT8 *rxPacket, far UINT8* txCode,far UINT8** txPacket)
{

	UINT8 i;

	UINT8 rxCode = rxPacket[0];
	UINT8 length = 0;

	switch( rxCode )
	{
		case CMD_GET_STATUS:

			*txCode = CMD_GET_STATUS;
			break;

		case CMD_RESOLVE_ISSUE:
		
			*txCode = CMD_RESOLVE_ISSUE;
			length = 0;
			
			break;

		case CMD_CLEAR_ISSUES:
		
			*txCode = CMD_CLEAR_ISSUES;
			length = 0;
			
			break;

	

		case CMD_PING:
			length = 0;
			*txCode = CMD_PING;
			break;

		default:
			length = 0;
			*txCode = COM_RESP_INVALID_CMD;
			break;

	}

	return length;

}
	
	
	

