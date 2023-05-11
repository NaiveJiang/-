#include "app.h"

//��λֵ�˲� raw_valueԭʼ���� num��������(����)
float middleValueFilter(__IO float *raw_value,uint8_t num){
    float value_buf[num],temp;
    uint8_t i,j,k;
    for(i = 0; i < num; ++i)
        value_buf[i] = raw_value[i];			
    for(j = 0 ; j < num-1; ++j){
        for(k = 0; k < num-j-1; ++k){
            //��С��������ð�ݷ�����
            if(value_buf[k] > value_buf[k+1]){
                temp = value_buf[k];
                value_buf[k] = value_buf[k+1];
                value_buf[k+1] = temp;
            }
        }
    }
		
    return value_buf[(num-1)/2];
}

//��ֵ�˲� raw_valueԭʼ���� num��������
float averageFilter(__IO float *raw_value,uint8_t num){
    float sum = 0;
    uint8_t i;
    for(i = 0; i < num; ++i)
        sum += raw_value[i];
    return (sum / num);
}

//��λֵƽ���˲� raw_valueԭʼ���� num��������
float middleAverageFilter(__IO float *raw_value,uint8_t num){
    uint8_t i,j,k;
    float temp,sum = 0;
    float value_buf[num];
    for(i = 0; i < num; ++i)
        value_buf[i] = raw_value[i];
    /*��С����ð������*/	
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







