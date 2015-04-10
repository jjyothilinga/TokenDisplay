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
	UINT8 	token[ NO_OF_DIGITS];
	UINT8	tokenFlag;
	UINT16 	hooterCount;
	UINT8   tokenNO;


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
		app.token[i] = Read_b_eep(EPROM_TOKEN  + i  );
		Busy_eep();
	}
/*
	app.Actual_Count = Read_b_eep(EPROM_ADD_ACTUAL);
	Busy_eep();
	app.Actual_Count <<= 8;
	app.Actual_Count |= Read_b_eep(EPROM_ADD_ACTUAL+1);
	Busy_eep();

	APP_ASCIIconversion();
*/
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
	UINT8 i;

	app.hooterCount++;
	if(	app.hooterCount >=  HOOTER_COUNT )
	{
		HOOTER = FALSE;
	}

	if(app.tokenFlag == TRUE)
	{
        for( i = 0; i < NO_OF_DIGITS; i++)
		{
			Write_b_eep(EPROM_TOKEN + i , app.token[i]);
			Busy_eep();
		}

		app.tokenFlag = FALSE;
	}		
	
			
	 
}

/*
*------------------------------------------------------------------------------
* void APP_conversion(UINT16)
*
* Summary	: 
*
* Input		: None
*
* Output	: None
*------------------------------------------------------------------------------
*/
void APP_ASCIIconversion(void)
{  
	UINT8 i=0;
    UINT16 count = app.tokenNO;

	for( i = 0; i < 4; i++)
	{
	      app.token[i] = (count % 10)  + '0';
		   count /= 10;
	}
 
}


/*
*------------------------------------------------------------------------------
* void APP_resetCounter_Buffer(void)
*
* Summary	: 
*
* Input		: None
*
* Output	: None
*------------------------------------------------------------------------------
*/
void APP_resetCounter_Buffer(void)
{
	UINT8 i;
	for(i = 0; i < NO_OF_DIGITS; i++)			//reset all digits
	{
	//	app.Actual[i] = '0';
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

	UINT8 i;
   	

	UINT8 rxCode = rxPacket[0];

	UINT8 length = 0;
		    	
	switch( rxCode )
 	{
	  	case MODIFY_TOKEN:
			
			app.hooterCount = 0;
            for( i = 0; i < NO_OF_DIGITS; i++)
			{
				app.token[i] = rxPacket[i+1];
			}
    
           	DigitDisplay_updateBuffer(app.token);
			HOOTER = TRUE;
			app.tokenFlag = TRUE;
			*txCode = MODIFY_TOKEN;
		   
		break;

		default:
		break;

	}

	return length;

}