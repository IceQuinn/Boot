#ifndef __PROTOCOL_H
#define __PROTOCOL_H
#include <stdint.h>


void Hand_Pack_Write(uint8_t *pbuf, uint8_t len);
void Hand_Pack_Deal(void);
void Updata_Pack_Write(uint8_t *pbuf, uint8_t len);
void Updata_Pack_Deal(void);

#endif


