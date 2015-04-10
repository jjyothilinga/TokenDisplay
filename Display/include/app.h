#include "config.h"
#include "board.h"
#include "communication.h"
#include "eep.h"
#include "digitdisplay.h"
#include "typedefs.h"


typedef enum 
{
	WAIT = 0,
	COUNT 
}APP_STATE;



 typedef enum 
{
	START_PB = 0
	
}PB;

extern void APP_init(void);
UINT8 APP_comCallBack( UINT8 *rxPacket, UINT8* txCode, UINT8** txPacket);
void APP_task(void);


