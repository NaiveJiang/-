#include "app.h"

//中位值滤波 raw_value原始数据 num采样次数(奇数)
float middleValueFilter(__IO float *raw_value,uint8_t num){
    float value_buf[num],temp;
    uint8_t i,j,k;
    for(i = 0; i < num; ++i)
        value_buf[i] = raw_value[i];			
    for(j = 0 ; j < num-1; ++j){
        for(k = 0; k < num-j-1; ++k){
            //从小到大排序，冒泡法排序
            if(value_buf[k] > value_buf[k+1]){
                temp = value_buf[k];
                value_buf[k] = value_buf[k+1];
                value_buf[k+1] = temp;
            }
        }
    }
		
    return value_buf[(num-1)/2];
}

//均值滤波 raw_value原始数据 num采样次数
float averageFilter(__IO float *raw_value,uint8_t num){
    float sum = 0;
    uint8_t i;
    for(i = 0; i < num; ++i)
        sum += raw_value[i];
    return (sum / num);
}

//中位值平均滤波 raw_value原始数据 num采样次数
float middleAverageFilter(__IO float *raw_value,uint8_t num){
    uint8_t i,j,k;
    float temp,sum = 0;
    float value_buf[num];
    for(i = 0; i < num; ++i)
        value_buf[i] = raw_value[i];
    /*从小到大冒泡排序*/	
    for(j = 0; j < num-1; ++j){
        for(k = 0; k < num-j-1; ++k){
            if(value_buf[k] > value_buf[k+1]){
                temp = value_buf[k];
                value_buf[k] = value_buf[k+1];
                value_buf[k+1] = temp;			
            }
        }
    }
    for(i = 1; i < num-1; ++i)
        sum += value_buf[i];
    return sum / (num-2);
}







