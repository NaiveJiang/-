#ifndef __DRIVER_WDG_H
#define __DRIVER_WDG_H

#include "stm32f10x_iwdg.h"


void driver_IWDG_Init(uint8_t prer, uint16_t rlr);
void IWDG_Feed(void);

#endif




























