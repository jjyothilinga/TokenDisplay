 
/*
*------------------------------------------------------------------------------
* main.c
*
*------------------------------------------------------------------------------
*/


#pragma config FOSC 	= INTIO67
#pragma config PLLCFG 	= OFF
#pragma config PRICLKEN = OFF
#pragma config FCMEN 	= OFF
#pragma config IESO 	= OFF
#pragma config PWRTEN   = OFF
#pragma config BOREN 	= OFF
#pragma config BORV 	= 285
#pragma config WDTEN    = OFF
#pragma config WDTPS 	= 512
#pragma config CCP2MX 	= PORTC1
#pragma config PBADEN 	= OFF
#pragma config MCLRE 	= EXTMCLR
#pragma config STVREN 	= OFF
#pragma config XINST 	= OFF
#pragma config LVP      = OFF
#pragma config CP0      = OFF
#pragma config CP1      = OFF
#pragma config CP2      = OFF
#pragma config CP3      = OFF
#pragma config CPB      = OFF
#pragma config CPD      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
#pragma config WRT2     = OFF
#pragma config WRT3     = OFF
#pragma config WRTB     = OFF       
#pragma config WRTC     = OFF
#pragma config WRTD     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
#pragma config EBTR2    = OFF
#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF





/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include <timers.h>				// Timer library functions
#include <delays.h>				// Delay library functions
#include <string.h>				// String library functions
#include "board.h"				// board setup
#include "timer.h"
#include "heartbeat.h"
#include "config.h"
#include "uart.h"
#include "communication.h"
#include "digitdisplay.h"
#include "app.h"
/*
*------------------------------------------------------------------------------
* Private Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Processor config bits
*------------------------------------------------------------------------------
*/



/*
*------------------------------------------------------------------------------
* Private Macros
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables
*------------------------------------------------------------------------------
*/


/*

*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Constants
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Constants (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Function Prototypes (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* void main(void)

* Summary	: Application specifc main routine. Initializes all port and
*			: pheriperal and put the main task into an infinite loop.
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
#define DIGIT_REFRESH_PERIOD	(65535 - 8000)

void main(void)
{
	unsigned long temp;

	//board initialization
	BRD_init();

	DigitDisplay_init( NO_OF_DIGITS);		
	//initialize timer0
	TMR0_init(DIGIT_REFRESH_PERIOD,DigitDisplay_task);
	//initialize timer1
//	TMR1_init(DIGIT_REFRESH_PERIOD,DigitDisplay_task);
	//initialize timer2
//	TMR2_init(30,DigitDisplay_task);	
	//initialize Communication
    COM_init(CMD_SOP , CMD_EOP ,RESP_SOP , RESP_EOP , APP_comCallBack);
	APP_init();
	//Interrupts initialization
	EnableInterrupts();	

	//Heart Beat to blink at every 500ms
	temp = (500UL *1000UL)/TIMER0_TIMEOUT_DURATION;
	
	while(1)
	{

		//Heart Beat to blink at every 500ms
	`	if(heartBeatCount >= temp )
		{			
			HB_task();
			heartBeatCount = 0;
		}

		if(AppUpdate_count>= 50 )
		{
		  	APP_task();
        	AppUpdate_count= 0;
		
		}
		
     	COM_task();
	}
		
}




/*
*  End of main.c
*/
