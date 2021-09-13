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
 * �ļ�����: bsp_w25qxx.c
 * ������Ա: CY
 * ��������: 2021-03-14
 * �ĵ�����:
 *
 *----------------------------------�汾��Ϣ------------------------------------
 * �汾����: V1.0
 * �汾˵��:
 *          |-1.0
 * 	         |-2021-03-14
 * 	          |-��ʼ�汾
 *------------------------------------------------------------------------------
 */
#include "./bsp_w25qxx.h"
#include "../typedef/bsp_typedef.h"

W25QXX_HandleTypedef spiFlash_1 = {
    1,
    0,
    0xFFFF,
    &hspi1,
    GPIOC,
    GPIO_PIN_7,
};

W25QXX_HandleTypedef spiFlash_2 = {
    2,
    0,
    0xFFFF,
    &hspi1,
    GPIOC,
    GPIO_PIN_5,
};

/**
 * ��������: W25QXX_ReadWriteOneByte
 * ��������: ��дһ���ֽ�
 * ����˵��: spiFlash_x[����] spi���ݶ�����
 *           TxData: [����]Ҫд����ֽ�
 * ����˵��: ��ȡ�����ֽ�
 * ������Ա: CY
 * ��������: 2021-02-26
 *------------------*
 * �޸���Ա:
 * �޸�����:
 * �޸�����:
 *------------------*
 */
u8 W25QXX_ReadWriteOneByte(W25QXX_HandleTypedef* spiFlash_x, u8 TxData) {
    u8 Rxdata;
    HAL_SPI_TransmitReceive(spiFlash_x->HSPI, &TxData, &Rxdata, 1, 1000);
    return Rxdata;
}

/**
 * ��������: W25QXX_WriteEnable
 * ��������: ��FLASH���� дʹ�� ����
 * ����˵��: spiFlash_x[����] spi���ݶ�����
 * ����˵��: None
 * ������Ա: CY
 * ��������: 2021-02-26
 *------------------*
 * ��������: ��WEL��λ
 *------------------*
 */
void W25QXX_WriteEnable(W25QXX_HandleTypedef* spiFlash_x) {
    W25QXX_CS_ON(spiFlash_x); //ʹ������
    W25QXX_ReadWriteOneByte(spiFlash_x, W25X_WriteEnable);     //����дʹ��ָ��
    W25QXX_CS_OFF(spiFlash_x); //ȡ��Ƭѡ
}

/**
 * ��������: W25QXX_WriteDisable
 * ��������: W25Q256д��ֹ
 * ����˵��: spiFlash_x[����] spi���ݶ�����
 * ����˵��: None
 * ������Ա: CY
 * ��������: 2021-02-26
 *------------------*
 * ��������: ��WEL����
 *------------------*
 */
void W25QXX_WriteDisable(W25QXX_HandleTypedef* spiFlash_x) {
    W25QXX_CS_ON(spiFlash_x); //ʹ������
    W25QXX_ReadWriteOneByte(spiFlash_x, W25X_WriteDisable);     //����д��ָֹ��
    W25QXX_CS_OFF(spiFlash_x); //ȡ��Ƭѡ
}

/**
 * ��������: W25QXX_Read_SR
 * ��������: ��ȡW25Q256��״̬�Ĵ���
 * ����˵��: spiFlash_x[����] spi���ݶ�����
 *           regno: [����] ״̬�Ĵ����ţ���:1~3
 * ����˵��: ״̬�Ĵ���ֵ
 * ������Ա: CY
 * ��������: 2021-02-26
 *------------------*
 * ��������:
 *           ״̬�Ĵ���1��
 *           BIT7  6   5   4   3   2   1   0
 *           SPR   RV  TB BP2 BP1 BP0 WEL BUSY
 *           SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
 *           TB,BP2,BP1,BP0:FLASH����д��������
 *           WEL:дʹ������
 *           BUSY:æ���λ(1,æ;0,����)
 *           Ĭ��:0x00
 *           ״̬�Ĵ���2��
 *           BIT7  6   5   4   3   2   1   0
 *           SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
 *           ״̬�Ĵ���3��
 *           BIT7      6    5    4   3   2   1   0
 *           HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
 *------------------*
 */
u8 W25QXX_Read_SR(W25QXX_HandleTypedef* spiFlash_x, u8 regno) {
    u8 byte = 0, command = 0;
    switch(regno) {
    case 1:
        command = W25X_ReadStatusReg1;  //��״̬�Ĵ���1ָ��
        break;
    case 2:
        command = W25X_ReadStatusReg2;  //��״̬�Ĵ���2ָ��
        break;
    case 3:
        command = W25X_ReadStatusReg3;  //��״̬�Ĵ���3ָ��
        break;
    default:
        command = W25X_ReadStatusReg1;
        break;
    }
    W25QXX_CS_ON(spiFlash_x); //ʹ������
    W25QXX_ReadWriteOneByte(spiFlash_x, command);       //���Ͷ�ȡ״̬�Ĵ�������
    byte = W25QXX_ReadWriteOneByte(spiFlash_x, 0Xff); //��ȡһ���ֽ�
    W25QXX_CS_OFF(spiFlash_x); //ȡ��Ƭѡ
    return byte;
}

/**
 * ��������: W25QXX_Write_SR
 * ��������: дW25Q256״̬�Ĵ���
 * ����˵��: spiFlash_x[����] spi���ݶ�����
 *           regno: [����] ״̬�Ĵ����ţ���:1~3
 *    			 sr: [����] д���ֵ
 * ����˵��: None
 * ������Ա: CY
 * ��������: 2021-02-26
 *------------------*
 * ��������:
 *------------------*
 */
void W25QXX_Write_SR(W25QXX_HandleTypedef* spiFlash_x, u8 regno, u8 sr) {
    u8 command = 0;
    switch(regno) {
    case 1:
        command = W25X_WriteStatusReg1;  //д״̬�Ĵ���1ָ��
        break;
    case 2:
        command = W25X_WriteStatusReg2;  //д״̬�Ĵ���2ָ��
        break;
    case 3:
        command = W25X_WriteStatusReg3;  //д״̬�Ĵ���3ָ��
        break;
    default:
        command = W25X_WriteStatusReg1;
        break;
    }
    W25QXX_CS_ON(spiFlash_x);   //ʹ������
    W25QXX_ReadWriteOneByte(spiFlash_x, command);         //����дȡ״̬�Ĵ�������
    W25QXX_ReadWriteOneByte(spiFlash_x, sr);                      //д��һ���ֽ�
    W25QXX_CS_OFF(spiFlash_x);  //ȡ��Ƭѡ
}

/**
 * ��������: W25QXX_WaitBusy
 * ��������: �ȴ�����
 * ����˵��: spiFlash_x[����] spi���ݶ�����
 * ����˵��: None
 * ������Ա: CY
 * ��������: 2021-02-26
 *------------------*
 * ��������:
 *
 *------------------*
 */
void W25QXX_WaitBusy(W25QXX_HandleTypedef* spiFlash_x) {
    while((W25QXX_Read_SR(spiFlash_x, 1) & 0x01) == 0x01); // �ȴ�BUSYλ���
}

/**
 * ��������: W25QXX_ChipReset
 * ��������: ��������оƬ
 * ����˵��: spiFlash_x[����] spi���ݶ�����
 * ����˵��: None
 * ������Ա: CY
 * ��������: 2021-03-04
 *------------------*
 * ��������:
 *           ��ʱ����û��
 *------------------*
 */
void W25QXX_ChipReset(W25QXX_HandleTypedef* spiFlash_x) {
    if(spiFlash_x->ready == 0) {
        W25QXX_Init(spiFlash_x);
    } else {
        W25QXX_WriteEnable(spiFlash_x);                  //SET WEL
        W25QXX_WaitBusy(spiFlash_x);
        W25QXX_CS_ON(spiFlash_x);   //ʹ������
        W25QXX_ReadWriteOneByte(spiFlash_x, W25X_ResetEnable);       //������������
        W25QXX_CS_OFF(spiFlash_x);  //ȡ��Ƭѡ
        W25QXX_WaitBusy(spiFlash_x);   				   //�ȴ�оƬ���ý���
        W25QXX_CS_ON(spiFlash_x);   //ʹ������
        W25QXX_ReadWriteOneByte(spiFlash_x, W25X_ResetMemory);       //������������
        W25QXX_CS_OFF(spiFlash_x);  //ȡ��Ƭѡ
        W25QXX_WaitBusy(spiFlash_x);   				   //�ȴ�оƬ���ý���
    }
}

/**
 * ��������: W25QXX_PowerDown
 * ��������: �������ģʽ
 * ����˵��: spiFlash_x[����] spi���ݶ�����
 * ����˵��: None
 * ������Ա: CY
 * ��������: 2021-02-26
 *------------------*
 * ��������:
 *
 *------------------*
 */
void W25QXX_PowerDown(W25QXX_HandleTypedef* spiFlash_x) {
    W25QXX_CS_ON(spiFlash_x);   //ʹ������
    W25QXX_ReadWriteOneByte(spiFlash_x, W25X_PowerDown);    //���͵�������
    W25QXX_CS_OFF(spiFlash_x);  //ȡ��Ƭѡ
    HAL_Delay(3);                            //�ȴ�TPD
}

/**
 * ��������: W25QXX_WAKEUP
 * ��������: ����
 * ����˵��: spiFlash_x[����] spi���ݶ�����
 * ����˵��: None
 * ������Ա: CY
 * ��������: 2021-02-26
 *------------------*
 * ��������:
 *
 *------------------*
 */
void W25QXX_WAKEUP(W25QXX_HandleTypedef* spiFlash_x) {
    W25QXX_CS_ON(spiFlash_x);   //ʹ������
    W25QXX_ReadWriteOneByte(spiFlash_x, W25X_ReleasePowerDown); //  ���ͻ���ָ��
    W25QXX_CS_OFF(spiFlash_x);  //ȡ��Ƭѡ
    HAL_Delay(3);                                //�ȴ�TRES1
}

/**
 * ��������: W25QXX_ReadID
 * ��������: ��ȡоƬID
 * ����˵��: spiFlash_x[����] spi���ݶ�����
 * ����˵��: None
 * ������Ա: CY
 * ��������: 2021-02-26
 *------------------*
 * ��������:
 *           ����ֵ����:
 *           0XEF13,��ʾоƬ�ͺ�ΪW25Q80
 *           0XEF14,��ʾоƬ�ͺ�ΪW25Q16
 *           0XEF15,��ʾоƬ�ͺ�ΪW25Q32
 *           0XEF16,��ʾоƬ�ͺ�ΪW25Q64
 *           0XEF17,��ʾоƬ�ͺ�ΪW25Q128
 *           0XEF18,��ʾоƬ�ͺ�ΪW25Q256
 *------------------*
 */
void W25QXX_ReadID(W25QXX_HandleTypedef* spiFlash_x) {
    u16 Temp = 0;
    W25QXX_CS_ON(spiFlash_x);   //ʹ������
    W25QXX_ReadWriteOneByte(spiFlash_x, 0x90); //���Ͷ�ȡID����
    W25QXX_ReadWriteOneByte(spiFlash_x, 0x00);
    W25QXX_ReadWriteOneByte(spiFlash_x, 0x00);
    W25QXX_ReadWriteOneByte(spiFlash_x, 0x00);
    Temp |= W25QXX_ReadWriteOneByte(spiFlash_x, 0xFF) << 8;
    Temp |= W25QXX_ReadWriteOneByte(spiFlash_x, 0xFF);
    W25QXX_CS_OFF(spiFlash_x);  //ȡ��Ƭѡ
    spiFlash_x->W25Qxx_Type = Temp;
}



//��ʼ��SPI FLASH��IO��


/**
 * ��������: W25QXX_Init
 * ��������: ��ʼ��W25QXX����W25Q256���ó�4�ֽڵ�ַģʽ
 * ����˵��: spiFlash_x[����] spi���ݶ�����
 * ����˵��: None
 * ������Ա: CY
 * ��������: 2021-02-26
 *------------------*
 * ��������:
 *           4KbytesΪһ��Sector
 *           16��SectorΪ1��Block
 *           W25Q256
 *           ����Ϊ32M�ֽ�,����512��Block,8192��Sector
 *------------------*
 */
void W25QXX_Init(W25QXX_HandleTypedef* spiFlash_x) {
    u8 temp;
    if(spiFlash_x->ready != 0) {
        return;
    } else {
        W25QXX_CS_OFF(spiFlash_x);  //ȡ��Ƭѡ
        W25QXX_ReadID(spiFlash_x);	      //��ȡFLASH ID.
        if(spiFlash_x->W25Qxx_Type == W25Q256) {            //SPI FLASHΪW25Q256
            temp = W25QXX_Read_SR(spiFlash_x, 3);           //��ȡ״̬�Ĵ���3���жϵ�ַģʽ
            if((temp & 0X01) == 0) {		    //�������4�ֽڵ�ַģʽ,�����4�ֽڵ�ַģʽ
                W25QXX_CS_ON(spiFlash_x);   //ʹ������
                W25QXX_ReadWriteOneByte(spiFlash_x, W25X_Enable4ByteAddr); //���ͽ���4�ֽڵ�ַģʽָ��
                W25QXX_CS_OFF(spiFlash_x);  //ȡ��Ƭѡ
            }
        }
    }
    spiFlash_x->ready = 1;
}

/**
 * ��������: W25QXX_Read
 * ��������: ��ȡSPI FLASH ��ָ����ַ��ʼ��ȡָ�����ȵ�����
 * ����˵��: spiFlash_x[����] spi���ݶ�����
 *           pBuffer: [���] ���ݴ洢��
 *			     ReadAddr: [����] ��ʼ��ȡ�ĵ�ַ(24bit)
 *			     NumByteToRead: [����] Ҫ��ȡ���ֽ���(���65535)
 * ����˵��: None
 * ������Ա: CY
 * ��������: 2021-02-26
 *------------------*
 * ��������:
 *
 *------------------*
 */
void W25QXX_Read(W25QXX_HandleTypedef* spiFlash_x, u8* pBuffer, u32 ReadAddr, u16 NumByteToRead) {
    u16 i;

    if(spiFlash_x->ready == 0) {
        return;
    } else {
        W25QXX_CS_ON(spiFlash_x);   //ʹ������
        W25QXX_ReadWriteOneByte(spiFlash_x, W25X_ReadData);     //���Ͷ�ȡ����
        if(spiFlash_x->W25Qxx_Type == W25Q256) {            //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
            W25QXX_ReadWriteOneByte(spiFlash_x, (u8)((ReadAddr) >> 24));
        }
        W25QXX_ReadWriteOneByte(spiFlash_x, (u8)((ReadAddr) >> 16)); //����24bit��ַ
        W25QXX_ReadWriteOneByte(spiFlash_x, (u8)((ReadAddr) >> 8));
        W25QXX_ReadWriteOneByte(spiFlash_x, (u8)ReadAddr);
        for(i = 0; i < NumByteToRead; i++) {
            pBuffer[i] = W25QXX_ReadWriteOneByte(spiFlash_x, 0XFF); //ѭ������
        }
        W25QXX_CS_OFF(spiFlash_x);  //ȡ��Ƭѡ
    }
}

/**
 * ��������: W25QXX_Write_Page
 * ��������: SPI��һҳ(0~65535)��д������256���ֽڵ�����
 * ����˵��: spiFlash_x[����] spi���ݶ�����
 *           pBuffer: [����] ���ݴ洢��
 *    			 WriteAddr: [����] ��ʼд��ĵ�ַ(24bit)
 *			     NumByteToWrite: [����] Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
 * ����˵��: None
 * ������Ա: CY
 * ��������: 2021-02-26
 *------------------*
 * ��������:
 *           ��ָ����ַ��ʼд�����256�ֽڵ�����
 *------------------*
 */
void W25QXX_Write_Page(W25QXX_HandleTypedef* spiFlash_x, u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite) {
    u16 i;
    if(spiFlash_x->ready == 0) {
        return;
    } else {
        W25QXX_WriteEnable(spiFlash_x);                  //SET WEL
        W25QXX_CS_ON(spiFlash_x);   //ʹ������
        W25QXX_ReadWriteOneByte(spiFlash_x, W25X_PageProgram);  //����дҳ����
        if(spiFlash_x->W25Qxx_Type == W25Q256) {            //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
            W25QXX_ReadWriteOneByte(spiFlash_x, (u8)((WriteAddr) >> 24));
        }
        W25QXX_ReadWriteOneByte(spiFlash_x, (u8)((WriteAddr) >> 16)); //����24bit��ַ
        W25QXX_ReadWriteOneByte(spiFlash_x, (u8)((WriteAddr) >> 8));
        W25QXX_ReadWriteOneByte(spiFlash_x, (u8)WriteAddr);
        for(i = 0; i < NumByteToWrite; i++) {
            W25QXX_ReadWriteOneByte(spiFlash_x, pBuffer[i]);  //ѭ��д��
        }
        W25QXX_CS_OFF(spiFlash_x);  //ȡ��Ƭѡ
        W25QXX_WaitBusy(spiFlash_x);					   //�ȴ�д�����
    }
}

/**
 * ��������: W25QXX_Write_NoCheck
 * ��������: �޼���дSPI FLASH
 * ����˵��: spiFlash_x[����] spi���ݶ�����
 *           pBuffer: [����] ���ݴ洢��
 *		     	 WriteAddr: [����] ��ʼд��ĵ�ַ(24bit)
 *		    	 NumByteToWrite: [����] Ҫд����ֽ���(���65535)
 * ����˵��: None
 * ������Ա: CY
 * ��������: 2021-02-26
 *------------------*
 * ��������:
 *           �޼���дSPI FLASH
 *           ����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
 *           �����Զ���ҳ����
 *           ��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
 *------------------*
 */
void W25QXX_Write_NoCheck(W25QXX_HandleTypedef* spiFlash_x, u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite) {
    u16 pageremain;
    pageremain = 256 - WriteAddr % 256; //��ҳʣ����ֽ���
    if(NumByteToWrite <= pageremain) {
        pageremain = NumByteToWrite; //������256���ֽ�
    }

    while(1) {
        W25QXX_Write_Page(spiFlash_x, pBuffer, WriteAddr, pageremain);
        if(NumByteToWrite == pageremain) {
            break;//д�������
        } else {  //NumByteToWrite>pageremain
            pBuffer += pageremain;
            WriteAddr += pageremain;

            NumByteToWrite -= pageremain;			 //��ȥ�Ѿ�д���˵��ֽ���
            if(NumByteToWrite > 256) {
                pageremain = 256; //һ�ο���д��256���ֽ�
            } else {
                pageremain = NumByteToWrite; 	 //����256���ֽ���
            }
        }
    }
}

/**
 * ��������: W25QXX_Write
 * ��������: дSPI FLASH ��ָ����ַ��ʼд��ָ�����ȵ�����
 * ����˵��: spiFlash_x[����] spi���ݶ�����
 *           pBuffer: [����] ���ݴ洢��
 *			     WriteAddr: [����] ��ʼд��ĵ�ַ(24bit)
 *		    	 NumByteToWrite: [����] Ҫд����ֽ���(���65535)
 * ����˵��: None
 * ������Ա: CY
 * ��������: 2021-02-26
 *------------------*
 * ��������:
 *           дSPI FLASH
 *           ��ָ����ַ��ʼд��ָ�����ȵ�����
 *           �ú�������������!
 *------------------*
 */
u8 W25QXX_BUFFER[4096];
void W25QXX_Write(W25QXX_HandleTypedef* spiFlash_x, u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite) {
    u32 secpos;
    u16 secoff;
    u16 secremain;
    u16 i;
    u8 * W25Q256_BUF;
    W25Q256_BUF = W25QXX_BUFFER;
    secpos = WriteAddr / 4096; //������ַ
    secoff = WriteAddr % 4096; //�������ڵ�ƫ��
    secremain = 4096 - secoff; //����ʣ��ռ��С
    if(NumByteToWrite <= secremain) {
        secremain = NumByteToWrite; //������4096���ֽ�
    }
    while(1) {
        W25QXX_Read(spiFlash_x, W25Q256_BUF, secpos * 4096, 4096); //������������������
        for(i = 0; i < secremain; i++) { //У������
            if(W25Q256_BUF[secoff + i] != 0XFF) {
                break;//��Ҫ����
            }
        }
        if(i < secremain) { //��Ҫ����
            W25QXX_SectorErase(spiFlash_x, secpos); //�����������
            for(i = 0; i < secremain; i++) { //����
                W25Q256_BUF[i + secoff] = pBuffer[i];
            }
            W25QXX_Write_NoCheck(spiFlash_x, W25Q256_BUF, secpos * 4096, 4096); //д����������

        } else {
            W25QXX_Write_NoCheck(spiFlash_x, pBuffer, WriteAddr, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
        }
        if(NumByteToWrite == secremain) {
            break;//д�������
        } else { //д��δ����
            secpos++;//������ַ��1
            secoff = 0; //ƫ��λ��Ϊ0

            pBuffer += secremain; //ָ��ƫ��
            WriteAddr += secremain; //д��ַƫ��
            NumByteToWrite -= secremain;				//�ֽ����ݼ�
            if(NumByteToWrite > 4096)secremain = 4096;	//��һ����������д����
            else secremain = NumByteToWrite;			//��һ����������д����
        }
    };
}

/**
 * ��������: W25QXX_ChipErase
 * ��������: ��������оƬ
 * ����˵��: spiFlash_x[����] spi���ݶ�����
 * ����˵��: None
 * ������Ա: CY
 * ��������: 2021-02-26
 *------------------*
 * ��������:
 *           �ȴ�ʱ�䳬��...
 *------------------*
 */
void W25QXX_ChipErase(W25QXX_HandleTypedef* spiFlash_x) {
    W25QXX_WriteEnable(spiFlash_x);                  //SET WEL
    W25QXX_WaitBusy(spiFlash_x);
    W25QXX_CS_ON(spiFlash_x);   //ʹ������
    W25QXX_ReadWriteOneByte(spiFlash_x, W25X_ChipErase);       //����Ƭ��������
    W25QXX_CS_OFF(spiFlash_x);  //ȡ��Ƭѡ
    W25QXX_WaitBusy(spiFlash_x);   				   //�ȴ�оƬ��������
}

/**
 * ��������: W25QXX_SectorErase
 * ��������: ����һ������
 * ����˵��: spiFlash_x[����] spi���ݶ�����
 *           Dst_Addr: [����] ������ַ ����ʵ����������
 * ����˵��: None
 * ������Ա: CY
 * ��������: 2021-02-26
 *------------------*
 * ��������:
 *           ����һ������������ʱ��:150ms
 *------------------*
 */
void W25QXX_SectorErase(W25QXX_HandleTypedef* spiFlash_x, u32 Dst_Addr) {
    Dst_Addr *= 4096;
    if(spiFlash_x->ready == 0) {
        return;
    } else {
        W25QXX_WriteEnable(spiFlash_x);                  //SET WEL
        W25QXX_WaitBusy(spiFlash_x);
        W25QXX_CS_ON(spiFlash_x);   //ʹ������
        W25QXX_ReadWriteOneByte(spiFlash_x, W25X_SectorErase);  //������������ָ��
        if(spiFlash_x->W25Qxx_Type == W25Q256) {            //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
            W25QXX_ReadWriteOneByte(spiFlash_x, (u8)((Dst_Addr) >> 24));
        }
        W25QXX_ReadWriteOneByte(spiFlash_x, (u8)((Dst_Addr) >> 16)); //����24bit��ַ
        W25QXX_ReadWriteOneByte(spiFlash_x, (u8)((Dst_Addr) >> 8));
        W25QXX_ReadWriteOneByte(spiFlash_x, (u8)Dst_Addr);
        W25QXX_CS_OFF(spiFlash_x);  //ȡ��Ƭѡ
        W25QXX_WaitBusy(spiFlash_x);   				    //�ȴ��������
    }
}

/********************************End of File************************************/

