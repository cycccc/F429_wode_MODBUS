/**
 *******************************Copyright (c)************************************
 *
 *                   (c) Copyright 2021, CY, China, QD.
 *                           All Rights Reserved
 *
 *                           By(�Ͼ�������ӿƼ����޹�˾)
 *                           http://www.njwfx.com
 *
 *----------------------------------�ļ���Ϣ------------------------------------
 * �ļ�����: bsp_w25qxx.h
 * ������Ա: CY
 * ��������: 2021-03-14
 * �ĵ�����:
 *
 *----------------------------------�汾��Ϣ------------------------------------
 * ʵ��ƽ̨
 *          |--
 * �汾����: V1.0
 * �汾˵��:
 *          |-1.0
 * 	         |-2021-03-14
 * 	          |-��ʼ�汾
 *------------------------------------------------------------------------------
 */

#ifndef __BSP_W25QXX_H_
#define __BSP_W25QXX_H_

#include "main.h"
#include "spi.h"

//w25qxx ϵ��ָ���
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
/* Reset ָ�� */
#define W25X_ResetEnable        0x66
#define W25X_ResetMemory        0x99
#define W25X_ExitQspiMode       0xff
#define W25X_EnterQspiMode      0x38




//W25Xϵ��/Qϵ��оƬ�б�
#define W25Q80 	0XEF13
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18

//��������----------------------------
//SPI �� FLASH ����
typedef struct {
    u8 flash_ID;                 ///>flashID���
    u8 ready;                    ///>flash״̬
    u16 W25Qxx_Type;             ///>оƬ����
    SPI_HandleTypeDef* HSPI;     ///>spi���ߺ�
    GPIO_TypeDef* CS_PORT;       ///>Ƭѡ�� �˿�
    u16 CS_PIN;                  ///>Ƭѡ�� �ź�
    //����Ϊ QSPI ��ѡ��
    #ifdef FLASH_QSPI
    GPIO_TypeDef* HOLD_PORT;     ///> SPI Ĭ������
    u16 HOLD_PIN;                ///> SPI Ĭ������
    GPIO_TypeDef* WD_PORT;       ///> SPI Ĭ������
    u16 WD_PIN;                  ///> SPI Ĭ������
    #endif
} W25QXX_HandleTypedef;

//��������----------------------------
#define W25QXX_CS_ON(W25QXX_HT)   HAL_GPIO_WritePin(W25QXX_HT->CS_PORT,W25QXX_HT->CS_PIN,GPIO_PIN_RESET)  //��Ƭѡ�����Ͳ���
#define W25QXX_CS_OFF(W25QXX_HT)  HAL_GPIO_WritePin(W25QXX_HT->CS_PORT,W25QXX_HT->CS_PIN,GPIO_PIN_SET)    //�ر�Ƭѡ�����߲���

#define HOLD_AND_WD_PULLUP(HOLD_PORT,HOLD_PIN,WD_PORT,WD_PIN)  {HAL_GPIO_WritePin(HOLD_PORT,HOLD_PIN,GPIO_PIN_SET);HAL_GPIO_WritePin(WD_PORT,WD_PIN,GPIO_PIN_SET);} //ȡ����ͣ��д����


void W25QXX_Init(W25QXX_HandleTypedef* spiFlash_x);    //��ʼ��W25QXX����W25Q256���ó�4�ֽڵ�ַģʽ
void W25QXX_Read(W25QXX_HandleTypedef* spiFlash_x, u8* pBuffer, u32 ReadAddr, u16 NumByteToRead); //��ȡSPI FLASH ��ָ����ַ��ʼ��ȡָ�����ȵ�����
void W25QXX_Write_Page(W25QXX_HandleTypedef* spiFlash_x, u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite); //SPI��һҳ(0~65535)��д������256���ֽڵ�����
void W25QXX_Write_NoCheck(W25QXX_HandleTypedef* spiFlash_x, u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite); //�޼���дSPI FLASH
void W25QXX_Write(W25QXX_HandleTypedef* spiFlash_x, u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite); //дSPI FLASH ��ָ����ַ��ʼд��ָ�����ȵ�����
void W25QXX_ChipErase(W25QXX_HandleTypedef* spiFlash_x);  //��������оƬ
void W25QXX_ChipReset(W25QXX_HandleTypedef* spiFlash_x);  //��������оƬ
void W25QXX_SectorErase(W25QXX_HandleTypedef* spiFlash_x, u32 Dst_Addr); //����һ������

#endif

/********************************End of Head************************************/
