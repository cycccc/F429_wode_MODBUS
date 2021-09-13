#include "protocol.h"
#include "../typedef/bsp_typedef.h"
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


mb_HandleTypedef m_ctrl_dev;    //����modbus������


//����һ֡���ݣ���������洢��fx����  �������ж�
//ע�⣺���������õ�malloc��fx����ָ�������ڴ棬��������fx��һ��Ҫ�ͷ��ڴ棡��
//����������ڴ�й¶������
//fx��ָ֡��
//����ֵ�����������0��OK��������������롣
m_result mb_unpack_frame(mb_frameHandle* fx)
{
    u16 rxchkval = 0;           //���յ���У��ֵ
    u16 calchkval = 0;          //����õ���У��ֵ
    u8 datalen = 0;             //��Ч���ݳ���
    if (m_ctrl_dev.rxlen > M_MAX_FRAME_LENGTH || m_ctrl_dev.rxlen < M_MIN_FRAME_LENGTH)
    {
        m_ctrl_dev.rxlen = 0;       //���rxlen
        m_ctrl_dev.frameok = 0;     //���framok��ǣ��Ա��´ο�����������
        return MR_FRAME_FORMAT_ERR;//֡��ʽ����
    }
    if (m_ctrl_dev.modbusAddre == 0)  //���������
    {
        //����CRCУ��
        if ((m_ctrl_dev.rxbuf[1] == 0x01) || (m_ctrl_dev.rxbuf[1] == 0x02) || (m_ctrl_dev.rxbuf[1] == 0x03) || (m_ctrl_dev.rxbuf[1] == 0x04))
        {
            datalen = m_ctrl_dev.rxbuf[2];
            calchkval = mc_check_crc16(m_ctrl_dev.rxbuf, datalen + 3);
            rxchkval = ((u16)m_ctrl_dev.rxbuf[datalen + 3] << 8) + m_ctrl_dev.rxbuf[datalen + 4];
            m_ctrl_dev.rxlen = 0;         //���rxlen
            m_ctrl_dev.frameok = 0;   //���framok��ǣ��Ա��´ο�����������
            if (calchkval == rxchkval)   //У������
            {
                fx->address = m_ctrl_dev.rxbuf[0];
                fx->count = m_ctrl_dev.rxbuf[1];
                fx->dataCount = datalen;
                //...�������屣��
                fx->data = (u16*)malloc(datalen/2);
                if (fx->data)
                {
                    for (int j = 0; j < datalen/2; j++)
                    {
                        fx->data[j] = ((u16)m_ctrl_dev.rxbuf[3 + j * 2] << 8) + m_ctrl_dev.rxbuf[4 + j * 2];
                    }
                }
                // CRCУ�鱣��
                fx->chkval = rxchkval;
            }
            else
            {
                return MR_FRAME_CHECK_ERR;
            }
        }
        else
        {
            calchkval = mc_check_crc16(m_ctrl_dev.rxbuf, 6);
            rxchkval = ((u16)m_ctrl_dev.rxbuf[6] << 8) + m_ctrl_dev.rxbuf[7];
            m_ctrl_dev.rxlen = 0;         //���rxlen
            m_ctrl_dev.frameok = 0;   //���framok��ǣ��Ա��´ο�����������
            if (calchkval == rxchkval)   //У������
            {
                fx->address = m_ctrl_dev.rxbuf[0];
                fx->count = m_ctrl_dev.rxbuf[1];
                fx->regAdds = ((u16)m_ctrl_dev.rxbuf[2] << 8) + m_ctrl_dev.rxbuf[3];
                //...�������屣��
                fx->data = (u16*)malloc(1);
                if (fx->data)
                {
                    fx->data[0] = ((u16)m_ctrl_dev.rxbuf[4] << 8) + m_ctrl_dev.rxbuf[5];
                }
                // CRCУ�鱣��
                fx->chkval = rxchkval;
            }
            else
            {
                return MR_FRAME_CHECK_ERR;
            }
        }
    }
    else      //����Ǵӻ�
    {
        if (m_ctrl_dev.rxbuf[0] != m_ctrl_dev.modbusAddre)  //��֤��ַ�Ƿ�һ��
        {
            m_ctrl_dev.rxlen = 0;       //���rxlen
            m_ctrl_dev.frameok = 0;     //���framok��ǣ��Ա��´ο�����������
            return MB_ADDRESS_ERR;
        }
        //����CRCУ��
        if ((m_ctrl_dev.rxbuf[1] == 0x10) || (m_ctrl_dev.rxbuf[1] == 0x0F))
        {
            datalen = m_ctrl_dev.rxbuf[6];
            calchkval = mc_check_crc16(m_ctrl_dev.rxbuf, datalen + 7);
            rxchkval = ((u16)m_ctrl_dev.rxbuf[datalen + 7] << 8) + m_ctrl_dev.rxbuf[datalen + 8];
            m_ctrl_dev.rxlen = 0;          //���rxlen
            m_ctrl_dev.frameok = 0;      //���framok��ǣ��Ա��´ο�����������
            if (calchkval == rxchkval)   //У������
            {
                fx->address = m_ctrl_dev.rxbuf[0];
                fx->count = m_ctrl_dev.rxbuf[1];
                fx->regAdds = ((u16)m_ctrl_dev.rxbuf[2] << 8) + m_ctrl_dev.rxbuf[3];
                fx->dataCount = m_ctrl_dev.rxbuf[5];
                //...�������屣��
                fx->data = (u16*)malloc(datalen/2);
                if (fx->data)
                {
                    for (int j = 0; j < datalen / 2; j++)
                    {
                        fx->data[j] = ((u16)m_ctrl_dev.rxbuf[7 + j * 2] << 8) + m_ctrl_dev.rxbuf[8 + j * 2];
                    }
                }
                // CRCУ�鱣��
                fx->chkval = rxchkval;
            }
            else
            {
                return MR_FRAME_CHECK_ERR;
            }
        }
        else  // 01H,02H,03H,04H,05H,06H,
        {
            calchkval = mc_check_crc16(m_ctrl_dev.rxbuf, 6);
            rxchkval = ((u16)m_ctrl_dev.rxbuf[6] << 8) + m_ctrl_dev.rxbuf[7];
            m_ctrl_dev.rxlen = 0;        //���rxlen
            m_ctrl_dev.frameok = 0;      //���framok��ǣ��Ա��´ο�����������
            if (calchkval == rxchkval)   //У������
            {
                fx->address = m_ctrl_dev.rxbuf[0];
                fx->count = m_ctrl_dev.rxbuf[1];
                fx->regAdds = ((u16)m_ctrl_dev.rxbuf[2] << 8) + m_ctrl_dev.rxbuf[3];
                //...�������屣��
                fx->data = (u16*)malloc(1);
                if (fx->data)
                {
                    fx->data[0] = ((u16)m_ctrl_dev.rxbuf[4] << 8) + m_ctrl_dev.rxbuf[5];
                }
                // CRCУ�鱣��
                fx->chkval = rxchkval;
            }
            else
            {
                return MR_FRAME_CHECK_ERR;
            }
        }
    }
    return MR_OK;
}

//���һ֡���ݣ�������
//fx��ָ����Ҫ�����֡
void mb_framePackSend(mb_frameHandle *fx)
{
    u16 i = 0;                     //�ܳ���
    u8  len = 0;                   //��Ȧ����
    u16 calchkval = 0;             //����õ���У��ֵ
    u8 txbuf[M_MAX_FRAME_LENGTH];  //���ͻ�����
    //-------//
    txbuf[i++] = fx->address;
    txbuf[i++] = fx->count;
    txbuf[i++] = (u8)fx->regAdds >> 8;
    txbuf[i++] = (u8)fx->regAdds;
    switch(fx->count)
    {
        case 0x10:
            txbuf[i++] = 0x00;  //��ʽ��Ҫ
            txbuf[i++] = fx->dataCount;
            txbuf[i++] = (fx->dataCount) * 2;
            for(u8 j = 0; j < fx->dataCount; j++)
            {
                txbuf[i++] = (u8)(fx->data[j] >> 8);
                txbuf[i++] = (u8)(fx->data[j]);
            }
            break;
        case 0x0F:
            txbuf[i++] = 0x00;  //��ʽ��Ҫ
            txbuf[i++] = fx->dataCount;
            if(fx->dataCount > 8)
            {
                if(fx->dataCount % 8)
                {
                    len = (fx->dataCount / 8) + 1;
                }
                else
                {
                    len = (fx->dataCount / 8);
                }
            }
            else
            {
                len = 1;
            }
            txbuf[i++] = len;
            for(u8 j = 0; j < len; j++)
            {
                txbuf[i++] = (u8)(fx->data[j]);
            }
            break;
        default:
            txbuf[i++] = (u8)(fx->data[0] >> 8);
            txbuf[i++] = (u8)(fx->data[0]);
            break;
    }
    //-----//
    calchkval = mc_check_crc16(txbuf, i);
    fx->chkval = calchkval;
    txbuf[i++] = (calchkval >> 8) & 0XFF; //���ֽ���ǰ
    txbuf[i++] = calchkval & 0XFF;      //���ֽ��ں�
    mp_send_data(txbuf, i);   //������һ֡����
}


//��ʼ��modbus
//modbusAddre��modbus�豸��ַ��0Ϊ���� ��1~255���ӻ���ַ�� ����ѡ��Ժ��ڼ�
//����ֵ��0,�ɹ�;����,�������
m_result mb_init(u8 modbusAddre, UART_HandleTypeDef* uartHandle, u32 bound)
{
    m_ctrl_dev.rxbuf = (u8 *)malloc(M_MAX_FRAME_LENGTH); //��������֡���ջ���
    m_ctrl_dev.rxlen = 0;
    m_ctrl_dev.frameok = 0;
    m_ctrl_dev.modbusAddre = modbusAddre;
    mp_init(uartHandle, bound);  //��ʼ������Ӳ��
    if(m_ctrl_dev.rxbuf)
    {
        return MR_OK;
    }
    else
    {
        return MR_MEMORY_ERR;
    }
}

//����MODBUS,�ͷ��ڴ�
void mb_destroy(void)
{
    free(m_ctrl_dev.rxbuf); //��������֡���ջ���
    m_ctrl_dev.rxlen = 0;
    m_ctrl_dev.frameok = 0;
}























