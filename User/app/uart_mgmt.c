#include "uart_mgmt.h"
#include "usart.h"

#define BUFFER_SIZE	512
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


void Uart_IDLE_RxCallback(uint16_t DMA_Idx)
{
	rx_len =  BUFFER_SIZE - DMA_Idx; //总计数减去未传输的数据个数，得到已经接收的数据个数
	HAL_UART_Receive_DMA(&huart1, rx_buffer, BUFFER_SIZE);
	recv_end_flag = 1;	// 接受完成标志位置1
}


void Uart_RxBuf_Check_SHAKE_HANDS(void)
{
	static uint8_t z_count = 0;

	if(rx_buffer[0] == 'z')
	{
		z_count++;
		if(z_count >= 3)
		{
			uart_state = HEAD_HANDS;	// 握手成功，进入头部阶段
			z_count = 0;				// 重置计数器
			HAL_UART_Transmit(&huart1, (uint8_t *)"AAA", 3, 100);	// 接收到数据马上使用串口1发送出去
		}
	}
	else
	{
		z_count = 0;					// 非'z'字符，重置计数器
		uart_state = SHAKE_HANDS;		// 握手失败，重新进入握手阶段
	}
}

void Uart_RxBuf_Check_HEAD_HANDS(void)
{
//	uint32_t tmp_flag = 0;
//	uint32_t temp;
//	tmp_flag =__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE); //获取IDLE标志位
//	if((tmp_flag != RESET))//idle标志被置位
//	{ 
//		__HAL_UART_CLEAR_IDLEFLAG(&huart1);//清除标志位
//		//temp = huart1.Instance->SR;  //清除状态寄存器SR,读取SR寄存器可以实现清除SR寄存器的功能
//		//temp = huart1.Instance->DR; //读取数据寄存器中的数据
//		//这两句和上面那句等效
//		HAL_UART_DMAStop(&huart1); //  停止DMA传输，防止
//		temp  =  __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);// 获取DMA中未传输的数据个数   
//		//temp  = hdma_usart1_rx.Instance->NDTR;// 读取NDTR寄存器，获取DMA中未传输的数据个数，
//		
//		Uart_IDLE_RxCallback(temp);

//	 }
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
		break;
	
	default:
		break;
	}


}





void Uart_RxBuf_Check(void)
{

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(&huart1 == huart)
	{
			Uart1_IRQHandler();
			HAL_UART_Receive_IT(&huart1, rx_buffer, 1);		// 重新使能串口1接收中断
	}
}
