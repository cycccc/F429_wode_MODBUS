#include "protocol.h"
#include "mcheck.h"
#include "mport.h"


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


//���ڽ�����ɻص�
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
        __HAL_TIM_SET_COUNTER(&TIM_RS485,0);   //...�������㣬���¼���
        //TIM_RS485.Instance->CNT = 0;         //�Ĵ���д��
        HAL_UART_Receive_IT(&HUSART_RS485, &(m_ctrl_dev.rxbuff), 1);
    }
}

//��ʱ������жϻص�
void  MY_HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM_RS485.Instance)
    {
        m_ctrl_dev.frameok = 1;  //��ǽ��ճ�ʱ
        HAL_TIM_Base_Stop_IT(htim);
    }
}

////����1�жϷ�����
////�����ͨ��������ʽ���ܣ��밴Ҫ��ʵ�����ݴ洢
////��һ֡���������Ĵ洢��m_ctrl_dev.rxbuf����,����֡���ݳ��ȴ洢��m_ctrl_dev.rxlen����
//void USART2_IRQHandler(void)
//{
//  u8 res;
//  if(HUSART_RS485.Instance->SR&(1<<5))            //���յ�����
//  {
//      res=HUSART_RS485.Instance->DR;
//      if(m_ctrl_dev.frameok==0)   //����δ���
//      {
//          m_ctrl_dev.rxbuf[m_ctrl_dev.rxlen]=res;
//          m_ctrl_dev.rxlen++;
//          if(m_ctrl_dev.rxlen>(M_MAX_FRAME_LENGTH-1))m_ctrl_dev.rxlen=0;//�������ݴ���,���¿�ʼ����
//          }
//  }else if(HUSART_RS485.Instance->SR&(1<<4))      //�����ж�
//  {
//      res=HUSART_RS485.Instance->DR;          //��DR�����IDLEλ
//      m_ctrl_dev.frameok=1;       //������һ֡���ݽ���
//  }
//}


//��ʼ���ӿ�
//��������ʹ�ô���1���ø����Լ�����Ҫ�޸�
//����1���������ÿ����жϣ���֡�����жϣ���������ӿڣ�û�����ƿ���״̬���绰
//�����Լ�ʵ��֡���ݽ�ȡ�������ó�ʱ����
//pclk2��PCLK2ʱ��;  APBʱ��
//bound��������
void mp_init(UART_HandleTypeDef *uartHandle, u32 bound)
{
    //  //����������
    //  uartHandle->Instance->BRR = bound;
    //  USART1->BRR=(pclk2*1000000)/bound;
    //  USART1->CR1|=0X200C;  //1λֹͣ,��У��λ
    //  USART2->CR1|=1<<4;    //�����������߿����ж�.
    //ʹ�ܽ����ж�
    //  USART2->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��
    //  MY_NVIC_Init(3,3,USART1_IRQn,2);//��2��������ȼ�
  
    /* �����ж�,�洢����ʱ������ */
    RS485_RX_MODE;  //��485ģ��Ĭ��Ϊ����ģʽ
    HAL_UART_Receive_IT(uartHandle, &(m_ctrl_dev.rxbuff), 1);         // Enable the USART2 Interrupt
}


//����ָ�����ȵ�����
//buf��Ҫ���͵����ݻ������׵�ַ
//len��Ҫ���͵����ݳ���
void mp_send_data(u8 *buf, u16 len)
{
    RS485_TX_MODE;
    HAL_UART_Transmit(&HUSART_RS485, buf, len, 0xFFFF);
    //  u16 i=0;
    //  for(i=0;i<len;i++)
    //  {
    //      while((HUSART_RS485.Instance->SR&0X40)==0); //�ȴ���һ�δ������ݷ������
    //    HUSART_RS485.Instance->DR = buf[i];           //дDR,����1����������
    //  } 
    RS485_RX_MODE;  
}














