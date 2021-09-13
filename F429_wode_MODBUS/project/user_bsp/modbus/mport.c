#include "protocol.h"
#include "mcheck.h"
#include "mport.h"


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


//串口接收完成回调
void MY_HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == HUSART_RS485.Instance)
    {
        if(m_ctrl_dev.frameok == 0)
        {
            HAL_TIM_Base_Start_IT(&TIM_RS485);
            __HAL_TIM_CLEAR_IT(&TIM_RS485, TIM_IT_UPDATE);
            m_ctrl_dev.frameok = 2;
        }
        m_ctrl_dev.rxbuf[m_ctrl_dev.rxlen++] = m_ctrl_dev.rxbuff;
        __HAL_TIM_SET_COUNTER(&TIM_RS485,0);   //...计数清零，重新计数
        //TIM_RS485.Instance->CNT = 0;         //寄存器写法
        HAL_UART_Receive_IT(&HUSART_RS485, &(m_ctrl_dev.rxbuff), 1);
    }
}

//定时器溢出中断回调
void  MY_HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM_RS485.Instance)
    {
        m_ctrl_dev.frameok = 1;  //标记接收超时
        HAL_TIM_Base_Stop_IT(htim);
    }
}

////串口1中断服务函数
////如果是通过其他方式接受，请按要求实现数据存储
////将一帧数据完整的存储到m_ctrl_dev.rxbuf里面,并将帧数据长度存储在m_ctrl_dev.rxlen里面
//void USART2_IRQHandler(void)
//{
//  u8 res;
//  if(HUSART_RS485.Instance->SR&(1<<5))            //接收到数据
//  {
//      res=HUSART_RS485.Instance->DR;
//      if(m_ctrl_dev.frameok==0)   //接收未完成
//      {
//          m_ctrl_dev.rxbuf[m_ctrl_dev.rxlen]=res;
//          m_ctrl_dev.rxlen++;
//          if(m_ctrl_dev.rxlen>(M_MAX_FRAME_LENGTH-1))m_ctrl_dev.rxlen=0;//接收数据错误,重新开始接收
//          }
//  }else if(HUSART_RS485.Instance->SR&(1<<4))      //空闲中断
//  {
//      res=HUSART_RS485.Instance->DR;          //读DR，清除IDLE位
//      m_ctrl_dev.frameok=1;       //标记完成一帧数据接收
//  }
//}


//初始化接口
//这里我们使用串口1，得根据自己的需要修改
//串口1，我们利用空闲中断，做帧结束判断，如果其他接口，没有类似空闲状态监测电话
//必须自己实现帧数据截取（可以用超时法）
//pclk2：PCLK2时钟;  APB时钟
//bound：波特率
void mp_init(UART_HandleTypeDef *uartHandle, u32 bound)
{
    //  //波特率设置
    //  uartHandle->Instance->BRR = bound;
    //  USART1->BRR=(pclk2*1000000)/bound;
    //  USART1->CR1|=0X200C;  //1位停止,无校验位
    //  USART2->CR1|=1<<4;    //开启串口总线空闲中断.
    //使能接收中断
    //  USART2->CR1|=1<<5;    //接收缓冲区非空中断使能
    //  MY_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级
  
    /* 开启中断,存储在临时缓存中 */
    RS485_RX_MODE;  //把485模块默认为接收模式
    HAL_UART_Receive_IT(uartHandle, &(m_ctrl_dev.rxbuff), 1);         // Enable the USART2 Interrupt
}


//发送指定长度的数据
//buf：要发送的数据缓冲区首地址
//len：要发送的数据长度
void mp_send_data(u8 *buf, u16 len)
{
    RS485_TX_MODE;
    HAL_UART_Transmit(&HUSART_RS485, buf, len, 0xFFFF);
    //  u16 i=0;
    //  for(i=0;i<len;i++)
    //  {
    //      while((HUSART_RS485.Instance->SR&0X40)==0); //等待上一次串口数据发送完成
    //    HUSART_RS485.Instance->DR = buf[i];           //写DR,串口1将发送数据
    //  } 
    RS485_RX_MODE;  
}














