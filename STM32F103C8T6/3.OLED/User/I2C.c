#include "I2C.h"

void I2C_My_Init(I2C_TypeDef *I2Cx)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // 复用开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    if (I2Cx == I2C1)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    }
    else
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    }
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*------------------------------------------------------I2C配置------------------------------------------------------*/
    I2C_DeInit(I2Cx);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;                                // I2C模式
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;                        // 占空比2
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 7位地址
    I2C_InitStructure.I2C_OwnAddress1 = OwnAddress1;                          // 本机地址
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;                               // 使能应答
    I2C_InitStructure.I2C_ClockSpeed = 400000;                                // 时钟速度400KHz
    I2C_Init(I2Cx, &I2C_InitStructure);
    I2C_Cmd(I2Cx, ENABLE);
}

void I2C_My_IT_Init(I2C_TypeDef *I2Cx)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // 响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 使能中断

    if (I2Cx == I2C1)
    {
        NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn; // I2C1事件中断
        NVIC_Init(&NVIC_InitStructure);
        NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn; // I2C1错误中断
        NVIC_Init(&NVIC_InitStructure);
    }
    else
    {
        NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn; // I2C2事件中断
        NVIC_Init(&NVIC_InitStructure);
        NVIC_InitStructure.NVIC_IRQChannel = I2C2_ER_IRQn; // I2C2错误中断
        NVIC_Init(&NVIC_InitStructure);
    }

    I2C_ITConfig(I2Cx, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, ENABLE); // 使能I2C1事件中断、缓冲区中断、错误中断
}

u8 I2C_START(I2C_TypeDef *I2Cx, u8 address, u8 direction)
{
    u16 I2C_TIMEOUT = I2C_TIMEROUT;

    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        if (!(I2C_TIMEOUT--))
            return I2C_BUSY;

    I2C_GenerateSTART(I2Cx, ENABLE);
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) // EV5,等待起始信号发送完成
        if (!(I2C_TIMEOUT--))
            return I2C_START_ERROR;

    I2C_Send7bitAddress(I2Cx, address, direction); // 发送从机地址,EV6,等待从机应答
    if (direction == I2C_Direction_Transmitter)
    {
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
            if (!(I2C_TIMEOUT--))
                return I2C_Slave_Address_Transmission_ERROR;
    }
    else
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
            if (!(I2C_TIMEOUT--))
                return I2C_Slave_Address_Transmission_ERROR;

    return 0;
}

u8 I2C_Write_Byte(I2C_TypeDef *I2Cx, u8 address, u8 dat)
{
    u16 I2C_TIMEOUT = I2C_TIMEROUT;
    u8 I2C_START_FLAG = I2C_START(I2Cx, address, I2C_Direction_Transmitter);
    if (I2C_START_FLAG)
        return I2C_START_FLAG;

    I2C_SendData(I2Cx, dat);                                                 // 发送数据
    while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR) // EV8_2,等待数据发送完成
        if (!(I2C_TIMEOUT--))
            return I2C_TRANSMITTER_ERROR;

    I2C_GenerateSTOP(I2Cx, ENABLE); // 发送停止信号

    return 0;
}

u8 I2C_Write_2_Bytes(I2C_TypeDef *I2Cx, u8 address, u8 dat1, u8 dat2)
{
    u16 I2C_TIMEOUT = I2C_TIMEROUT;

    u8 I2C_START_FLAG = I2C_START(I2Cx, address, I2C_Direction_Transmitter);
    if (I2C_START_FLAG)
        return I2C_START_FLAG;

    I2C_SendData(I2Cx, dat1);                                                 // 发送数据
    while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR) // EV8,等待数据从DR到移位寄存器,DR空移位寄存器非空
        if (!(I2C_TIMEOUT--))
            return I2C_TRANSMITTER_ERROR;

    I2C_SendData(I2Cx, dat2);                                                // 发送数据
    while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR) // EV8_2,等待数据发送完成
        if (!(I2C_TIMEOUT--))
            return I2C_TRANSMITTER_ERROR;

    I2C_GenerateSTOP(I2Cx, ENABLE); // 发送停止信号

    return 0;
}

u8 I2C_Write_Bytes(I2C_TypeDef *I2Cx, u8 address, u8 *dat, u32 len)
{
    u16 I2C_TIMEOUT = I2C_TIMEROUT;

    u8 I2C_START_FLAG = I2C_START(I2Cx, address, I2C_Direction_Transmitter);
    if (I2C_START_FLAG)
        return I2C_START_FLAG;

    for (u32 i = 0; i < len; i++)
    {
        I2C_SendData(I2Cx, dat[i]);                                               // 发送数据
        while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR) // EV8,等待数据从DR到移位寄存器,DR空移位寄存器非空
            if (!(I2C_TIMEOUT--))
                return I2C_TRANSMITTER_ERROR;
    }

    while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR) // EV8_2,等待数据发送完成
        if (!(I2C_TIMEOUT--))
            return I2C_TRANSMITTER_ERROR;

    I2C_GenerateSTOP(I2Cx, ENABLE); // 发送停止信号

    return 0;
}

u8 I2C_Read_Byte(I2C_TypeDef *I2Cx, u8 address)
{
    u16 I2C_TIMEOUT = I2C_TIMEROUT;

    u8 dat = 0;

    u8 I2C_START_FLAG = I2C_START(I2Cx, address, I2C_Direction_Receiver);
    if (I2C_START_FLAG)
        return I2C_START_FLAG;

    I2C_AcknowledgeConfig(I2Cx, DISABLE); // 关闭应答
    I2C_GenerateSTOP(I2Cx, ENABLE);       // 发送停止信号

    while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) == ERROR) // 等待数据接收完成
        if (!(I2C_TIMEOUT--))
            return I2C_RECEIVER_ERROR;

    dat = I2C_ReceiveData(I2Cx); // 读取数据
    return dat;
}

u8 I2C_Read_Bytes(I2C_TypeDef *I2Cx, u8 address, u8 *dat, u8 len)
{
    u16 I2C_TIMEOUT = I2C_TIMEROUT;

    u8 I2C_START_FLAG = I2C_START(I2Cx, address, I2C_Direction_Receiver);
    if (I2C_START_FLAG)
        return I2C_START_FLAG;

    for (u8 i = 0; i < len; i++)
    {
        if (i == len - 1)
        {
            I2C_AcknowledgeConfig(I2Cx, DISABLE); // 关闭应答
            I2C_GenerateSTOP(I2Cx, ENABLE);       // 发送停止信号
        }

        while (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) == ERROR) // 等待数据接收完成
            if (!(I2C_TIMEOUT--))
                return I2C_RECEIVER_ERROR;

        dat[i] = I2C_ReceiveData(I2Cx); // 读取数据
    }

    return 0;
}

/*------------------------------------------------

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

void I2C2_EV_IRQHandler(void)
{
    if (I2C_GetITStatus(I2C2, I2C_IT_EVT))
    {
        I2C_ClearITPendingBit(I2C2, I2C_IT_EVT);
    }
}

void I2C2_ER_IRQHandler(void)
{
    if (I2C_GetITStatus(I2C2, I2C_IT_ERR))
    {
        I2C_ClearITPendingBit(I2C2, I2C_IT_ERR);
    }
}

------------------------------------------------*/
