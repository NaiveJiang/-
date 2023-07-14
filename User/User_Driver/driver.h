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
#include "driver_roll_changing.h"

#define USE_ASSIST2 1
//辅助定时器1 每隔10ms中断一次
#define ASSIST_TIM1 TIM2
#define ASSIST_TIM1_PEROID 100-1
#define ASSIST_TIM1_PRESCALER 7200-1
#define ASSIST_TIM1_PREEMPTIONPRIORITY 6
#define ASSIST_TIM1_SUBPRIORITY 0
//辅助定时器2 每隔10ms中断一次
#if USE_ASSIST2
#define ASSIST_TIM2 TIM5	
#define ASSIST_TIM2_PEROID 100-1	
#define ASSIST_TIM2_PRESCALER 7200-1
#define ASSIST_TIM2_PREEMPTIONPRIORITY 5
#define ASSIST_TIM2_SUBPRIORITY 0
#endif
void driver_Init(void);

#endif
