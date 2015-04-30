#include "app.h" 

/*
*------------------------------------------------------------------------------
* Public Variables
* Buffer[0] = seconds, Buffer[1] = minutes, Buffer[2] = Hour,
* Buffer[3] = day, Buffer[4] = date, Buffer[5] = month, Buffer[6] = year
*------------------------------------------------------------------------------
*/



void APP_ASCIIconversion(void);
void APP_resetCounter_Buffer(void);


 

/*
*------------------------------------------------------------------------------
* app - the app structure. 
*------------------------------------------------------------------------------
*/
typedef struct _App
{   
	UINT8 	token[ NO_OF_DIGITS];		//store the data going to be display
	UINT8	tokenFlag;   				// to count the time of callback function exicute
	UINT8 	hooterCount; 				//to maintain hooter on
	UINT8	blinkCount;					//to maintain blink on
	UINT8   tokenBuff[MAX_BUFF_SIZE];	//store the data from callback function
	UINT8 	buffIndex;					//to maintain tokenBuff[MAX_BUFF_SIZE] for data write
	UINT8   curBuffIndex;				//to maintain tokenBuff[MAX_BUFF_SIZE] for data read
}APP;



#pragma idata APP_DATA
APP app = {0};
#pragma idata




/*
*------------------------------------------------------------------------------
* void APP_init(void)
*
* Summary	: Initialize application
*
* Input		: None
*
* Output	: None
*------------------------------------------------------------------------------
*/
void APP_init( void )
{ 
	UINT8 i;

	for(i= 0; i < NO_OF_DIGITS; i++)
	{
		app.token[i] = '0';
	}
	DigitDisplay_updateBuffer(app.token);


}


/*
*------------------------------------------------------------------------------
* void APP_task(void)
*
* Summary	: 
*
* Input		: None
*
* Output	: None
*------------------------------------------------------------------------------
*/

void APP_task( void )
{
	UINT8 i ;

	if(	++app.hooterCount >=  HOOTER_COUNT )
	{
		HOOTER = FALSE;
	}



	if ((app.tokenFlag > 0 )&& (DigitDisplay_Getblinkcount() == 0))
	{
		
		if(app.curBuffIndex  == MAX_BUFF_SIZE)
		{
			app.curBuffIndex  = 0;	
		}

	//fill the data from 
	    for( i = 0  ; i <  NO_OF_DIGITS; i++ , app.curBuffIndex++)
		{
			app.token[i] = app.tokenBuff[app.curBuffIndex] ;
		}

       	DigitDisplay_updateBuffer(app.token);
		HOOTER = TRUE;
		DigitDisplay_blinkOn(1000 ,3);


		app.hooterCount = 0;
		app.blinkCount = 0;
		--app.tokenFlag ;



	}
		
	
	 
}

/*
*------------------------------------------------------------------------------
* void APP_comCallBack(void)
*
* Summary	: 
*
* Input		: None
*
* Output	: None
*------------------------------------------------------------------------------
*/

UINT8 APP_comCallBack( UINT8 *rxPacket, UINT8* txCode, UINT8** txPacket)
{

	UINT8 j ;
	UINT8 length = 0;

            for( j = 0; j < 3; app.buffIndex++ , j++)
			{
				app.tokenBuff[app.buffIndex ] = rxPacket[2 - j] ;
			}

			if(app.buffIndex  == MAX_BUFF_SIZE)
			{
				app.buffIndex  = 0;	
			}
			app.tokenFlag++;

			*txCode = MODIFY_TOKEN;

	return length;

}


