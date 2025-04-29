#include "uart_mgmt.h"
#include "usart.h"
#include "my_printf.h"
#include "protocol.h"

#define BUFFER_SIZE	1024
uint8_t rx_buffer[BUFFER_SIZE];
uint16_t rx_len;
uint8_t recv_end_flag;


enum
{
	SHAKE_HANDS,	// ���ֽ׶�
	HEAD_HANDS,		// ͷ���׶�
	DATA_HANDS,		// ���ݽ׶�
	CHECK_HANDS,	// У��׶�
};
uint8_t uart_state = SHAKE_HANDS;	// Ĭ�����ֽ׶�

void Uart_Mgmt_Init(void)
{
	HAL_UART_Receive_IT(&huart1, rx_buffer, 1);


}


void Uart_Send(void *pbuf, uint16_t len)
{
	uint8_t *p = (uint8_t *)pbuf;
	HAL_UART_Transmit(&huart1, p, len, 1000);	// ��������
}


void Uart_RxBuf_Check_SHAKE_HANDS(void)
{
	static uint8_t z_count = 0;

	if(rx_buffer[0] == 'Z')
	{
		z_count++;
		if(z_count >= 3)
		{
			HAL_UART_Transmit(&huart1, (uint8_t *)"AAA", 3, 100);	// ���յ���������ʹ�ô���1���ͳ�ȥ
			uart_state = HEAD_HANDS;	// ���ֳɹ�������ͷ���׶�
			z_count = 0;				// ���ü�����
			HAL_UART_Receive_IT(&huart1, rx_buffer, 17);		// ����ʹ�ܴ���1�����ж�
			return ;
		}
	}
	else
	{
		z_count = 0;					// ��'z'�ַ������ü�����
		uart_state = SHAKE_HANDS;		// ����ʧ�ܣ����½������ֽ׶�
	}
	HAL_UART_Receive_IT(&huart1, rx_buffer, 1);		// ����ʹ�ܴ���1�����ж�
}


uint8_t Head_Pack[1024];
uint8_t Head_pack_idx;
uint32_t Head_pack_idx_idle_Tcik = 0;
void Uart_RxBuf_Check_HEAD_HANDS(void)
{
	if(Head_pack_idx < 1024)
	{
		Head_pack_idx_idle_Tcik = HAL_GetTick();	// ��¼�������ݵ�ʱ���
		Head_Pack[Head_pack_idx] = rx_buffer[0];
		Head_pack_idx++;
		
	}
}


void Uart1_IRQHandler(void)
{

	switch (uart_state)
	{
	case SHAKE_HANDS:
		Uart_RxBuf_Check_SHAKE_HANDS();
		break;
	case HEAD_HANDS:
		Uart_RxBuf_Check_HEAD_HANDS();
		HAL_UART_Receive_IT(&huart1, rx_buffer, 17);		// ����ʹ�ܴ���1�����ж�
		break;
	case DATA_HANDS:
		Uart_RxBuf_Check_HEAD_HANDS();
		HAL_UART_Receive_IT(&huart1, rx_buffer, 519);		// ����ʹ�ܴ���1�����ж�
		break;
	
	default:
		break;
	}


}




void Uart_RxBuf_Check(void)
{
	if((HAL_GetTick() - Head_pack_idx_idle_Tcik > 100) && (Head_pack_idx))	// 0.1����û�н��յ����ݣ���Ϊ���ݽ������
	{
		switch (uart_state)
		{
		case SHAKE_HANDS:
			break;
		case HEAD_HANDS:
			Hand_Pack_Write(Head_Pack, Head_pack_idx);	// ������յ������ݰ�
			Head_pack_idx = 0;	// ������ݰ�����
			Hand_Pack_Deal();	// ������յ������ݰ�
			break;
		case DATA_HANDS:
			Updata_Pack_Write(Head_Pack, Head_pack_idx);	// ������յ������ݰ�
			Head_pack_idx = 0;	// ������ݰ�����
			Updata_Pack_Deal();	// ������յ������ݰ�
			break;
		default:
			break;
		}
	


		
		

	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(&huart1 == huart)
	{
			Uart1_IRQHandler();
			
	}
}
