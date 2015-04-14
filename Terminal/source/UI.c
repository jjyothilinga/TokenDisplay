#include "board.h"
#include "config.h"
#include "Keypad.h"
#include "lcd.h"
#include "string.h"
#include "ui.h"
#include "app.h"



typedef struct _UI
{
	UI_STATE state;
	UI_STATE prevState;
	UINT8 buffer[MAX_INPUT_CHARS+1];
	UINT8 bufferIndex;
	UINT8 prevcode;
	UINT8 keyIndex;
	UINT8 input[MAX_INPUT_CHARS+1];
	UINT8 inputIndex;
	BOOL initDispUpdateFlag;
}UI;





const rom UINT8 keyMap[MAX_KEYS] = { '1','2','3','\x08',
									 '4','5','6','\x0A',
									 '7','8','9','\x0B',
									 '*','0','#','\x0C' } ;


#pragma idata UI_DATA
UI ui = {0,0,{0},0,0xFF,0,0};
OpenIssue openIssue={{0},-1};
OpenIssue ackIssue={{0},-1};
//#pragma idata



UINT8 mapKey(UINT8 scancode, UINT8 duration);
UINT8 getStation(void);
void getData(void);
void clearUIBuffer(void);

void clearUIInput(void);
void showUImsg( UINT8* msg );



void UI_init(void)
{

	UINT8 i;

	LCD_setBackSpace('\x08');	//Indicates LCD driver "\x08" is the symbol for backspace

	ui.state = UI_IDLE;
	clearUIBuffer();

	APP_readEEPROM( ui.buffer );

	for(i = 0; i < MAX_OUTPUT_CHARS; i++)
	{
		if(ui.buffer[i] != '0')
			ui.initDispUpdateFlag = TRUE;
	}
			
//	showUImsg( ui.buffer );
	clearUIInput();
	
}



void UI_task(void)
{

	UINT8 keypressed = 0xFF;
	UINT8 i;
	UINT8 duration, scancode;
	UINT8 uimsg;

	if(ui.initDispUpdateFlag == TRUE)
	{
		showUImsg( ui.buffer );
		APP_call( ui.buffer );
		ui.initDispUpdateFlag = FALSE;
		ui.bufferIndex = 3;
	}


	if(KEYPAD_read(&scancode, &duration) == FALSE)			//Check whether key has been pressed
	{
		return;
	}

	
	keypressed = mapKey(scancode,duration);				//Map the key

	if( keypressed == 0xFF)
	{
		return;
	}


	switch(ui.state)
	{
		case UI_IDLE:



		break;

		case UI_UPDATE:
		//If pressed key is RESET, reset the content of LCD
		if( keypressed == '#' )
		{
			clearUIBuffer( );
			LCD_clear( );
		}

		// If pressed key is backspace handle the buffer and update LCD
		else if( keypressed == '\x08' )
		{
			if(ui.bufferIndex > 0 )
			{
				LCD_putChar(keypressed);
				ui.bufferIndex--;
				ui.buffer[ui.bufferIndex] = '\0';
				if( ui.inputIndex > 0 )
					ui.inputIndex--;
			}
		}
		else
		{
			if(ui.bufferIndex < 3 )
			{
				LCD_putChar(keypressed);
				ui.buffer[ui.bufferIndex++] = keypressed;
			}
		}
		APP_writeEEPROM( ui.buffer );
		ui.state = UI_IDLE;	
		break;


		case UI_CALL:
			APP_call( ui.buffer );
			ui.state = UI_IDLE;		
																										
		
		break;

		case UI_INCREMENT:

			APP_incrementAndCall( ui.buffer );
			APP_writeEEPROM( ui.buffer );
			showUImsg( ui.buffer );
			ui.state = UI_IDLE;
																										
		break;

		case UI_DECREMENT:
			APP_decrementAndCall( ui.buffer );
			APP_writeEEPROM( ui.buffer );
			showUImsg( ui.buffer );
			ui.state = UI_IDLE;																										
		
		break;	

		default:
		break;


	}



}


UINT8 mapKey(UINT8 scancode, UINT8 duration)
{
	UINT8 keypressed = 0xFF;

	switch(ui.state)
	{

		case UI_IDLE:
		keypressed = keyMap[scancode];
		
		if( keypressed == '*' )
			keypressed = 0xFF;
		// If pressed key is CALL change state to UI_CALL
		else if( keypressed == '\x0C' )
			ui.state = UI_CALL;
		// If pressed key is INCREMENT change state to UI_INCREMENT
		else if ( keypressed == '\x0A' )
			ui.state = UI_INCREMENT;
		// If pressed key is DECREMENT change state to UI_DECREMENT
		else if ( keypressed == '\x0B' )
			ui.state = UI_DECREMENT;
		else
			ui.state = UI_UPDATE;


		break;

		case UI_UPDATE:
		keypressed = keyMap[scancode];
		
			if( (keypressed == '\x0A') || (keypressed == '\x0B') || (keypressed == '*') )
				keypressed = 0xFF;
																										
				// If pressed key is CALL change state to UI_CALL
			else if( keypressed == '\x0C' )
				ui.state = UI_CALL;
		break;

		case UI_CALL:
		keypressed = keyMap[scancode];
																								
		
		break;

/*		case UI_INCREMENT:
		keypressed = keyMap[scancode];
																										
		
		break;

		case UI_DECREMENT:
		keypressed = keyMap[scancode];
																										
		
		break;	
*/
		default:keypressed = keyMap[scancode];
		break;
	}

	return keypressed;
}

void getData(void)
{
	UINT8 i;

	for( i = 0; i< ui.bufferIndex; i++)
	{
		ui.input[ui.inputIndex] = ui.buffer[i];
		ui.inputIndex++;
		
	}
	ui.input[ui.inputIndex] = '\0';
	ui.inputIndex++;

	if( ui.inputIndex >= MAX_INPUT_CHARS )
		ui.inputIndex = 0;
}


void clearUIBuffer(void)
{
	UINT8 i;
	for( i = 0; i < MAX_OUTPUT_CHARS; i++ )
		ui.buffer[i] = '0';

	ui.buffer[MAX_OUTPUT_CHARS] = '\0';
	ui.bufferIndex = 0;
//	memset(ui.buffer,0, MAX_OUTPUT_CHARS);
}


void clearUIInput(void)
{
	memset((UINT8*)ui.input,0, MAX_INPUT_CHARS);
	ui.inputIndex = 0;
}




void showUImsg( UINT8* msg )
{
	UINT8 i;

	
	LCD_clear();

	i = 0;
	while( msg[i] != '\0')
	{
		LCD_putChar(msg[i]);
		i++;
	}
}


	

		