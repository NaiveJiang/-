#ifndef __APP_FILTER_H
#define __APP_FILTER_H

#include "util.h"

float middleValueFilter(__IO float *raw_value,uint8_t num);
float averageFilter(__IO float *raw_value,uint8_t num);
float middleAverageFilter(__IO float *raw_value,uint8_t num);


#endif


