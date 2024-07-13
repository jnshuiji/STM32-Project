#include "I2C.h"

void I2C_init(void)
{
    /*------------------------------------------------------时钟配置------------------------------------------------------*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    /*------------------------------------------------------引脚配置------------------------------------------------------*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // 复用开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*------------------------------------------------------I2C中断配置------------------------------------------------------*/
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // 响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 使能中断

    NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn; // I2C1事件中断
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn; // I2C1错误中断
    NVIC_Init(&NVIC_InitStructure);

    I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, ENABLE); // 使能I2C1事件中断、缓冲区中断、错误中断

    /*------------------------------------------------------I2C配置------------------------------------------------------*/
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;                                // I2C模式
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;                        // 占空比2
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 7位地址
    I2C_InitStructure.I2C_OwnAddress1 = 0x30;                                 // 本机地址
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;                               // 使能应答
    I2C_InitStructure.I2C_ClockSpeed = 400000;                                // 时钟速度400KHz
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
}

void I2C1_EV_IRQHandler(void)
{
    if (I2C_GetITStatus(I2C1, I2C_IT_EVT))
    {
        I2C_ClearITPendingBit(I2C1, I2C_IT_EVT);
    }
}

void I2C1_ER_IRQHandler(void)
{
    if (I2C_GetITStatus(I2C1, I2C_IT_ERR))
    {
        I2C_ClearITPendingBit(I2C1, I2C_IT_ERR);
    }
}

void I2C_Write_Byte(I2C_TypeDef *I2Cx, u8 address, u8 dat)
{
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        return;

    I2C_GenerateSTART(I2Cx, ENABLE);
    while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) // EV5,等待起始信号发送完成
        ;

    I2C_Send7bitAddress(I2Cx, address, I2C_Direction_Transmitter);                    // 发送从机地址
    while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR) // EV6,等待从机应答
        ;

    I2C_SendData(I2Cx, dat);                                                 // 发送数据
    while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR) // EV8_2,等待数据发送完成
        ;

    I2C_GenerateSTOP(I2Cx, ENABLE); // 发送停止信号
}

void I2C_Write_Bytes(I2C_TypeDef *I2Cx, u8 address, u8 *dat, u32 len)
{
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        return;

    I2C_GenerateSTART(I2Cx, ENABLE);
    while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) // EV5,等待起始信号发送完成
        ;

    I2C_Send7bitAddress(I2Cx, address, I2C_Direction_Transmitter);                    // 发送从机地址
    while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR) // EV6,等待从机应答
        ;

    for (u32 i = 0; i < len; i++)
    {
        I2C_SendData(I2Cx, dat[i]);                                               // 发送数据
        while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR) // EV8,等待数据从DR到移位寄存器,DR空移位寄存器非空
            ;
    }

    while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR) // EV8_2,等待数据发送完成
        ;

    I2C_GenerateSTOP(I2Cx, ENABLE); // 发送停止信号
}

u8 I2C_Read_Byte(I2C_TypeDef *I2Cx, u8 address)
{
    u8 dat = 0;
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        return 0;

    I2C_GenerateSTART(I2Cx, ENABLE);
    while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) // EV5,等待起始信号发送完成
        ;

    I2C_Send7bitAddress(I2Cx, address, I2C_Direction_Receiver);                    // 发送从机地址
    while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == ERROR) // EV6,等待从机应答
        ;

    I2C_AcknowledgeConfig(I2Cx, DISABLE); // 关闭应答
    I2C_GenerateSTOP(I2Cx, ENABLE);       // 发送停止信号

    while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) == ERROR) // 等待数据接收完成
        ;

    dat = I2C_ReceiveData(I2Cx); // 读取数据
    return dat;
}

void I2C_Read_Bytes(I2C_TypeDef *I2Cx, u8 address, u8 *dat, u8 len)
{
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        return;

    I2C_GenerateSTART(I2Cx, ENABLE);
    while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) // EV5,等待起始信号发送完成
        ;

    I2C_Send7bitAddress(I2Cx, address, I2C_Direction_Receiver);                    // 发送从机地址
    while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == ERROR) // EV6,等待从机应答
        ;

    for (u8 i = 0; i < len; i++)
    {
        if (i == len - 1)
        {
            I2C_AcknowledgeConfig(I2Cx, DISABLE); // 关闭应答
            I2C_GenerateSTOP(I2Cx, ENABLE);       // 发送停止信号
        }

        while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) == ERROR) // 等待数据接收完成
            ;

        dat[i] = I2C_ReceiveData(I2Cx); // 读取数据
    }
}