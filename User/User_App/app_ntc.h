#ifndef __APP_NTC_H
#define __APP_NTC_H

#include "util.h"


#define NTC_NUM 101
#define BASE_TEMP 0.0f


extern const uint16_t ntc_adc[NTC_NUM];
uint8_t ntc_look_up(const uint16_t *list,uint16_t data);
float get_ntc_temp(const uint16_t *list,uint16_t adc_val,float base_temp);


#endif
