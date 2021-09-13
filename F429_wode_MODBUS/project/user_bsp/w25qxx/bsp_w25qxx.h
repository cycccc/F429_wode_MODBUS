/**
 *******************************Copyright (c)************************************
 *
 *                   (c) Copyright 2021, CY, China, QD.
 *                           All Rights Reserved
 *
 *                           By(南京万福祥电子科技有限公司)
 *                           http://www.njwfx.com
 *
 *----------------------------------文件信息------------------------------------
 * 文件名称: bsp_w25qxx.h
 * 创建人员: CY
 * 创建日期: 2021-03-14
 * 文档描述:
 *
 *----------------------------------版本信息------------------------------------
 * 实验平台
 *          |--
 * 版本代号: V1.0
 * 版本说明:
 *          |-1.0
 * 	         |-2021-03-14
 * 	          |-初始版本
 *------------------------------------------------------------------------------
 */

#ifndef __BSP_W25QXX_H_
#define __BSP_W25QXX_H_

#include "main.h"
#include "spi.h"

//w25qxx 系列指令表
#define W25X_WriteEnable		    0x06
#define W25X_WriteDisable		    0x04
#define W25X_ReadStatusReg1		  0x05
#define W25X_ReadStatusReg2		  0x35
#define W25X_ReadStatusReg3		  0x15
#define W25X_WriteStatusReg1    0x01
#define W25X_WriteStatusReg2    0x31
#define W25X_WriteStatusReg3    0x11
#define W25X_ReadData			      0x03
#define W25X_FastReadData		    0x0B
#define W25X_FastReadDual		    0x3B
#define W25X_PageProgram		    0x02
#define W25X_BlockErase			    0xD8
#define W25X_SectorErase		    0x20
#define W25X_ChipErase			    0xC7
#define W25X_PowerDown			    0xB9
#define W25X_ReleasePowerDown	  0xAB
#define W25X_DeviceID			      0xAB
#define W25X_ManufactDeviceID	  0x90
#define W25X_JedecDeviceID		  0x9F
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9
/* Reset 指令 */
#define W25X_ResetEnable        0x66
#define W25X_ResetMemory        0x99
#define W25X_ExitQspiMode       0xff
#define W25X_EnterQspiMode      0x38




//W25X系列/Q系列芯片列表
#define W25Q80 	0XEF13
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18

//变量声明----------------------------
//SPI 与 FLASH 定义
typedef struct {
    u8 flash_ID;                 ///>flashID编号
    u8 ready;                    ///>flash状态
    u16 W25Qxx_Type;             ///>芯片类型
    SPI_HandleTypeDef* HSPI;     ///>spi总线号
    GPIO_TypeDef* CS_PORT;       ///>片选线 端口
    u16 CS_PIN;                  ///>片选线 脚号
    //以下为 QSPI 的选项
    #ifdef FLASH_QSPI
    GPIO_TypeDef* HOLD_PORT;     ///> SPI 默认拉高
    u16 HOLD_PIN;                ///> SPI 默认拉高
    GPIO_TypeDef* WD_PORT;       ///> SPI 默认拉高
    u16 WD_PIN;                  ///> SPI 默认拉高
    #endif
} W25QXX_HandleTypedef;

//函数定义----------------------------
#define W25QXX_CS_ON(W25QXX_HT)   HAL_GPIO_WritePin(W25QXX_HT->CS_PORT,W25QXX_HT->CS_PIN,GPIO_PIN_RESET)  //打开片选是拉低操作
#define W25QXX_CS_OFF(W25QXX_HT)  HAL_GPIO_WritePin(W25QXX_HT->CS_PORT,W25QXX_HT->CS_PIN,GPIO_PIN_SET)    //关闭片选是拉高操作

#define HOLD_AND_WD_PULLUP(HOLD_PORT,HOLD_PIN,WD_PORT,WD_PIN)  {HAL_GPIO_WritePin(HOLD_PORT,HOLD_PIN,GPIO_PIN_SET);HAL_GPIO_WritePin(WD_PORT,WD_PIN,GPIO_PIN_SET);} //取消暂停和写保护


void W25QXX_Init(W25QXX_HandleTypedef* spiFlash_x);    //初始化W25QXX并将W25Q256设置成4字节地址模式
void W25QXX_Read(W25QXX_HandleTypedef* spiFlash_x, u8* pBuffer, u32 ReadAddr, u16 NumByteToRead); //读取SPI FLASH 在指定地址开始读取指定长度的数据
void W25QXX_Write_Page(W25QXX_HandleTypedef* spiFlash_x, u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite); //SPI在一页(0~65535)内写入少于256个字节的数据
void W25QXX_Write_NoCheck(W25QXX_HandleTypedef* spiFlash_x, u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite); //无检验写SPI FLASH
void W25QXX_Write(W25QXX_HandleTypedef* spiFlash_x, u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite); //写SPI FLASH 在指定地址开始写入指定长度的数据
void W25QXX_ChipErase(W25QXX_HandleTypedef* spiFlash_x);  //擦除整个芯片
void W25QXX_ChipReset(W25QXX_HandleTypedef* spiFlash_x);  //重置整个芯片
void W25QXX_SectorErase(W25QXX_HandleTypedef* spiFlash_x, u32 Dst_Addr); //擦除一个扇区

#endif

/********************************End of Head************************************/
