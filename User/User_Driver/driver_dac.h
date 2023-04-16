#ifndef _DRIVER_DAC_H
#define _DRIVER_DAC_H

#include "bsp.h"
#include "util.h"

#define PGD3V3 BSP_GPIOA4		//DA-3.3V输出功率给定
#define POUT3V3 BSP_GPIOA5		//DA-3.3V输出功率转电流环输出

void driver_DAC1_Configuration(void);
void driver_DAC2_Configuration(void);
void dac_ch1_voltageOut(float setVoltage);
void dac_ch2_voltageOut(float setVoltage);

#endif


