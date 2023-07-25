#ifndef __DRIVER_ROLL_CHANGING_H
#define __DRIVER_ROLL_CHANGING_H

#include "bsp.h"

#define HJSH_PORT BSP_GPIOE11		//»»¾íÐÅºÅ
#define HJSH PEin(11)
#define ROLL_CHANGING_PREEMPTIONPRIORITY 6
#define ROLL_CHANGING_SUBPRIORITY 0

void driver_roll_changing(void);






#endif


