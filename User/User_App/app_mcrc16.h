#ifndef _APP_MCRC16_H
#define _APP_MCRC16_H

extern const uint8_t auchCRCHi[];
extern const uint8_t auchCRCLo[];

uint16_t Get_MODBUS_CRC16_Checksum(uint8_t *puchMsgg,uint8_t usDataLen);
void Append__MODBUS_CRC16_Checksum(uint8_t *puchMsgg,uint8_t usDataLen);
uint32_t Verify_MODBUS_CRC16_Checksum(uint8_t *puchMsgg,uint8_t usDataLen);


#endif

