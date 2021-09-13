#ifndef __MPORT_H
#define __MPORT_H 

#include "main.h"

#include "usart.h"
#include "tim.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//MODBUS �ӿڲ���������	  
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/8/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2017-2027
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define TIM_RS485         htim6
#define HUSART_RS485      huart2
/* ʹ��485ͨ�ŵ�ʱ��Ż��õ�ʹ��IO */
#define RS485_REDE_PORT  GPIOB
#define RS485_REDE_PIN   GPIO_PIN_8
#define RS485_RX_MODE    HAL_GPIO_WritePin(RS485_REDE_PORT,RS485_REDE_PIN,GPIO_PIN_RESET)
#define RS485_TX_MODE    HAL_GPIO_WritePin(RS485_REDE_PORT,RS485_REDE_PIN,GPIO_PIN_SET)
 
void mp_init(UART_HandleTypeDef* uartHandle,u32 bound);			//��ʼ���ӿ�
void mp_send_data(u8* buf, u16 len);		//�������� 

//�Լ�д�Ļص��ж�  ������ֲ
void MY_HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim); 
void MY_HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);  

#endif


















