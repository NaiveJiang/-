#include "app.h"
#include "math.h"

__IO dry_pageStruct dryData;
__IO dry_pageStruct *get_drydata(void){
	return &dryData;
}

//解析得到湿启动页面的数据  湿启动页面返回的数据   需要读取的状态，0位湿启动状态位
uint8_t get_dryData(uint8_t dry_data,dryData_e data_state){
	return (dry_data & data_state) >> ((uint8_t)sqrt(data_state));
}

