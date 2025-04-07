#include "uart_mgmt.h"
#include "usart.h"

#define BUFFER_SIZE	512
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


void Uart_IDLE_RxCallback(uint16_t DMA_Idx)
{
	rx_len =  BUFFER_SIZE - DMA_Idx; //�ܼ�����ȥδ��������ݸ������õ��Ѿ����յ����ݸ���
	HAL_UART_Receive_DMA(&huart1, rx_buffer, BUFFER_SIZE);
	recv_end_flag = 1;	// ������ɱ�־λ��1
}


void Uart_RxBuf_Check_SHAKE_HANDS(void)
{
	static uint8_t z_count = 0;

	if(rx_buffer[0] == 'z')
	{
		z_count++;
		if(z_count >= 3)
		{
			uart_state = HEAD_HANDS;	// ���ֳɹ�������ͷ���׶�
			z_count = 0;				// ���ü�����
			HAL_UART_Transmit(&huart1, (uint8_t *)"AAA", 3, 100);	// ���յ���������ʹ�ô���1���ͳ�ȥ
		}
	}
	else
	{
		z_count = 0;					// ��'z'�ַ������ü�����
		uart_state = SHAKE_HANDS;		// ����ʧ�ܣ����½������ֽ׶�
	}
}

void Uart_RxBuf_Check_HEAD_HANDS(void)
{
//	uint32_t tmp_flag = 0;
//	uint32_t temp;
//	tmp_flag =__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE); //��ȡIDLE��־λ
//	if((tmp_flag != RESET))//idle��־����λ
//	{ 
//		__HAL_UART_CLEAR_IDLEFLAG(&huart1);//�����־λ
//		//temp = huart1.Instance->SR;  //���״̬�Ĵ���SR,��ȡSR�Ĵ�������ʵ�����SR�Ĵ����Ĺ���
//		//temp = huart1.Instance->DR; //��ȡ���ݼĴ����е�����
//		//������������Ǿ��Ч
//		HAL_UART_DMAStop(&huart1); //  ֹͣDMA���䣬��ֹ
//		temp  =  __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);// ��ȡDMA��δ��������ݸ���   
//		//temp  = hdma_usart1_rx.Instance->NDTR;// ��ȡNDTR�Ĵ�������ȡDMA��δ��������ݸ�����
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
			HAL_UART_Receive_IT(&huart1, rx_buffer, 1);		// ����ʹ�ܴ���1�����ж�
	}
}
