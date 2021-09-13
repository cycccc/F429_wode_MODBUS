#include "protocol.h"
#include "../typedef/bsp_typedef.h"
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


mb_HandleTypedef m_ctrl_dev;    //定义modbus控制器


//解析一帧数据，解析结果存储在fx里面  不进行判断
//注意：本函数会用到malloc给fx数据指针申请内存，后续用完fx，一定要释放内存！！
//否则会引起内存泄露！！！
//fx：帧指针
//返回值：解析结果，0，OK，其他，错误代码。
m_result mb_unpack_frame(mb_frameHandle* fx)
{
    u16 rxchkval = 0;           //接收到的校验值
    u16 calchkval = 0;          //计算得到的校验值
    u8 datalen = 0;             //有效数据长度
    if (m_ctrl_dev.rxlen > M_MAX_FRAME_LENGTH || m_ctrl_dev.rxlen < M_MIN_FRAME_LENGTH)
    {
        m_ctrl_dev.rxlen = 0;       //清除rxlen
        m_ctrl_dev.frameok = 0;     //清除framok标记，以便下次可以正常接收
        return MR_FRAME_FORMAT_ERR;//帧格式错误
    }
    if (m_ctrl_dev.modbusAddre == 0)  //如果是主机
    {
        //进行CRC校验
        if ((m_ctrl_dev.rxbuf[1] == 0x01) || (m_ctrl_dev.rxbuf[1] == 0x02) || (m_ctrl_dev.rxbuf[1] == 0x03) || (m_ctrl_dev.rxbuf[1] == 0x04))
        {
            datalen = m_ctrl_dev.rxbuf[2];
            calchkval = mc_check_crc16(m_ctrl_dev.rxbuf, datalen + 3);
            rxchkval = ((u16)m_ctrl_dev.rxbuf[datalen + 3] << 8) + m_ctrl_dev.rxbuf[datalen + 4];
            m_ctrl_dev.rxlen = 0;         //清除rxlen
            m_ctrl_dev.frameok = 0;   //清除framok标记，以便下次可以正常接收
            if (calchkval == rxchkval)   //校验正常
            {
                fx->address = m_ctrl_dev.rxbuf[0];
                fx->count = m_ctrl_dev.rxbuf[1];
                fx->dataCount = datalen;
                //...数据主体保存
                fx->data = (u16*)malloc(datalen/2);
                if (fx->data)
                {
                    for (int j = 0; j < datalen/2; j++)
                    {
                        fx->data[j] = ((u16)m_ctrl_dev.rxbuf[3 + j * 2] << 8) + m_ctrl_dev.rxbuf[4 + j * 2];
                    }
                }
                // CRC校验保存
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
            m_ctrl_dev.rxlen = 0;         //清除rxlen
            m_ctrl_dev.frameok = 0;   //清除framok标记，以便下次可以正常接收
            if (calchkval == rxchkval)   //校验正常
            {
                fx->address = m_ctrl_dev.rxbuf[0];
                fx->count = m_ctrl_dev.rxbuf[1];
                fx->regAdds = ((u16)m_ctrl_dev.rxbuf[2] << 8) + m_ctrl_dev.rxbuf[3];
                //...数据主体保存
                fx->data = (u16*)malloc(1);
                if (fx->data)
                {
                    fx->data[0] = ((u16)m_ctrl_dev.rxbuf[4] << 8) + m_ctrl_dev.rxbuf[5];
                }
                // CRC校验保存
                fx->chkval = rxchkval;
            }
            else
            {
                return MR_FRAME_CHECK_ERR;
            }
        }
    }
    else      //如果是从机
    {
        if (m_ctrl_dev.rxbuf[0] != m_ctrl_dev.modbusAddre)  //验证地址是否一致
        {
            m_ctrl_dev.rxlen = 0;       //清除rxlen
            m_ctrl_dev.frameok = 0;     //清除framok标记，以便下次可以正常接收
            return MB_ADDRESS_ERR;
        }
        //进行CRC校验
        if ((m_ctrl_dev.rxbuf[1] == 0x10) || (m_ctrl_dev.rxbuf[1] == 0x0F))
        {
            datalen = m_ctrl_dev.rxbuf[6];
            calchkval = mc_check_crc16(m_ctrl_dev.rxbuf, datalen + 7);
            rxchkval = ((u16)m_ctrl_dev.rxbuf[datalen + 7] << 8) + m_ctrl_dev.rxbuf[datalen + 8];
            m_ctrl_dev.rxlen = 0;          //清除rxlen
            m_ctrl_dev.frameok = 0;      //清除framok标记，以便下次可以正常接收
            if (calchkval == rxchkval)   //校验正常
            {
                fx->address = m_ctrl_dev.rxbuf[0];
                fx->count = m_ctrl_dev.rxbuf[1];
                fx->regAdds = ((u16)m_ctrl_dev.rxbuf[2] << 8) + m_ctrl_dev.rxbuf[3];
                fx->dataCount = m_ctrl_dev.rxbuf[5];
                //...数据主体保存
                fx->data = (u16*)malloc(datalen/2);
                if (fx->data)
                {
                    for (int j = 0; j < datalen / 2; j++)
                    {
                        fx->data[j] = ((u16)m_ctrl_dev.rxbuf[7 + j * 2] << 8) + m_ctrl_dev.rxbuf[8 + j * 2];
                    }
                }
                // CRC校验保存
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
            m_ctrl_dev.rxlen = 0;        //清除rxlen
            m_ctrl_dev.frameok = 0;      //清除framok标记，以便下次可以正常接收
            if (calchkval == rxchkval)   //校验正常
            {
                fx->address = m_ctrl_dev.rxbuf[0];
                fx->count = m_ctrl_dev.rxbuf[1];
                fx->regAdds = ((u16)m_ctrl_dev.rxbuf[2] << 8) + m_ctrl_dev.rxbuf[3];
                //...数据主体保存
                fx->data = (u16*)malloc(1);
                if (fx->data)
                {
                    fx->data[0] = ((u16)m_ctrl_dev.rxbuf[4] << 8) + m_ctrl_dev.rxbuf[5];
                }
                // CRC校验保存
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

//打包一帧数据，并发送
//fx：指向需要打包的帧
void mb_framePackSend(mb_frameHandle *fx)
{
    u16 i = 0;                     //总长度
    u8  len = 0;                   //线圈长度
    u16 calchkval = 0;             //计算得到的校验值
    u8 txbuf[M_MAX_FRAME_LENGTH];  //发送缓冲区
    //-------//
    txbuf[i++] = fx->address;
    txbuf[i++] = fx->count;
    txbuf[i++] = (u8)fx->regAdds >> 8;
    txbuf[i++] = (u8)fx->regAdds;
    switch(fx->count)
    {
        case 0x10:
            txbuf[i++] = 0x00;  //格式需要
            txbuf[i++] = fx->dataCount;
            txbuf[i++] = (fx->dataCount) * 2;
            for(u8 j = 0; j < fx->dataCount; j++)
            {
                txbuf[i++] = (u8)(fx->data[j] >> 8);
                txbuf[i++] = (u8)(fx->data[j]);
            }
            break;
        case 0x0F:
            txbuf[i++] = 0x00;  //格式需要
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
    txbuf[i++] = (calchkval >> 8) & 0XFF; //高字节在前
    txbuf[i++] = calchkval & 0XFF;      //低字节在后
    mp_send_data(txbuf, i);   //发送这一帧数据
}


//初始化modbus
//modbusAddre：modbus设备地址：0为主机 ；1~255，从机地址。 其他选项，以后在加
//返回值：0,成功;其他,错误代码
m_result mb_init(u8 modbusAddre, UART_HandleTypeDef* uartHandle, u32 bound)
{
    m_ctrl_dev.rxbuf = (u8 *)malloc(M_MAX_FRAME_LENGTH); //申请最大的帧接收缓存
    m_ctrl_dev.rxlen = 0;
    m_ctrl_dev.frameok = 0;
    m_ctrl_dev.modbusAddre = modbusAddre;
    mp_init(uartHandle, bound);  //初始化串口硬件
    if(m_ctrl_dev.rxbuf)
    {
        return MR_OK;
    }
    else
    {
        return MR_MEMORY_ERR;
    }
}

//结束MODBUS,释放内存
void mb_destroy(void)
{
    free(m_ctrl_dev.rxbuf); //申请最大的帧接收缓存
    m_ctrl_dev.rxlen = 0;
    m_ctrl_dev.frameok = 0;
}























