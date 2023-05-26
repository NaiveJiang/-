#ifndef _DRIVER_H
#define _DRIVER_H

#include "bsp.h"

#include "driver_key.h"
#include "driver_linearMovement.h"
#include "driver_wdg.h"
#include "driver_dac.h"
#include "driver_can.h"
#include "driver_485.h"
#include "driver_gui.h"
#include "driver_pulsecap.h"
#include "driver_swport.h"

//辅助定时器 每隔10ms中断一次
#define ASSIST_TIM TIM2
#define ASSIST_TIM_PEROID 1000-1
#define ASSIST_TIM_PRESCALER 7200-1
#define ASSIST_TIM_PREEMPTIONPRIORITY 8
#define ASSIST_TIM_SUBPRIORITY 0





void driver_Init(void);

#endif
