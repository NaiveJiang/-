#ifndef __APP_SERIAL_H
#define __APP_SERIAL_H

#define SERIAL_TASK_PEROID 2

typedef enum{
	SERIAL_PRIORITY = 5,
	SERIAL_STACK_SIZE = 256,
}appSerial_e;

void app_serialTaskInit(void);

#endif
