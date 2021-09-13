#ifndef __MPORT_H
#define __MPORT_H 

#include "main.h"

#include "usart.h"
#include "tim.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//MODBUS 接口层驱动代码	  
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/8/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2017-2027
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define TIM_RS485         htim6
#define HUSART_RS485      huart2
/* 使用485通信的时候才会用到使能IO */
#define RS485_REDE_PORT  GPIOB
#define RS485_REDE_PIN   GPIO_PIN_8
#define RS485_RX_MODE    HAL_GPIO_WritePin(RS485_REDE_PORT,RS485_REDE_PIN,GPIO_PIN_RESET)
#define RS485_TX_MODE    HAL_GPIO_WritePin(RS485_REDE_PORT,RS485_REDE_PIN,GPIO_PIN_SET)
 
void mp_init(UART_HandleTypeDef* uartHandle,u32 bound);			//初始化接口
void mp_send_data(u8* buf, u16 len);		//发送数据 

//自己写的回调中断  方便移植
void MY_HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim); 
void MY_HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);  

#endif


















