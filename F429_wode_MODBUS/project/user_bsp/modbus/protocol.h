#ifndef __PROTOCOL_H
#define __PROTOCOL_H 

#include "main.h"
#include "mport.h"
#include "mcheck.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//MODBUS Э�����������	  
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/8/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2017-2027
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 



#define M_MAX_FRAME_LENGTH	261		//���֡����Ϊ261�ֽ� ��ַ+����+֡��+���ݳ���+����+CRC(CRC16��2�ֽ�)
#define M_MIN_FRAME_LENGTH	5		  //��С֡����Ϊ5�ֽ� ��ַ+����+֡��+���ݳ���+CRC8/XOR/SUM



//���ؽ�����������Ͷ���
typedef enum
{
	MR_OK,						      //����
	MR_FRAME_FORMAT_ERR,		//֡��ʽ����	 
	MR_FRAME_CHECK_ERR,			//У��ֵ��λ 
	MR_MEMORY_ERR,				  //�ڴ���� 
  MB_ADDRESS_ERR,         //�ӻ���ַ����
}m_result;


//��modbus֡����
typedef struct
{ 
	u8  address;			//�豸��ַ��0���㲥��ַ��1~255���豸��ַ��
	u8  count;			  //֡���ܣ�0~255
	u16 regAdds;      //�Ĵ�����ַ
  u8  dataCount;    //�����ֽڼ�����ֻ�й�����Ϊ10H,0FH���������Ͳ��л�����Ϊ�ӻ�����01H,02H,03H,04H��
	u16 *data;				//���ݴ洢��
	u16 chkval;				//У��ֵ 
}mb_frameHandle;


//��modbusЭ������վ���������
typedef  struct
{
  u8 *rxbuf;
  u8 rxbuff;            //��ʱ���ջ���
  u16 rxlen;
	u8 frameok;						//һ֡���ݽ�����ɱ�ǣ�0����û��ʼ��1�������һ֡���ݵĽ��գ�2��һ֡�������ڽ�����
  u8 modbusAddre;        // modbus�豸��ַ��0Ϊ���� ��1~255���ӻ���ַ��
}mb_HandleTypedef;

extern mb_HandleTypedef m_ctrl_dev;			//����modbus������
	

void mb_framePackSend(mb_frameHandle *fx);     //����һ֡����
m_result mb_unpack_frame(mb_frameHandle* fx);  //����һ֡����
m_result mb_init(u8 modbusAddre, UART_HandleTypeDef* uartHandle, u32 bound);						//��ʼ��MODBUS,�����ڴ�
void mb_destroy(void);								//����MODBUS,�ͷ��ڴ�

#endif













