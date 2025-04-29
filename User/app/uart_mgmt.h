#ifndef __UART_MGMT_H
#define __UART_MGMT_H
#include <stdint.h>

void Uart_Mgmt_Init(void);

void Uart_RxBuf_Check(void);

void Uart_Send(void *pbuf, uint16_t len);

#endif

