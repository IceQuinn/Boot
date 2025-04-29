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
	SHAKE_HANDS,	// 握手阶段
	HEAD_HANDS,		// 头部阶段
	DATA_HANDS,		// 数据阶段
	CHECK_HANDS,	// 校验阶段
};
uint8_t uart_state = SHAKE_HANDS;	// 默认握手阶段

void Uart_Mgmt_Init(void)
{
	HAL_UART_Receive_IT(&huart1, rx_buffer, 1);


}


void Uart_Send(void *pbuf, uint16_t len)
{
	uint8_t *p = (uint8_t *)pbuf;
	HAL_UART_Transmit(&huart1, p, len, 1000);	// 发送数据
}


void Uart_RxBuf_Check_SHAKE_HANDS(void)
{
	static uint8_t z_count = 0;

	if(rx_buffer[0] == 'Z')
	{
		z_count++;
		if(z_count >= 3)
		{
			HAL_UART_Transmit(&huart1, (uint8_t *)"AAA", 3, 100);	// 接收到数据马上使用串口1发送出去
			uart_state = HEAD_HANDS;	// 握手成功，进入头部阶段
			z_count = 0;				// 重置计数器
			HAL_UART_Receive_IT(&huart1, rx_buffer, 17);		// 重新使能串口1接收中断
			return ;
		}
	}
	else
	{
		z_count = 0;					// 非'z'字符，重置计数器
		uart_state = SHAKE_HANDS;		// 握手失败，重新进入握手阶段
	}
	HAL_UART_Receive_IT(&huart1, rx_buffer, 1);		// 重新使能串口1接收中断
}


uint8_t Head_Pack[1024];
uint8_t Head_pack_idx;
uint32_t Head_pack_idx_idle_Tcik = 0;
void Uart_RxBuf_Check_HEAD_HANDS(void)
{
	if(Head_pack_idx < 1024)
	{
		Head_pack_idx_idle_Tcik = HAL_GetTick();	// 记录接收数据的时间戳
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
		HAL_UART_Receive_IT(&huart1, rx_buffer, 17);		// 重新使能串口1接收中断
		break;
	case DATA_HANDS:
		Uart_RxBuf_Check_HEAD_HANDS();
		HAL_UART_Receive_IT(&huart1, rx_buffer, 519);		// 重新使能串口1接收中断
		break;
	
	default:
		break;
	}


}




void Uart_RxBuf_Check(void)
{
	if((HAL_GetTick() - Head_pack_idx_idle_Tcik > 100) && (Head_pack_idx))	// 0.1秒内没有接收到数据，认为数据接收完成
	{
		switch (uart_state)
		{
		case SHAKE_HANDS:
			break;
		case HEAD_HANDS:
			Hand_Pack_Write(Head_Pack, Head_pack_idx);	// 处理接收到的数据包
			Head_pack_idx = 0;	// 清空数据包索引
			Hand_Pack_Deal();	// 处理接收到的数据包
			break;
		case DATA_HANDS:
			Updata_Pack_Write(Head_Pack, Head_pack_idx);	// 处理接收到的数据包
			Head_pack_idx = 0;	// 清空数据包索引
			Updata_Pack_Deal();	// 处理接收到的数据包
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
