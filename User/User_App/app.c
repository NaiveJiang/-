#include "app.h"
#include "GUI.h"

uint32_t parameter[NAME_OF_LIST];
appStruct_t app;
uint8_t version;
void app_ParameterInit(void){
	
	{
		parameter[NAME_VERSION] = VERSION;
		
		parameter[CONTROL_MODE] = 0;
		
		parameter[POWER_MODE] = 0;
		
		parameter[SPD_SIGNAL] = 0;
	}
	
	bsp_ReadCpuFlash(FLASH_SAVE_START_ADDR + NAME_VERSION,(uint8_t *)&version, 2);
	
	if(version == 0xFF){
		app_FlashWriteUdata();
	}
	else{
		app_FlashReadUdata();
	}
}



