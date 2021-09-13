#ifndef __PROTOCOL_H
#define __PROTOCOL_H 

#include "main.h"
#include "mport.h"
#include "mcheck.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//MODBUS 协议层驱动代码	  
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/8/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2017-2027
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 



#define M_MAX_FRAME_LENGTH	261		//最大帧长度为261字节 地址+功能+帧号+数据长度+数据+CRC(CRC16有2字节)
#define M_MIN_FRAME_LENGTH	5		  //最小帧长度为5字节 地址+功能+帧号+数据长度+CRC8/XOR/SUM



//返回结果：错误类型定义
typedef enum
{
	MR_OK,						      //正常
	MR_FRAME_FORMAT_ERR,		//帧格式错误	 
	MR_FRAME_CHECK_ERR,			//校验值错位 
	MR_MEMORY_ERR,				  //内存错误 
  MB_ADDRESS_ERR,         //从机地址错误
}m_result;


//类modbus帧定义
typedef struct
{ 
	u8  address;			//设备地址：0，广播地址；1~255，设备地址。
	u8  count;			  //帧功能，0~255
	u16 regAdds;      //寄存器地址
  u8  dataCount;    //数据字节计数，只有功能码为10H,0FH的主机发送才有或者作为从机接收01H,02H,03H,04H用
	u16 *data;				//数据存储区
	u16 chkval;				//校验值 
}mb_frameHandle;


//类modbus协议主从站定义管理器
typedef  struct
{
  u8 *rxbuf;
  u8 rxbuff;            //临时接收缓存
  u16 rxlen;
	u8 frameok;						//一帧数据接收完成标记：0，还没开始；1，完成了一帧数据的接收；2，一帧数据正在接收中
  u8 modbusAddre;        // modbus设备地址：0为主机 ；1~255，从机地址。
}mb_HandleTypedef;

extern mb_HandleTypedef m_ctrl_dev;			//定义modbus控制器
	

void mb_framePackSend(mb_frameHandle *fx);     //发送一帧数据
m_result mb_unpack_frame(mb_frameHandle* fx);  //解析一帧数据
m_result mb_init(u8 modbusAddre, UART_HandleTypeDef* uartHandle, u32 bound);						//初始化MODBUS,申请内存
void mb_destroy(void);								//结束MODBUS,释放内存

#endif













