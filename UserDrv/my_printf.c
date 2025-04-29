#include "my_printf.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "usart.h"

#define TX_BUF_LEN  256     /* ���ͻ�����������������Ҫ���е��� */
uint8_t TxBuf[TX_BUF_LEN];  /* ���ͻ�����                       */

uint8_t g_Send_Flg = 0;
void MyPrintf(const char *__format, ...)
{
  va_list ap;
  va_start(ap, __format);
  
  /* ��շ��ͻ����� */
  memset(TxBuf, 0x0, TX_BUF_LEN);
  
  /* ��䷢�ͻ����� */
  vsnprintf((char*)TxBuf, TX_BUF_LEN, (const char *)__format, ap);
  va_end(ap);
  int len = strlen((const char*)TxBuf);
  
  /* �����ڷ������� */
//  HAL_UART_Transmit_DMA(&huart2, (uint8_t*)&TxBuf, len);
	HAL_UART_Transmit(&huart2, (uint8_t*)&TxBuf, len, 1000);

}


uint8_t aa;
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart2)
	{
		aa++;
	}
}

void show_arr(char* name, void* data, int len)
{
    MyPrintf("[%08d]:", HAL_GetTick());

    char * ptr = (char *)data;
    MyPrintf("%s[%d] = 0x", name, len);
    for(int i=0; i<len; i++)
        MyPrintf("%02x ", ptr[i]);
    MyPrintf("\r\n");

    return;
}
